//#define __INFOMACRO__

#include "../utils/macro.hpp"
#include "parser_engine.hpp"
#include "parser_exception.hpp"
#include <sstream>

namespace chaos_parser {

	parser_context::parser_context() :
			lex { } {
	}

	void parser_context::setStream(std::istream &in) {
		lex.set_stream(in);
		collected.clear();
		while (!ncoll.empty())
			ncoll.pop();
	}

	void parser_context::setComment(const std::string &comment_begin, const std::string &comment_end, const std::string &comment_single_line) {
		lex.set_comment(comment_begin, comment_end, comment_single_line);
	}

	token_val parser_context::tryToken(const token &tk) {
		return lex.try_token(tk);
	}

	std::string parser_context::extract(const std::string &op, const std::string &cl) {
		return lex.extract(op, cl);
	}

	std::string parser_context::extractLine() {
		return lex.extract_line();
	}

	void parser_context::pushToken(token_val tk) {
		collected.push_back(tk);
	}

	void parser_context::pushToken(const std::string &s) {
		collected.push_back( { LEX_EXTRACTED_STRING, s });
	}

	void parser_context::save() {
		lex.save();
		ncoll.push(collected.size());
	}

	void parser_context::restore() {
		lex.restore();
		unsigned lev = ncoll.top();
		ncoll.pop();
		while (lev != collected.size())
			collected.pop_back();
	}

	void parser_context::discardSaved() {
		lex.discard_saved();
		ncoll.pop();
	}

	token_val parser_context::getLastToken() {
		return collected[collected.size() - 1];
	}

	std::vector<token_val> parser_context::collectTokens() {
		auto c = collected;
		collected.clear();
		return c;
	}

	void parser_context::setError(const token_val &err_msg) {
		error_token_msg = err_msg;
	}

	std::string parser_context::getFormattedErrMsg() {
		std::stringstream err;
		err << "At line " << lex.get_pos().first << ", column " << lex.get_pos().second << std::endl;
		err << lex.get_currline() << std::endl;
		for (int i = 0; i < lex.get_pos().second - 1; ++i)
			err << " ";
		err << "^" << std::endl;
		err << "Error code: " << error_token_msg.first << std::endl;
		err << "Error msg : " << error_token_msg.second << std::endl;
		return err.str();
	}

	/* ----------------------------------------------- */

//	 Classe Astratta che verra implementata da ogni regola per definire le azioni
	class abs_rule {
		protected:
			action_t fun;
		public:
			abs_rule() :
					fun(nullptr) {
			}
			virtual bool parse(parser_context &pc) = 0;
			bool action(parser_context &pc);
			virtual ~abs_rule() {
			}

			void install_action(action_t);
	};

	void abs_rule::install_action(action_t f) {
		fun = f;
	}

	bool abs_rule::action(parser_context &pc) {
		if (fun) {
			INFO_LINE("-- action found");
			//Chiamata a fun con il parametro lasciato in sospeso nel bind
			fun(pc);
			INFO_LINE("-- action completed");
		}
		return true;
	}

	//	 Implementazione delle regole , struttura dati che contiene puntatore all'implementazione della regola*/
	struct impl_rule {
			std::shared_ptr<abs_rule> abs_impl;

			impl_rule() :
					abs_impl(nullptr) {
			}
			impl_rule(abs_rule *r) :
					abs_impl(r) {
			}

			bool parse(parser_context &pc) {
				if (!abs_impl)
					return false;

				bool f = abs_impl->parse(pc);
				if (f)
					abs_impl->action(pc);
				return f;
			}
			bool action(parser_context &pc) {
				if (!abs_impl)
					return false;
				return abs_impl->action(pc);
			}
			void install_action(action_t f) {
				abs_impl->install_action(f);
			}

	};

	/* ----------------------------------------------- */

	class term_rule: public abs_rule {
			token mytoken;
			bool collect;
		public:
			term_rule(const token &tk, bool c = true) :
					mytoken(tk), collect(c) {
			}
			virtual bool parse(parser_context &pc);
	};

	/* ----------------------------------------------- */

	rule::rule() :
			pimpl(new impl_rule()) {
	}

	rule::rule(const rule &r) :
			pimpl(r.pimpl) {
	}

	rule::rule(std::shared_ptr<impl_rule> ir) :
			pimpl(ir) {
	}

