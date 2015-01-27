#ifndef _PARSER_EXCEPTION_HPP_
#define _PARSER_EXCEPTION_HPP_

#include <string>
#include <iostream>
#include <vector>
#include <stack>

#define LEX_EMPTY  0
#define LEX_ERROR -1

namespace chaos_parser{

	namespace exception{
		//Classe usata per gestire le eccezioni lanciate dal parser
	    class parser_exception : public std::exception {
		std::string msg;
	    public:
		parser_exception();
		parser_exception(const std::string &err_msg);
		std::string what();
		~parser_exception() throw();
	    };
	}
}

#endif /* PARSER_EXCEPTION_HPP_ */
