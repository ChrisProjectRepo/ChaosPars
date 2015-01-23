#ifndef __LEXER_HPP__
#define __LEXER_HPP__

#include <string>
#include <iostream>
#include <vector>
#include <stack>

#define LEX_EMPTY  0
#define LEX_ERROR -1

typedef int token_id;

namespace chaos_parser {
    struct token {

    //Unused Constructor
	token(const std::pair<token_id, std::string> &p) : name(p.first), expr(p.second) {}

	token(const token_id &n, const std::string &e) :name(n), expr(e) {}


	token_id    get_name() const { return name; } 
	std::string get_expr() const { return expr; } 
    private:
	token_id name;
	//Espressione regolare che definisce il token
	std::string expr;
    };

    typedef std::pair<token_id, std::string> token_val;

    // Id globale per identificare i token
    const int LEX_LIB_BASE    = 1000;
    const int LEX_INT         = (LEX_LIB_BASE + 1);
    const int LEX_IDENTIFIER  = (LEX_LIB_BASE + 2);
    const int LEX_EXTRACTED_STRING = (LEX_LIB_BASE + 3);
    const int LEX_CHAR        = (LEX_LIB_BASE + 4);

    //Definisco in toke piu usati
    //Rappresento Interi, e prendo tutti i digit a partire da inizio stringa una o più volte e prendo intero solo con spazio vuoto dopo,perchè seno lo vedo come stringa
    const token tk_int(LEX_INT, "^\\d+\\b"); 
    //Rappresento le Stringhe che non iniziano per un intero e un non-word.E prendo invece tutto il resto della stringa con w*
    const token tk_ident(LEX_IDENTIFIER, "^[^\\d\\W]\\w*");

    //Analizzatore Lessicale
    class lexer {
    protected:
	struct ctx {
	    int dist;
	    unsigned nl, nc;
	};

	std::string::iterator start;
	std::istream *p_input;
	std::string curr_line;
	unsigned nline, ncol;

	std::vector<std::string> all_lines;
	std::stack<ctx> saved_ctx; 

	std::string comment_begin; 
	std::string comment_end;
	std::string comment_single_line;

	bool next_line();
	void skip_spaces();
    public:
    
	lexer();
    
	/// Salva il contenuto del lexer per recuperarlo in seguito
	void save(); 
	/// Recupera contenuto lexer precedentemente salvato
	void restore();
	/// Eliminia ultimo salvataggio
	void discard_saved();

	/// Configures the lexer to skip all characters between strings b
	/// and e, and all characters from string sl until the end of the
	/// current line. The intended use is to skip comments.

//	Configura il lexer per saltare le stringhe all'interno di questo intervallo
	//b inizio sequenza, e fine sequenza,sl salta caratteri in una stringa
	void set_comment(const std::string &b, const std::string &e, const std::string &sl);

	/// imposto lo stream di input per i dati per il lexer
	void set_stream(std::istream &in);

	//controlla se il token è stato trovato e loro ritorna , altrimenti da errore
	token_val try_token(const token &x);

	//ritorna posizione attuale della scansione (line num, column num)
	std::pair<int, int> get_pos() const { return {nline, ncol}; }

	// ritorna la linea che sta attualmente processando
	std::string get_currline() const { return curr_line; }

	std::string extract(const std::string &sym_begin, const std::string &sym_end);
	std::string extract_line();
    };

}

#endif