	static std::string padding(const std::string &p) {
		static std::string elements { ".[{}()\\*+?|^$" };
		std::string r;
		for (auto c : p) {
			if (elements.find_first_of(c) != std::string::npos)
				r.append("\\");
			r.append(1, c);
		}
		INFO_LINE("PADDING RESULTS: " << r);
		return r;
	}

	rule::rule(char c, bool collect) {
		std::string p { c };
		p = padding(p);
		token tk = { LEX_CHAR, p };
		pimpl = std::make_shared<impl_rule>(new term_rule(tk, collect));
	}

	rule::rule(const std::string &s, bool collect) {
		std::string p = padding(s);
		token tk = { LEX_CHAR, p };
		pimpl = std::make_shared<impl_rule>(new term_rule(tk, collect));
	}

	rule::rule(const token &tk) :
			pimpl(new impl_rule(new term_rule(tk, true))) {
	}

	rule & rule::operator=(const rule &r) {
		pimpl->abs_impl = r.pimpl->abs_impl;
		return *this;
	}

	bool rule::parse(parser_context &pc) {
		bool f = pimpl->parse(pc);
		return f;
	}

	rule& rule::operator[](action_t af) {
		pimpl->install_action(af);
		INFO_LINE("Action installed");
		return *this;
	}

	bool term_rule::parse(parser_context &pc) {
		INFO_LINE("term_rule::parse() trying " << mytoken.get_expr());
		token_val result = pc.tryToken(mytoken);
		INFO_LINE("term_rule::parse() completed on " << result.first);
		if (result.first == mytoken.get_name()) {
			INFO_LINE(" ** ok");
			if (collect)
				pc.pushToken(result);
			return true;
		} else {
			INFO_LINE(" ** FALSE");
			pc.setError(result);
			return false;
		}
	}

	/* ----------------------------------------------- */

//	  Classe che definisce la sequenza di regole da valutare
	class seq_rule: public abs_rule {
			std::vector<std::shared_ptr<impl_rule> > rl;
		public:
			seq_rule(rule a, rule b);

			virtual bool parse(parser_context &pc);
	};

	/* ----------------------------------------------- */

	seq_rule::seq_rule(rule a, rule b) {
		rl.push_back(a.get_pimpl());
		rl.push_back(b.get_pimpl());

	}

	bool seq_rule::parse(parser_context &pc) {
		INFO("seq_rule::parse()");
		pc.save();
		for (auto &x : rl) {
			if (!x->parse(pc)) {
				pc.setError( { ERR_PARSE_SEQ, "Wrong element in sequence" });
				INFO_LINE(" ** FALSE ");
				pc.restore();
				return false;
			}
		}
		INFO_LINE(" ** ok ");
		return true;
	}

	rule operator>>(rule a, rule b) {
		auto s = std::make_shared<impl_rule>(new seq_rule(a, b));
		return rule(s);
	}

	/* -------------------------------------------- */

	// Classe che definisce la sequenza alternata di regole da valutare
	class alt_rule: public abs_rule {
			std::vector<std::shared_ptr<impl_rule> > rl;
		public:
			alt_rule(rule a, rule b);

			virtual bool parse(parser_context &pc);
	};

	alt_rule::alt_rule(rule a, rule b) {
		rl.push_back(a.get_pimpl());
		rl.push_back(b.get_pimpl());
	}

	bool alt_rule::parse(parser_context &pc) {
		INFO("alt_rule::parse() | ");
		for (auto &x : rl)
			if (x->parse(pc)) {
				INFO_LINE(" ** ok");
				return true;
			}
		pc.setError( { ERR_PARSE_ALT, "None of the alternatives parsed correctly" });
		INFO_LINE(" ** FALSE");
		return false;
	}

	rule operator|(rule a, rule b) {
		auto s = std::make_shared<impl_rule>(new alt_rule(a, b));
		return rule(s);
	}

	/** Null rule */

	class null_rule: public abs_rule {
		public:
			null_rule() {
			}
			virtual bool parse(parser_context &pc);
	};

	bool null_rule::parse(parser_context &pc) {
		return true;
	}

	/* ------------------------------------------- */

	//	  Classe che definisce la ripetizione della regola zero o piu volte
	class rep_rule: public abs_rule {
			std::shared_ptr<impl_rule> rl;
		public:
			rep_rule(rule a);

			virtual bool parse(parser_context &pc);
	};

	rep_rule::rep_rule(rule a) :
			rl(a.get_pimpl()) {
	}

