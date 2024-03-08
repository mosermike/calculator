/**
 * @author Mike Moser
 * 
 * @file file.hpp
 * @note Enthält verschiedene Funktionen für Dateien und Ordner
 * 
*/

#ifndef __FILE_HPP_CALC__
#define __FILE_HPP_CALC__

#include <functional>
#include "functions.hpp"
#include "shell.hpp"
#include <iostream>

namespace math{
	namespace file{
		
		/**
		 * @note Typ bestimmen, ob das Ziel eine Datei, Ordner etc. ist
		 * 
		 * @return Gibt eine Zahl zurück. Interpretierung siehe definitions.hpp
		 * @author Mike
		*/
		//int32_t  filetype(std::string filepath);
		
		/**
		 * @note Anzahl an Linien aus einer Datei bestimmen
		 * 
		 * @return Linienanzahl
		 * @author Mike
		 */
		std::size_t num_lines(std::string path);

		/**
		 * @note Berechnet die Größe einer Datei
		 * 
		 * @return Gibt die Größe der Datei zurück
		 * @author Lucas
		 */
		std::size_t size(const std::string& filename);

	}
}
#endif
