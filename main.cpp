#include "./parser/parser_exception.hpp"
#include "./parser/rules.hpp"
#include <sstream>

using namespace chaos_parser;
using namespace tree_struct;
using namespace std;

///////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

	//Costruttore usato creare albero sintattico
	builder b;

	// Stringa da parsare
	stringstream str("select device.storage_key , device_2.storage_key from device,device_2 where device.storage_id=device_2.storageid;");
	// Stringa da parsare presa da terminale come argomento
	//stringstream str(argv[1]);

	cout << "String to parse: " << str.str() << endl;

	//Parser e preparazione dello stream da cui prendere i dati
	parser_context pc;
	pc.set_stream(str);

	bool f = false;

	try {
		f=parse_string(&b,&pc);
	} catch (chaos_parser::exception::parser_exception & e) {
		cout << "Parse exception!" << endl;
	}
	if (!f) {

		//Se ritorna false vuol dire che la parola non fa parte della grammatica
		cout << pc.get_formatted_err_msg();
	} else {
		cout << "Parola Parsata con Successo!!!" << endl;
		// Costruzione dell'albero
		cout << "Creo Albero" << endl;
		string albero;
		//Classica visita Dfs di un albero
		tree_visit(b.get_tree_radix(), albero);
		cout << "Albero Creato con Successo" << endl;
		cout << "Parola estratta dall'Albero = " << albero << endl;

	}
}
