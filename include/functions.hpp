/**
 * @author Mike
 * 
 * @file functions.hpp
 * 
 * @description Enthält verschiedene Funktionen
 * 
*/

#ifndef __math_functions_hpp_CALC__
#define __math_functions_hpp_CALC__

#include <iostream>
#include <vector>
#include <string>
#include "math.hpp"
#include "standards.hpp"

// #include "basic_operations.hpp"
namespace math {
	namespace shell{
		class arg;
	}
	namespace rechner {
	
		/**
		* @note Das alte Ergebnis zum Ergebnis addieren/subtrahieren/dividieren/multiplizieren
		* 
		* @author Mike
		*/
		double add_oldresult(math::shell::arg args, double result);
		
		/**
		 * Grad zu Radiant
		 * 
		 * @return Wert in Radiant
		 * @author Mike
		 */
// 		double grad_to_rad(double grad);
		
		/**
		 * Zu einem Operator berechnungen durchführen
		 * 
		 * 
		 * @author Mike
		 */
		math::string operations(math::shell::arg args, math::string equation, math::string command, uint16_t tabs);
			
		/**
		 * @note Position der richtigen Klammer zu berechnen
		 * 
		 * @return neue Position
		 * @author Mike
		 */
		std::size_t pos_bracket(math::string equation, std::size_t temp, std::size_t temp_end);
		
		/**
		 * Radiant oder Grad wenn in args --grad
		 * 
		 * @return Wert in Grad oder Radiant
		 * @author Mike
		 */
		double rad_or_grad(math::shell::arg args, double grad);
		double grad_or_rad(math::shell::arg args, double grad); // nach Befehl wieder in richtigem Format
		/**
		 * Radiant zu Grad
		 * 
		 * @return Wert in Grad
		 * @author Mike
		 */
// 		double rad_to_grad(double rad);
		
		/**
		 * @note Verschiedene Operatoren durchführen
		 * 
		 * @author Mike
		 */
		math::string rechnung(math::shell::arg args, math::string equation, uint16_t tabs = 0);
		/**
		* @note Sachen ersetzen und auch bestimmte Funktionen(sqrt,^) berechnen
		* 
		* @param args Arguments from the shell.
		* @param equation Equation to calculate and replace.
		* @param tabs For verbose, how many tabs are printed to show hierarchy of calculation.
		* 
		* @return Replaced string with the result
		*
		* @author Mike
		*/
		math::string replace(math::shell::arg args, math::string equation);
		
	}

	namespace functions {
		/**
		 * @note Bilden eines Hashwertes zu einem String
		 * 
		 * @return Hashwert als Integerwert
		 * @author Mike
		 */
		constexpr uint32_t const_string_hash(const char *string){
			uint32_t	hash_value	= 0;
			uint32_t	prim[169]	= {
					  2,   3,   5,   7,  11,  13,  17,  19,  23,  29,  31,  37,  41,
					 43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97,	101, 103,
					107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179,
					181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257,
					263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347,
					349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431,
					433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509,
					521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607,
					613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691,
					701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797,
					809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883,
					887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991,
					997};
			uint32_t size = 0;
			
			for(uint16_t i = 0;;i++) {
				if(string[i] == '\0') {
					size = i;
					break;
				}
			}
			
			for(uint32_t i = 0; i < size; i++)
				hash_value +=string[i] * prim[i];
	
			return hash_value;
	
		}
		
		
		/**
		 * @note Eine Zufallszahl in bestimmten Grenzen erzeugen
		 * 
		 * @return Zufallszahl
		 * @author Mike
		 */
		int get_random(int min = 0, int max=100);

	
		/**
		 * @note Anzahl der Stellen eines doubles bestimmen
		 * 
		 * @return std::size_t
		 * @author Mike
		 */
		std::size_t digits(double number); // used
	}
}


#endif
