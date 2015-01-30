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
		rule keyword_and = keyword("and");
		rule keyword_or = keyword("or");
		rule select_key = rule(tk_ident);
		rule tab_key = rule(tk_ident);
		rule condition = rule('>', true) | rule('<', true) | rule('=', true);
//		rule end_query = rule(';', true);
		rule point_key = rule('.');
		rule comma_key = rule(',');

		//Inteso come nodo "." con all'interno come figli la tabella e la selezione
		rule select_element = tab_key >> point_key >> select_key;

		//Rule Select

		rule select_prop = comma_key >> select_element;
		rule select = keyword_select >> select_element >> *select_prop;

		//Rule From

		rule from_prop = comma_key >> rule(tk_ident);
		rule from = keyword_from >> tab_key >> *from_prop;

		//Rule Where
		rule where_node = select_element >> condition >> tab_key >> point_key >> select_key;

//		rule condition_node = where_node >> opt_node >> *condition_node | where_node;
//		rule condition_init = where_node | condition_node;
//		rule lparen = rule('(');
//		rule rparen = rule(')');
//		rule paren_node = (lparen >> where_node>>lparen);

		recursive_rule rwhere;
		rule where = keyword_where >> rwhere;
		rule multi_where = (where_node >> (keyword_and|keyword_or) >> rwhere);
		rwhere.bind(multi_where | where_node);

		//Rule Query
		rule query = select >> from >> where;

/////////////////////// RULES /////////////////

/////////////////////// BIND /////////////////

		query[std::bind(&tree_struct::builder::makeNodeRule, &b, _1, "query", vector<string> { "select", "from", "where" })];

		select[std::bind(&tree_struct::builder::makeNodeRule, &b, _1, "select", std::vector<std::string> { "keyword_select", "select_element","select_prop" })];
		select_prop[bind(&tree_struct::builder::makeNodeRule, &b, _1, "select_prop", vector<string> { "select_element" })];

		from[bind(&tree_struct::builder::makeNodeRule, &b, _1, "from", vector<string> { "keyword_from", "tab_key", "from_prop" })];
		from_prop[bind(&tree_struct::builder::makeNodeRule, &b, _1, "from_prop", vector<string> { "tab_key" })];

		where[bind(&tree_struct::builder::makeNodeRule, &b, _1, "where", vector<string> { "keyword_where", "rwhere" })];
		multi_where[bind(&tree_struct::builder::makeNodeRule, &b, _1, "multi_where", vector<string> { "where_node", "keyword_and","keyword_or","rwhere" })];
	//	single_where[bind(&tree_struct::builder::makeRecursiveNodeRule, &b, _1, "single_where", "where_node")];
		rwhere[bind(&tree_struct::builder::makeRecursiveNodeRule, &b, _1, "rwhere", vector<string> { "multi_where", "where_node"})];
		where_node[bind(&tree_struct::builder::makeNodeRule, &b, _1, "where_node", vector<string> { "select_element", "condition", "tab_key", "select_key" })];

		select_element[bind(&tree_struct::builder::makeNodeRule, &b, _1, "select_element", vector<string> { "tab_key", "select_key" })];

		condition[bind(&tree_struct::builder::makeLeaf, &b, _1, "condition")];
		//rules::point_key[bind(&tree_struct::builder::make_leaf, b, _1, "point_key")];
		//comma_key[bind(&tree_struct::builder::makeLeaf, &b, _1, "comma_key")];
		tab_key[bind(&tree_struct::builder::makeLeaf, &b, _1, "tab_key")];
		select_key[bind(&tree_struct::builder::makeLeaf, &b, _1, "select_key")];
		//opt_node[bind(&tree_struct::builder::makeLeaf, &b, _1, "opt_node")];
		//end_query[bind(&tree_struct::builder::makeLeaf, &b, _1, "end_query")];
		keyword_and[bind(&tree_struct::builder::makeLeaf, &b, _1, "keyword_and")];
		keyword_or[bind(&tree_struct::builder::makeLeaf, &b, _1, "keyword_or")];
		keyword_select[bind(&tree_struct::builder::makeLeaf, &b, _1, "keyword_select")];
		keyword_from[bind(&tree_struct::builder::makeLeaf, &b, _1, "keyword_from")];
		keyword_where[bind(&tree_struct::builder::makeLeaf, &b, _1, "keyword_where")];

		/////////////////////// BIND /////////////////

//		recursive_rule x;
//		rule a=rule('a');
//		rule v=rule('v');
//		rule plus=rule('+');
//		rule S=a>>x;
//		x.bind((v>>plus>>x)|v);

		return query.parse(pc);
	}

}

