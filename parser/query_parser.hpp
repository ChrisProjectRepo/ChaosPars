#ifndef __QUERY_PARSER_HPP__
#define __QUERY_PARSER_HPP__

#include "lexer.hpp"
#include <memory>
#include <functional>
#include <exception>

#define ERR_PARSE_SEQ   -100
#define ERR_PARSE_ALT   -101

namespace chaos_parser {

	//  Implementazione delle regole
			struct impl_rule;

	//Definizione del parser
	//Di base vengono usati i token definiti globalmente nel namespace
	class parser_context {
		public:
			//Analizzatore lessicale definito in precedenza
			lexer lex;
			//Vettore che mantiene la lista di token utilizzati dal parser
			std::vector<token_val> collected;
			std::stack<unsigned int> ncoll;
			token_val error_token_msg;
		public:
			parser_context();

			void set_stream(std::istream &in);
			void set_comment(const std::string &comment_begin, const std::string &comment_end, const std::string &comment_single_line);

			token_val try_token(const token &tk);
			std::string extract(const std::string &op, const std::string &cl);
			std::string extract_line();

			void save();
			void restore();
			void discard_saved();

			token_val get_last_token();

			void set_error(const token_val &err_msg);
			int get_error_code() const {
				return error_token_msg.first;
			}

			std::string get_error_string() const {
				return error_token_msg.second;
			}
			std::string get_formatted_err_msg();
			std::pair<int, int> get_error_pos() const {
				return lex.get_pos();
			}

			void push_token(token_val tk);
			void push_token(const std::string &s);
			std::vector<token_val> collect_tokens();
	};



	// The action function which is passed the parser context
		typedef std::function<void(parser_context &)> action_t;

//Classe che definisce la regola con tutti i metodi per interagire tra loro
	class rule {
			//Puntatore all'implementazione della regola
			std::shared_ptr<impl_rule> pimpl;
		public:
			// Costruttore vuoto
			rule();
			//Costruttore base
			rule(const rule &r);

			//Regola che fa match di un carattere,di default il carattere trovato non viene estrapolato
			explicit rule(char c, bool collect = false);

			//Regola che fa match della stringa,di default la stringa trovata non viene estrapolata
			explicit rule(const std::string &s, bool collect = false);

			//Regola che fa match di un token,di default il token trovato viene estrapolato
			explicit rule(const token &tk);

			// Ri-Definizione Metodo di assegnazione per le regole
			rule &operator=(const rule &);

			// Parser di regola
			bool parse(parser_context &pc);
			// Imposto una funziona da eseguira,che verrà implementata , per la regola
			rule& operator[](action_t af);

			explicit rule(std::shared_ptr<impl_rule> ir);
			std::shared_ptr<impl_rule> get_pimpl() {
				return pimpl;
			}
	};

//Sequenza di regole
	rule operator>>(rule a, rule b);

//Sequenza di regole
	rule operator>(rule a, rule b);

//Alternazione di regole
	rule operator|(rule a, rule b);

//Ripetizione di regole
	rule operator*(rule a);

	rule operator-(rule a);

//Regola per matchare una keyword
	rule keyword(const std::string &key, bool collect = true);

//Metodi che permettono estrazione di una parte del testo
	rule extract_rule(const std::string &op, const std::string &cl);
	rule extract_rule(const std::string &opcl);
	rule extract_line_rule(const std::string &opcl);

//Non necessario, usato per definire regole vuote
	rule null();
}

#endif
