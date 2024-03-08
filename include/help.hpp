/**
 * @author Mike Moser
 * 
 * @file help.hpp
 * @note Enthält verschiedene Hilfeseiten für C++-Programme
 * 
*/

//#ifdef __HELP__EXIST__

#ifndef __HELP_CALC__
#define __HELP_CALC__

#include "math.hpp"
#include "standards.hpp"
// TODO letzte Zeile Informationen bestimmen (Version, Jahr)

namespace math {
	namespace help {
		

		/**
		 * @note Ausgabe der jeweiligen Hilfetexte
		 * 
		 * @author Mike
		 */
		void rechner();
		
		/**
		 * @note Ausgabe der Fußzeile mit Angabe zum Autor und Datum
		 * 
		 * @author Mike
		 */
		void foot();
		
		// TODO sha256

		/**
		 * @note Ausgabe des Headers, dass nur von Variablen abhängig ist
		 * 
		 * @author Mike
		 */
		void header(std::string befehl, std::string option = "");
		
		/**
		 * @note Ausgabe in der Konsole
		 * 
		 * @author Mike
		 */
		void help(math::string output);
		
	}
}

#endif
//#endif
