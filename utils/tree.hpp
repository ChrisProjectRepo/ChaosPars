/*
 * Tree.hpp
 *
 *  Created on: 22/dic/2014
 *      Author: cicciox
 */

#ifndef TREE_HPP_
#define TREE_HPP_

#include <string>
#include <memory>
#include <list>

namespace tree_struct {

//Definizione della Struttura dati dell'Albero
class tree_node {

	std::string rule_name;
	std::string value;
	//Non necessario
	std::string type;
	std::shared_ptr<tree_node> parent;
	std::list<std::shared_ptr<tree_node>> children;

public:
	tree_node(std::string rule_name);

	tree_node(std::string rule_name, std::string value);

	const std::string getRuleName()const;

	const std::string getValue()const;

	const std::string getRuleType()const;

	const std::list<std::shared_ptr<tree_node> > get_all_children()const;

	//aggiunge un nodo alla fine
	void add_back_children(std::shared_ptr<tree_node> child);

	//aggiunge un nodo all'inizio
	void add_front_children(std::shared_ptr<tree_node> child);

	//aggiunge padre , non usata perchè non necessaria
	void add_parent(std::shared_ptr<tree_node> p);

	//restituisce ultimo nodo della lista
	std::shared_ptr<tree_node> get_last_back_children();

	//restituisce ultimo nodo inserito in cima
	std::shared_ptr<tree_node> get_last_front_children();

};

}

#endif /* TREE_HPP_ */
