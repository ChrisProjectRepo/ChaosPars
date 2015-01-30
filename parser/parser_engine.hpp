#ifndef PARSER_PARSER_ENGINE_HPP_
#define PARSER_PARSER_ENGINE_HPP_

#include "lexer.hpp"
#include <memory>
#include <functional>
#include <exception>

#define ERR_PARSE_SEQ   -100
#define ERR_PARSE_ALT   -101

namespace chaos_parser {
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

			void setStream(std::istream &in);
			void setComment(const std::string &comment_begin, const std::string &comment_end, const std::string &comment_single_line);

			token_val tryToken(const token &tk);
			std::string extract(const std::string &op, const std::string &cl);
			std::string extractLine();

			void save();
			void restore();
			void discardSaved();

			token_val getLastToken();

			void setError(const token_val &err_msg);
			int getErrorCode() const {
				return error_token_msg.first;
			}

			std::string getErrorString() const {
				return error_token_msg.second;
			}
			std::string getFormattedErrMsg();
			std::pair<int, int> get_error_pos() const {
				return lex.get_pos();
			}

			void pushToken(token_val tk);
			void pushToken(const std::string &s);
			std::vector<token_val> collectTokens();
	};

//  Implementazione delle regole
	struct impl_rule;

// The action function which is passed the parser context
	typedef std::function<void(parser_context &)> action_t;

//Classe che definisce la regola con tutti i metodi per interagire tra loro
	class rule {
			//Puntatore all'implementazione della regola
		public:
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

	class recursive_rule: public rule {
		public:
			recursive_rule();
			void bind(rule);
			bool parse(parser_context &);

	};

//Sequenza di regole
	rule operator>>(rule a, rule b);

//Sequenza di regole
	rule operator>(rule a, rule b);

//Alternazione di regole
	rule operator|(rule a, rule b);

//Ripetizione di regole
	rule operator*(rule a);

	//Regola presente una volta o nessuna
	rule operator-(rule a);

//Regola per matchare una keyword
	rule keyword(const std::string &key, bool collect = true);

//Metodi che permettono estrazione di una parte del testo
	rule extractRule(const std::string &op, const std::string &cl);
	rule extractRule(const std::string &opcl);
	rule extractLineRule(const std::string &opcl);

//Non necessario, usato per definire regole vuote
	rule null();

}

#endif /* PARSER_PARSER_ENGINE_HPP_ */
