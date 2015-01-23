//#define __INFOMACRO__
#include <iomanip>
#include <boost/regex.hpp>
#include "parser_exception.hpp"
#include "lexer.hpp"
#include "../utils/macro.hpp"

using namespace std;

namespace chaos_parser {


	lexer::lexer() {}

	void lexer::set_stream(istream &in) {
		// curr_line = "";
		// start = begin(curr_line);
		all_lines.clear();
		while (!saved_ctx.empty())
			saved_ctx.pop();

		p_input = &in;
		nline = 0;
		ncol = 0;

		next_line();
	}

	void lexer::set_comment(const std::string &b, const std::string &e, const std::string &sl) {
		comment_begin = b;
		comment_end = e;
		comment_single_line = sl;
	}

	bool lexer::next_line() {
		INFO_LINE("Next line");
		if (nline == all_lines.size()) {
			if (p_input->eof()) {
				INFO_LINE("No more lines to process");
				return false;
			}
			getline(*p_input, curr_line);
			all_lines.push_back(curr_line);
		} else if (nline > all_lines.size()) {
			throw exception::parser_exception("Lexer: exceeding all_lines array lenght!");
		} else {
			INFO_LINE("Going ahead again!");
			INFO_LINE(curr_line);
			INFO_LINE(all_lines[nline]);
		}

		nline++;
		curr_line = all_lines[nline - 1];
		ncol = 0;
		start = curr_line.begin();
		return true;
	}

	void lexer::save() {
		ctx c;
		c.nl = nline;
		c.nc = ncol;
		if (start == curr_line.end())
			c.dist = -1;
		else
			c.dist = std::distance(curr_line.begin(), start);

		saved_ctx.push(c);
	}

	void lexer::restore() {
		ctx c = saved_ctx.top();
		saved_ctx.pop();
		nline = c.nl;
		ncol = c.nc;
		curr_line = all_lines[nline - 1];
		if (c.dist < 0)
			start = curr_line.end();
		else
			start = curr_line.begin() + c.dist;
	}

	void lexer::discard_saved() {
		saved_ctx.pop();
	}

	void lexer::skip_spaces() {
		while (start != curr_line.end()) {
			int d = std::distance(start, curr_line.end());
			int m = std::min((int) comment_begin.size(), d);
			int n = std::min((int) comment_single_line.size(), d);
			std::string p(start, start + m);
			std::string q(start, start + n);

			if (*start == ' ' or *start == '\n') {
				++start;
				++ncol;
			} else if (*start == '\t') {
				++start;
				ncol += 8;
			} else if (m != 0 and comment_begin == p) {
				start += m;
				extract(comment_begin, comment_end);
			} else if (n != 0 and comment_single_line == q)
				extract_line();
			else
				break;
		}
	}

	token_val lexer::try_token(const token &x) {

		static boost::match_results<std::string::iterator> what;
		skip_spaces();

		while (start == curr_line.end() or *start == 0) {
			if (not next_line())
				return {LEX_ERROR, "EOF"};
			skip_spaces();
		}

		INFO_LINE("try_token(): start is at <" << *start << ">");
		INFO_LINE(curr_line);
		INFO_LINE(std::setw(ncol) << "^");

		//Funzioni della libreria boost per matchare la stringa con le espressioni regolari definite nel lexer per ogni token
		boost::regex expr(x.get_expr());
		//Cerca una sequenza che corrisponda con espressione regolare assegnatta
		auto flag = boost::regex_search(start, curr_line.end(), what, expr, boost::match_continuous);

		INFO_LINE("Regex_search completed");
		if (flag) {
			string res;
			copy(start, what[0].second, back_inserter(res));
			ncol += distance(start, what[0].second);
			start = what[0].second;
			return token_val(x.get_name(), res);
		}
		INFO_LINE("Token does not match");
		return {LEX_ERROR, "Token does not match"};
	}

	std::string lexer::extract_line() {
		std::string s(start, curr_line.end());
		next_line();
		return s;
	}

	std::string lexer::extract(const std::string &sym_begin, const std::string &sym_end) {
		std::string result;

		while (true) {
			while (start == curr_line.end()) {
				if (not next_line())
					throw exception::parser_exception("END OF INPUT WHILE EXTRACTING");
				result += "\n";
			}
			std::string s1(start, start + sym_begin.size());
			std::string s2(start, start + sym_end.size());
			if (sym_begin != "" and s1 == sym_begin) {
				result += s1;
				start += sym_begin.size();
				result += extract(sym_begin, sym_end) + sym_end;
			} else if (s2 == sym_end) {
				start += sym_end.size();
				return result;
			} else {
				result += *start;
				start++;
			}
		}
	}
}
