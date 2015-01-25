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

	void tree_node::setValue(const std::string& value) {
					this->value = value;
				}
	void tree_node::setRuleName(const std::string& rule_name){
		this->rule_name = rule_name;
	}
	void tree_node::setRuleType(const std::string& type){
			this->type = type;
		}

	const std::list<std::shared_ptr<tree_node> > tree_node::getAllChildren() const {
			return children;
		}
	//aggiunge un nodo alla fine
	void tree_node::addBackChildren(std::shared_ptr<tree_node> child) {
		children.push_back(child);
	}

	//aggiunge un nodo all'inizio
	void tree_node::addFrontChildren(std::shared_ptr<tree_node> child) {
		children.push_front(child);
	}

	void tree_node::addParent(std::shared_ptr<tree_node> p) {
		parent = p;
	}

	std::shared_ptr<tree_node> tree_node::getLastBackChildren() {
		return children.back();
	}
	std::shared_ptr<tree_node> tree_node::getLastFrontChildren() {
		return children.front();
	}


};
