#include "parser_exception.hpp"

using namespace std;

namespace chaos_parser {
	namespace exception{
			parser_exception::parser_exception() {
			}
			parser_exception::parser_exception(const string &err_msg) :
					msg(err_msg) {
			}

			parser_exception::~parser_exception() throw () {
			}

			string parser_exception::what() {
				return msg;
			}
	}
}

