//#define __INFOMACRO__
#include "macro.hpp"
#include "utils.hpp"

namespace tree_struct {

	node_stack_data::node_stack_data(std::string rule_name, std::shared_ptr<tree_node> node) :
			rule_name(rule_name), node(node) {
	}

	const std::shared_ptr<tree_node> node_stack_data::getNode() const {
		return node;
	}

	const std::string node_stack_data::getRuleName() const {
		return rule_name;
	}

	void builder::make_leaf(chaos_parser::parser_context &pc, std::string rule_name) {
		INFO_LINE("Tocca a:"<<rule_name);
		auto x = pc.collect_tokens();
		if (x.size() < 1)
			throw std::string("Error in collecting variable");
		std::string value = x[x.size() - 1].second;
		auto node_rule = std::make_shared<tree_node>(rule_name, value);
		tree_stack.push(node_stack_data(rule_name, node_rule));
	}

//Dentro Rules verrano messe le regole tab,select quindi ma nello stack in posizione top c'è select
//quindi devo fargli prendere prima la select
	void builder::make_node_rule(chaos_parser::parser_context &pc, std::string rule_name, std::vector<std::string> rules) {
		INFO_LINE("Tocca a:"<<rule_name);
		//Usato per appoggia le regole estratte dallo stack
		node_stack_data data;
		//Creo il nodo padre che in un secondo momento andrò ad inserire dentro lo stack
		std::shared_ptr<tree_node> node_rule = std::make_shared<tree_node>(rule_name);
		//Iteratore per scorrere la lista di rules
		//initializer_list<string>::iterator it;
		for (int it = rules.size() - 1; it != -1; it--) {
			data = tree_stack.top();
			if (data.getRuleName() == rules[it]) {
				//Controllo Usato per gestire la ricorsione, cioè l'asterisco che eviene eseguito ricorsivamente
				if (it == (rules.size() - 1)) {
					it++;
				}
				//Aggiungo figlio al Padre dall'inizio visto che il primo nodo nello stack è l'ultimo dei figli della regola
				//Quindi inserisco sempre in testa cosi il primo scorre fino all'ultima posizione
				node_rule->add_front_children(data.getNode());
				tree_stack.pop();
			}

		}
		//Infine aggiugiamo nello stack la regola di produzione
		tree_stack.push(node_stack_data(rule_name, node_rule));
	}

	void builder::position_order(std::string type_term) {
		INFO_LINE("Tocca a:"<<type_term);
	}

	std::shared_ptr<tree_node> builder::get_tree_radix() {
		node_stack_data data = tree_stack.top();
		return data.getNode();
	}

//Visita albero con stampa a schermo della visita in profondità
	void tree_visit(std::shared_ptr<tree_node> x, std::string &target) {
		//Se trovo una foglia
		if (x->get_all_children().size() == 0) {
			std::string temp = (x->getValue() + " ");
			target.append(temp);
		} else {
			//Se non trovo una foglia per ogni figlio del nodo richiamo la visita sui figli fino ad arrivare ad un figlio foglia
			for (auto i : x->get_all_children()) {
				tree_visit(i, target);
			}
		}
	}

	void tree_visit(std::shared_ptr<tree_node> x) {
		if (x->get_all_children().size() == 0) {
			/*
			 * Operazioni se trovo foglia
			 */
		} else {
			for (auto i : x->get_all_children()) {
				tree_visit(i);
			}
		}
	}

}
;
