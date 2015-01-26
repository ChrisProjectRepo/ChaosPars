//#define _INFOMACRO_ 1
#include "query_parser.hpp"
#include "parser_exception.hpp"
#include "../utils/macro.hpp"
#include <sstream>

namespace chaos_parser {

	parser_context::parser_context() :
			lex { } {
	}

	void parser_context::set_stream(std::istream &in) {
		lex.set_stream(in);
		collected.clear();
		while (!ncoll.empty())
			ncoll.pop();
	}

	void parser_context::set_comment(const std::string &comment_begin, const std::string &comment_end, const std::string &comment_single_line) {
		lex.set_comment(comment_begin, comment_end, comment_single_line);
	}

	token_val parser_context::try_token(const token &tk) {
		return lex.try_token(tk);
	}

	std::string parser_context::extract(const std::string &op, const std::string &cl) {
		return lex.extract(op, cl);
	}

	std::string parser_context::extract_line() {
		return lex.extract_line();
	}

	void parser_context::push_token(token_val tk) {
		collected.push_back(tk);
	}

	void parser_context::push_token(const std::string &s) {
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

	void parser_context::discard_saved() {
		lex.discard_saved();
		ncoll.pop();
	}

	token_val parser_context::get_last_token() {
		return collected[collected.size() - 1];
	}

	std::vector<token_val> parser_context::collect_tokens() {
		auto c = collected;
		collected.clear();
		return c;
	}

	void parser_context::set_error(const token_val &err_msg) {
		error_token_msg = err_msg;
	}

	std::string parser_context::get_formatted_err_msg() {
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





	/* ----------------------------------------------- */



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



	rule operator>>(rule a, rule b) {
		auto s = std::make_shared<impl_rule>(new seq_rule(a, b));
		return rule(s);
	}



	rule operator|(rule a, rule b) {
		auto s = std::make_shared<impl_rule>(new alt_rule(a, b));
		return rule(s);
	}


	/* ------------------------------------------- */



	rule operator*(rule a) {
		auto s = std::make_shared<impl_rule>(new rep_rule(a));
		return rule(s);
	}

	rule extract_rule(const std::string &op, const std::string &cl) {
		auto s = std::make_shared<impl_rule>(new extr_rule(op, cl));
		return rule(s);
	}

	rule extract_rule(const std::string &opcl) {
		auto s = std::make_shared<impl_rule>(new extr_rule(opcl));
		return rule(s);
	}

	rule extract_line_rule(const std::string &opcl) {
		auto s = std::make_shared<impl_rule>(new extr_rule(opcl, true));
		return rule(s);
	}

	rule keyword(const std::string &key, bool collect) {
		auto s = std::make_shared<impl_rule>(new keyword_rule(key, collect));
		return rule(s);
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
}