	bool rep_rule::parse(parser_context &pc) {
		INFO("rep_rule::parse() | ");
		while (rl->parse(pc)) {
			INFO("*");
		}
		INFO(" end ");
		return true;
	}

	rule operator*(rule a) {
		auto s = std::make_shared<impl_rule>(new rep_rule(a));
		return rule(s);
	}

	class extr_rule: public abs_rule {
			std::string open_sym;
			std::string close_sym;
			bool nested;
			bool line;
		public:
			extr_rule(const std::string &op, const std::string &cl) :
					open_sym(op), close_sym(cl), nested(true), line(false) {
			}
			extr_rule(const std::string &op_cl, bool l = false) :
					open_sym(op_cl), close_sym(op_cl), nested(false), line(l) {
			}
			bool parse(parser_context &pc) {
				INFO("extr_rule::parse()");
				token open_tk = { LEX_CHAR, padding(open_sym) };
				if (pc.tryToken(open_tk).first == LEX_CHAR) {
					if (line) {
						pc.pushToken(pc.extractLine());
						INFO_LINE(" ** ok");
						//pc.next_token();
						return true;
					}
					std::string o = "";
					if (nested)
						o = open_sym;
					pc.pushToken(pc.extract(o, close_sym));
					INFO_LINE(" ** ok");
					//pc.next_token();
					return true;
				} else {
					INFO_LINE(" ** FALSE");
					return false;
				}
			}
	};

	class keyword_rule: public abs_rule {
			std::string kw;
			term_rule rl;
		public:
			keyword_rule(const std::string &key, bool collect) :
					kw(key), rl(tk_ident, collect) {
			}

			virtual bool parse(parser_context &pc);
	};

	bool keyword_rule::parse(parser_context &pc) {
		pc.save();
		bool flag = rl.parse(pc);
		if (flag && pc.getLastToken().second == kw) {
			pc.discardSaved();
			return true;
		} else {
			pc.restore();
			return false;
		}
	}

	rule extractRule(const std::string &op, const std::string &cl) {
		auto s = std::make_shared<impl_rule>(new extr_rule(op, cl));
		return rule(s);
	}

	rule extract_rule(const std::string &opcl) {
		auto s = std::make_shared<impl_rule>(new extr_rule(opcl));
		return rule(s);
	}

	rule extractLineRule(const std::string &opcl) {
		auto s = std::make_shared<impl_rule>(new extr_rule(opcl, true));
		return rule(s);
	}

	rule keyword(const std::string &key, bool collect) {
		auto s = std::make_shared<impl_rule>(new keyword_rule(key, collect));
		return rule(s);
	}

	class strict_seq_rule: public abs_rule {
			std::vector<std::shared_ptr<impl_rule> > rl;
		public:
			strict_seq_rule(rule a, rule b);

			virtual bool parse(parser_context &pc);
	};

	strict_seq_rule::strict_seq_rule(rule a, rule b) {
		rl.push_back(a.get_pimpl());
		rl.push_back(b.get_pimpl());
	}

	bool strict_seq_rule::parse(parser_context &pc) {
		INFO("strict_seq_rule::parse()");
		int i = 0;
		for (auto &x : rl) {
			if (!x->parse(pc)) {
				pc.setError( { ERR_PARSE_SEQ, "Wrong element in sequence" });
				INFO_LINE(" ** FALSE ");
				if (i == 0)
					return false;
				else
					throw exception::parser_exception("Error in strict sequence (after first)");
			}
			i++;
		}
		INFO_LINE(" ** ok ");
		return true;
	}

	rule operator>(rule a, rule b) {
		auto s = std::make_shared<impl_rule>(new strict_seq_rule(a, b));
		return rule(s);
	}

	rule null() {
		auto s = std::make_shared<impl_rule>(new null_rule);
		return rule(s);
	}

	rule operator-(rule a) {
		auto s = std::make_shared<impl_rule>(new alt_rule(a, null()));
		return rule(s);
	}

	recursive_rule::recursive_rule() :
			rule(), rl(null()) {
	}

	void recursive_rule::bind(rule rl) {
		pimpl->abs_impl = rl.pimpl->abs_impl;
		INFO_LINE("Regola Bindata: "<<rl.get_pimpl());
	}

	bool recursive_rule::parse(parser_context &pc) {
		return rule::parse(pc);
	}

}
