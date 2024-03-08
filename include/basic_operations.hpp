/**
 * @author Mike
 * 
 * @file basic_operations.hpp
 * 
 * @description Enthält verschiedene Basic Rechenoperationen
 * 
*/

#ifndef __math_basic_operations_hpp_CALC__
#define __math_basic_operations_hpp_CALC__

#include "math.hpp"

namespace math {
	namespace rechner {
		
		extern void save_result(shell::arg args, math::string equation, double result);

		/**
		 * @note Addition of values with backspace as entries
		 * @return double
		 * @author mike
		 */
		math::vector<double> add(math::shell::arg args);
		/**
		* @note equation with many operators
		* 
		* @return double
		* @author Mike
		*/
		double calculate(math::shell::arg args, math::string equation, bool replace = true);
		
		/**
		* @note Execute multiple calculations
		* 
		* @return double
		* @author Mike
		*/
		double calculations(math::shell::arg args);
		/**
		 * @note Berechnungen ausführen für andere Programme (keine Ausgaben)
		 * 
		 * @return double
		 * @author Mike
		 */
		double calculate_intern(string equation, bool verbose = false, bool replace = true);
		
		/**
		 * @note Werte multiplizieren
		 * 
		 * @return double
		 * @author Mike
		 */
		double multiply(math::shell::arg args, math::string, bool replace = true);


		/**
		* @note Werte addieren
		* 
		* @return double
		* @author Mike
		*/
		double summation(math::shell::arg args, math::string equation, bool replace = true);

	}
}
#endif
