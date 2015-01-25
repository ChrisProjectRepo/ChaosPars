#include "rules.hpp"
#include "functional"


using namespace std;
using namespace std::placeholders;


namespace chaos_parser{

	namespace rules {
			//Rule  Terminali
			rule keyword_select = keyword("select");
			rule keyword_from = keyword("from");
			rule keyword_where = keyword("where");
			rule select_key = rule(tk_ident);
			rule tab_key = rule(tk_ident);
			rule opt_node = keyword("and") | keyword("or");
			rule condition = rule('>', true) | rule('<', true) | rule('=', true);
			rule end_query = rule(';', true);
			rule point_key = rule('.');
			rule comma_key = rule(',', true);

			//Inteso come nodo "." con all'interno come figli la tabella e la selezione
			rule select_operation = tab_key >>point_key>>select_key;

			//Rule Select

			rule select_prop = comma_key >> select_operation;
			rule select = keyword_select >> select_operation >> *select_prop;

			//Rule From

			rule from_prop = comma_key >> tab_key;
			rule from = keyword_from >> tab_key >> *from_prop;

			//Rule Where

			rule where_node = select_operation >> condition >> select_operation;

			rule condition_node=where_node>>opt_node>>*condition_node|where_node;

			rule condition_init=where_node|condition_node;

			rule where = keyword_where >>condition_init;


			//Rule Query
			rule query = select >> from >> where >> end_query;
			}

bool parse_string(tree_struct::builder* b,parser_context* pc){

		rules::query[std::bind(&tree_struct::builder::make_node_rule, b, _1, "query", vector<string> { "select", "from", "where", "end_query" })];

		rules::select[std::bind(&tree_struct::builder::make_node_rule, b, _1, "select", std::vector<std::string> { "keyword_select", "select_operation", "select_prop" })];
		rules::select_prop[bind(&tree_struct::builder::make_node_rule, b, _1, "select_prop", vector<string> { "comma_key", "select_operation" })];

		rules::from[bind(&tree_struct::builder::make_node_rule, b, _1, "from", vector<string> { "keyword_from", "tab_key", "from_prop" })];
		rules::from_prop[bind(&tree_struct::builder::make_node_rule, b, _1, "from_prop", vector<string> { "comma_key", "tab_key" })];

		rules::where[bind(&tree_struct::builder::make_node_rule, b, _1, "where", vector<string> { "keyword_where", "condition_init" })];
		rules::condition_init[bind(&tree_struct::builder::make_node_rule, b, _1, "condition_init", vector<string> { "where_node", "condition_node"})];
		//Gli faccio prendere anche opt ma quando faccio la visita di questa regola e trova opt gli dico che invece di assegnarlo come figlio lo fa diventare come valore del padre
		rules::condition_node[bind(&tree_struct::builder::make_node_rule, b, _1, "condition_node", vector<string> { "where_node","opt_node", "condition_node","where_node"})];
		rules::where_node[bind(&tree_struct::builder::make_node_rule, b, _1, "where_node", vector<string> { "select_operation", "condition", "select_operation" })];


		rules::select_operation[bind(&tree_struct::builder::make_node_rule, b, _1, "select_operation", vector<string> { "tab_key", "select_key" })];


		rules::condition[bind(&tree_struct::builder::make_leaf, b, _1, "condition")];
		//rules::point_key[bind(&tree_struct::builder::make_leaf, b, _1, "point_key")];
		rules::comma_key[bind(&tree_struct::builder::make_leaf, b, _1, "comma_key")];
		rules::tab_key[bind(&tree_struct::builder::make_leaf,b, _1, "tab_key")];
		rules::select_key[bind(&tree_struct::builder::make_leaf, b, _1, "select_key")];
		rules::opt_node[bind(&tree_struct::builder::make_leaf, b, _1, "opt_node")];
		rules::end_query[bind(&tree_struct::builder::make_leaf, b, _1, "end_query")];
		rules::keyword_select[bind(&tree_struct::builder::make_leaf, b, _1, "keyword_select")];
		rules::keyword_from[bind(&tree_struct::builder::make_leaf,b, _1, "keyword_from")];
		rules::keyword_where[bind(&tree_struct::builder::make_leaf, b, _1, "keyword_where")];

		return rules::query.parse(*pc);
}

}


