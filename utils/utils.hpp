/*
 * Utils.hpp
 *
 *  Created on: 22/dic/2014
 *      Author: cicciox
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <stack>
#include <vector>
#include "../parser/query_parser.hpp"
#include "tree.hpp"

namespace tree_struct {
//Definizione nuovo Oggetto che verrà usato nello stack
//Si poteva usare tranquillamente un pair<type 1, type 2>
	class node_stack_data {
			std::string rule_name;
			std::shared_ptr<tree_node> node;
		public:
			node_stack_data() = default;

			node_stack_data(std::string rule_name, std::shared_ptr<tree_node> node);

			const std::shared_ptr<tree_node> getNode() const;

			const std::string getRuleName() const;
	};

//Class Builder utilizzato per la costruzione dell'Albero;
	class builder {
			//Stack delle chiamate alle Rule per la costruzione dell'Albero
			//A fine esecuzione in cima allo stack sarà presente la radice dell'Albero
			std::stack<node_stack_data> tree_stack;

		public:
			void make_leaf(chaos_parser::parser_context &pc, std::string rule_name);

//Dentro Rules verrano messe le regole tab,select quindi ma nello stack in posizione top c'è select
//quindi devo fargli prendere prima la select
			void make_node_rule(chaos_parser::parser_context &pc, std::string rule_name, std::vector<std::string> rules);

			void position_order(std::string type_term);

			std::shared_ptr<tree_node> get_tree_radix();

	};

//Funzione usata per la visita ricorsiva dell'albero di Query
	void tree_visit(std::shared_ptr<tree_node> x, std::string &target);
	void tree_visit(std::shared_ptr<tree_node> x);

}

#endif /* UTILS_HPP_ */
