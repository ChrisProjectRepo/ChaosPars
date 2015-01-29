#include "rules_implementation.hpp"

#include "functional"

using namespace std;
using namespace std::placeholders;

namespace chaos_parser {

	namespace rules {
	//Rule  Terminali
//			rule keyword_select = keyword("select");
//			rule keyword_from = keyword("from");
//			rule keyword_where = keyword("where");
//			rule select_key = rule(tk_ident);
//			rule tab_key = rule(tk_ident);
//			rule opt_node = keyword("and") | keyword("or");
//			rule condition = rule('>', true) | rule('<', true) | rule('=', true);
//			rule end_query = rule(';', true);
//			rule point_key = rule('.');
//			rule comma_key = rule(',', true);
//
//			//Inteso come nodo "." con all'interno come figli la tabella e la selezione
//			rule select_operation = tab_key >>point_key>>select_key;
//
//			//Rule Select
//
//			rule select_prop = comma_key >> select_operation;
//			rule select = keyword_select >> select_operation >> *select_prop;
//
//			//Rule From
//
//			rule from_prop = comma_key >> tab_key;
//			rule from = keyword_from >> tab_key >> *from_prop;
//
//			//Rule Where
//
//			rule where_node = select_operation >> condition >> select_operation;
//
//			rule condition_node=where_node>>opt_node>>*condition_node|where_node;
//
//			rule condition_init=where_node|condition_node;
//
//			rule keyword_and = keyword("and");
//			rule keyword_or = keyword("or");
//			rule lparen = rule('(');
//			rule rparen = rule(')');
//
//			recursive_rule x;
//			rule z = where_node | (lparen >> x >> rparen);
//			rule y = z >> *(keyword_and >> z);
//			rule f=y >> *(keyword_or >> y);
//
//
//			// bind = set_pimpl
//
//			rule where = keyword_where >>condition_init;//x
//			rule where=keyword_where>>x;
//
//			//Rule Query
//			rule query = select >> from >> where >> end_query;

	}

	bool parseString(tree_struct::builder &b, parser_context &pc) {


///////////////////// RULES /////////////////
		rule keyword_select = keyword("select");
		rule keyword_from = keyword("from");
		rule keyword_where = keyword("where");
		rule select_key = rule(tk_ident);
		rule tab_key = rule(tk_ident);
		rule opt_node = keyword("and") | keyword("or");
		rule condition = rule('>', true) | rule('<', true) | rule('=', true);
//		rule end_query = rule(';', true);
		rule point_key = rule('.');
		rule comma_key = rule(',', true);

		//Inteso come nodo "." con all'interno come figli la tabella e la selezione
		rule select_operation = tab_key >> point_key >> select_key;

		//Rule Select

		rule select_prop = comma_key >> select_operation;
		rule select = keyword_select >> select_operation >> *select_prop;

		//Rule From

		rule from_prop = comma_key >> tab_key;
		rule from = keyword_from >> tab_key >> *from_prop;

		//Rule Where

		rule where_node = select_operation >> condition >> select_operation;

//		rule condition_node = where_node >> opt_node >> *condition_node | where_node;
//		rule condition_init = where_node | condition_node;
//		rule lparen = rule('(');
//		rule rparen = rule(')');
//		rule paren_node = (lparen >> where_node>>lparen);

		recursive_rule recursive_where;
		recursive_where.bind(where_node >> opt_node >> *recursive_where);
//		rule condition_init= recursive_where;
		rule where = keyword_where >> recursive_where;

		//Rule Query
		rule query = select >> from >> where;

//		recursive_rule S;
//		rule z = (lparen >> S) | rparen;
//		S.bind((rule('a')>>rule('+')>>*S)|rule('b')|z);
//		rule S=(rule('a')>>rule('+')>>S)|rule('b');

///////////////////// RULES /////////////////

		query[std::bind(&tree_struct::builder::makeNodeRule, &b, _1, "query", vector<string> { "select", "from", "where"})];

		select[std::bind(&tree_struct::builder::makeNodeRule, &b, _1, "select", std::vector<std::string> { "keyword_select", "select_operation", "select_prop" })];
		select_prop[bind(&tree_struct::builder::makeNodeRule, &b, _1, "select_prop", vector<string> { "comma_key", "select_operation" })];

		from[bind(&tree_struct::builder::makeNodeRule, &b, _1, "from", vector<string> { "keyword_from", "tab_key", "from_prop" })];
		from_prop[bind(&tree_struct::builder::makeNodeRule, &b, _1, "from_prop", vector<string> { "comma_key", "tab_key" })];

		where[bind(&tree_struct::builder::makeNodeRule, &b, _1, "where", vector<string> { "keyword_where", "recursive_where" })];
//		condition_init[bind(&tree_struct::builder::makeNodeRule, &b, _1, "condition_init", vector<string> { "where_node", "condition_node"})];
		//Gli faccio prendere anche opt ma quando faccio la visita di questa regola e trova opt gli dico che invece di assegnarlo come figlio lo fa diventare come valore del padre
//		condition_node[bind(&tree_struct::builder::makeNodeRule, &b, _1, "condition_node", vector<string> { "where_node","opt_node", "condition_node","where_node"})];
		recursive_where[bind(&tree_struct::builder::makeNodeRule, &b, _1, "recursive_where", vector<string> { "where_node","opt_node", "recursive_where"})];
		where_node[bind(&tree_struct::builder::makeNodeRule, &b, _1, "where_node", vector<string> { "select_operation", "condition", "select_operation" })];


		select_operation[bind(&tree_struct::builder::makeNodeRule, &b, _1, "select_operation", vector<string> { "tab_key", "select_key" })];

		condition[bind(&tree_struct::builder::makeLeaf, &b, _1, "condition")];
		//rules::point_key[bind(&tree_struct::builder::make_leaf, b, _1, "point_key")];
		comma_key[bind(&tree_struct::builder::makeLeaf, &b, _1, "comma_key")];
		tab_key[bind(&tree_struct::builder::makeLeaf,&b, _1, "tab_key")];
		select_key[bind(&tree_struct::builder::makeLeaf, &b, _1, "select_key")];
		opt_node[bind(&tree_struct::builder::makeLeaf, &b, _1, "opt_node")];
//		end_query[bind(&tree_struct::builder::makeLeaf, &b, _1, "end_query")];
		keyword_select[bind(&tree_struct::builder::makeLeaf, &b, _1, "keyword_select")];
		keyword_from[bind(&tree_struct::builder::makeLeaf,&b, _1, "keyword_from")];
		keyword_where[bind(&tree_struct::builder::makeLeaf, &b, _1, "keyword_where")];

		return query.parse(pc);
	}

}

