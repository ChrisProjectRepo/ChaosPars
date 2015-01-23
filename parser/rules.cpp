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
			rule opt = keyword("and") | keyword("or");
			rule condition = rule('>', true) | rule('<', true) | rule('=', true);
			rule end_query = rule(';', true);
			rule point_key = rule('.', true);
			rule comma_key = rule(',', true);

			rule storage_key = tab_key >> point_key >> select_key;

			//Rule Select

			rule select_prop = comma_key >> storage_key;
			rule select = keyword_select >> storage_key >> *select_prop;

			//Rule From

			rule from_prop = comma_key >> tab_key;
			rule from = keyword_from >> tab_key >> *from_prop;

			//Rule Where

			rule where_prop = storage_key >> condition >> storage_key;
			rule operator_prop = opt >> where_prop;
			rule where = keyword_where >> where_prop >> *operator_prop;

			//Rule Query
			rule query = select >> from >> where >> end_query;
			}

bool parse_string(tree_struct::builder* b,parser_context* pc){

		rules::query[std::bind(&tree_struct::builder::make_node_rule, b, _1, "query", vector<string> { "select", "from", "where", "end_query" })];

		rules::select[std::bind(&tree_struct::builder::make_node_rule, b, _1, "select", std::vector<std::string> { "keyword_select", "storage_key", "select_prop" })];
		rules::select_prop[bind(&tree_struct::builder::make_node_rule, b, _1, "select_prop", vector<string> { "comma_key", "storage_key" })];

		rules::from[bind(&tree_struct::builder::make_node_rule, b, _1, "from", vector<string> { "keyword_from", "tab_key", "from_prop" })];
		rules::from_prop[bind(&tree_struct::builder::make_node_rule, b, _1, "from_prop", vector<string> { "comma_key", "tab_key" })];

		rules::where[bind(&tree_struct::builder::make_node_rule, b, _1, "where", vector<string> { "keyword_where", "where_prop", "operator_prop" })];
		rules::where_prop[bind(&tree_struct::builder::make_node_rule, b, _1, "where_prop", vector<string> { "storage_key", "condition", "storage_key" })];
		rules::operator_prop[bind(&tree_struct::builder::make_node_rule, b, _1, "operator_prop", vector<string> { "opt", "where_prop" })];

		rules::storage_key[bind(&tree_struct::builder::make_node_rule, b, _1, "storage_key", vector<string> { "tab_key", "point_key", "select_key" })];

		rules::condition[bind(&tree_struct::builder::make_leaf, b, _1, "condition")];
		rules::point_key[bind(&tree_struct::builder::make_leaf, b, _1, "point_key")];
		rules::comma_key[bind(&tree_struct::builder::make_leaf, b, _1, "comma_key")];
		rules::tab_key[bind(&tree_struct::builder::make_leaf,b, _1, "tab_key")];
		rules::select_key[bind(&tree_struct::builder::make_leaf, b, _1, "select_key")];
		rules::opt[bind(&tree_struct::builder::make_leaf, b, _1, "opt")];
		rules::end_query[bind(&tree_struct::builder::make_leaf, b, _1, "end_query")];
		rules::keyword_select[bind(&tree_struct::builder::make_leaf, b, _1, "keyword_select")];
		rules::keyword_from[bind(&tree_struct::builder::make_leaf,b, _1, "keyword_from")];
		rules::keyword_where[bind(&tree_struct::builder::make_leaf, b, _1, "keyword_where")];

		return rules::query.parse(*pc);
}

}


