/**
 * @author Mike
 * 
 * @file logging.hpp
 * 
 * @description Enthält Funktionen zum Loggen
 * 
*/

#ifndef __math_logging_hpp_CALC__
#define __math_logging_hpp_CALC__

#include "math.hpp"

	namespace math{
		namespace rechner {
			
			/**
			* @note Logfile backup
			* 
			* @author Mike
			*/
			void backup_logfile(shell::arg args);
			
			/**
			* @note Nullen am Ende löschen
			* 
			* @author Mike
			*/
			math::string delete_zeros(double equation);
		
			/**
			* @note Altes Ergebnis speichern
			* 
			* @author Mike
			*/
			void save_matrix(math::shell::arg args, math::string matrix_equation, math::string matrix_result);
			void save_result(shell::arg args, math::string equation, double result);
			
			
			/**
			* @note Logfile zurücksetzen
			* 
			* @author Mike
			*/
			void reset_logfile(shell::arg args);
		}
	}
#endif
