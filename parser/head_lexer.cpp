//#define _INFOMACRO_ 1
#include "head_lexer.hpp"
#include <boost/regex.hpp>
#include "../utils/macro.hpp"

using namespace std;

namespace chaos_parser {

	head_lexer::head_lexer(const std::vector<token> &keys) :
			array(keys) {
	}

	void head_lexer::add_token(const token_id &name, const string &expr) {
		array.push_back(token(name, expr));
	}

	token_val head_lexer::get_token() {
		static boost::match_results<std::string::iterator> what;

		skip_spaces();

		while (start == curr_line.end() or *start == 0) {
			if (not next_line())
				return {LEX_ERROR, "EOF"};
			skip_spaces();
		}

		INFO_LINE("get_token(): start is at \"" << *start << "\"");
		INFO_LINE(curr_line);
	//	INFO_LINE(std::setw(ncol) << "^");

		// try to identify which token
		for (auto x:array) {
			boost::regex expr(x.get_expr());
			auto flag = boost::regex_search(start, curr_line.end(), what, expr, boost::match_continuous);
			if (flag) {
				string res;
				copy(start, what[0].second, back_inserter(res));
				ncol += distance(start, what[0].second);
				start = what[0].second;
				// INFO_LINE("get_token(): start now pointing at \"" << *start << "\"");
				// INFO_LINE("token: " << res);
				return {x.get_name(), res};
				break;
			}
		}
		INFO_LINE("token not found");
		return {LEX_ERROR, "Unknown token"};
	}

}
