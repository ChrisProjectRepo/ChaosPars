#include "./parser/parser_exception.hpp"
#include <sstream>
#include "parser/rules_implementation.hpp"

using namespace chaos_parser;
using namespace tree_struct;
using namespace std;

///////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

	//Costruttore usato creare albero sintattico
	builder b;

	// Stringa da parsare
	stringstream str("select device.storage_key from device where device.storageid > device_2.storageid");
	//stringstream str(argv[1]);

	cout << "String to parse: " << str.str() << endl;

	//Parser e preparazione dello stream da cui prendere i dati
	parser_context pc;
	pc.setStream(str);

	bool f = false;

	try {
		f = parseString(b,pc);
	} catch (chaos_parser::exception::parser_exception & e) {
		cout << "Errore sintassi query!!!" << endl;
		cout << "Parse exception!" << endl;
	}
	if (!f) {
		//Se ritorna false vuol dire che la parola non fa parte della grammatica
		cout << "Stringa non appartenente al linguaggio!!!" << endl;
		cout << pc.getFormattedErrMsg();
	} else {
		cout << "Parola Parsata con Successo!!!" << endl;
		// Costruzione dell'albero
		cout << "Creo Albero" << endl;
		string albero;
		//Classica visita Dfs di un albero
		treeVisit(b.get_tree_radix(), albero);
		cout << "Albero Creato con Successo!!!" << endl;
		cout << "Parola estratta dall'Albero = " << albero << endl;

	}
}
