/**
 * @author Mike
 * 
 * @file matrix.hpp
 * 
 * @description Enthält verschiedene Matrizenberechungen
 * 
*/

#ifndef __math_matrix_hpp_CALC__
#define __math_matrix_hpp_CALC__

#include "math.hpp"

namespace math {
	namespace rechner {
		
		math::string matrix_calculation(math::shell::arg args, math::string matrix);
		
		/**
		 * @note Matrix addieren oder subtrahieren
		 * 
		 * @return Erebnis als String
		 * @author Mike
		 */
		math::string matrix_addition(math::string matrix1, math::string matrix2, char op = '+', bool ausgabe = false);
		/**
		 * @note Adjunkte einer Matrix bestimmen (wird für Inverses benötigt)
		 * 
		 * @return Erebnis als String
		 * @author Mike
		 */
		math::string matrix_adjunct(math::string matrix, bool ausgabe = false);
		/**
		 * @note Determinante einer Matrix bestimmen
		 * 
		 * @return Erebnis als double
		 * @author Mike
		 */
		double matrix_determinante(string matrix, const bool ausgabe);
		/**
		 * @note Inverse einer Matrix bestimmen
		 * 
		 * @return Erebnis als String
		 * @author Mike
		 */
		math::string matrix_inverse(math::string matrix, bool ausgabe = false);
		/**
		 * @note Matrizen multiplizieren
		 * 
		 * @return Erebnis als String
		 * @author Mike
		 */
		math::string matrix_multiply(math::string matrix1, math::string matrix2, bool ausgabe = false);
		/**
		 * @note Matrix ohne bestimmte Zeile und Linie
		 * 
		 * @return Erebnis als String
		 * @author Mike
		 */
		math::string matrix_smaller(math::string matrix, uint32_t row, uint32_t column, bool ausgabe = false);
		/**
		 * @note Matrix ausgeben
		 * 
		 * @return Erebnis als String
		 * @author Mike
		 */
		void matrix_print(math::string matrix);
		/**
		 * @note Matrix transponieren
		 * 
		 * @return Erebnis als String
		 * @author Mike
		 */
		math::string matrix_transposed(math::string matrix, bool ausgabe = false);
	}
}


#endif // __matrix_hpp__
