/*
 * head_lexer.hpp
 *
 *  Created on: 08/gen/2015
 *      Author: cicciox
 */

#ifndef HEAD_LEXER_HPP_
#define HEAD_LEXER_HPP_

#include "lexer.hpp"

namespace chaos_parser {

	//	Classe usate come interfaccia dell'Analizzatore Lessica senza dover istanziare il lexer e il parser

	class head_lexer: public lexer {
			std::vector<token> array;
		public:

			//Costruttore del Lexer
			head_lexer(const std::vector<token> &keys);
			//Aggiunge un token al Lexer
			void add_token(const token_id &name, const std::string &expr);
			//Restituisce il token sotto forma di coppia definita in precedenza come typedef std::pair<token_id, std::string> token_val
			token_val get_token();
	};

}

#endif /* HEAD_LEXER_HPP_ */
