#include "tree.hpp"

namespace tree_struct {
//Struttura Albero
tree_node::tree_node(std::string rule_name) :
			rule_name(rule_name) {
		type = "Rule";
	}

tree_node::tree_node(std::string rule_name, std::string value) :
			rule_name(rule_name), value(value) {
		type = "Terminal";
	}
const std::string tree_node::getRuleName() const {
		return rule_name;
	}
	const std::string tree_node::getValue() const {
		return value;
	}
	const std::string tree_node::getRuleType() const {
		return type;
	}
	//aggiunge un nodo alla fine
	void tree_node::add_back_children(std::shared_ptr<tree_node> child) {
		children.push_back(child);
	}

	//aggiunge un nodo all'inizio
	void tree_node::add_front_children(std::shared_ptr<tree_node> child) {
		children.push_front(child);
	}

	void tree_node::add_parent(std::shared_ptr<tree_node> p) {
		parent = p;
	}

	const std::list<std::shared_ptr<tree_node> > tree_node::get_all_children() const {
		return children;
	}

	std::shared_ptr<tree_node> tree_node::get_last_back_children() {
		return children.back();
	}
	std::shared_ptr<tree_node> tree_node::get_last_front_children() {
		return children.front();
	}

};
