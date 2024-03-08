/**
 * : @author Mike Moser
 * : @date 07.03.2020
 * : @file logging.cpp
 * : 
 * : 
 * : Beschreibung: 
 * : Logging Informationen
 * :
 * :
 * :
 * :
 */

#include "../include/math.hpp"
#include <vector>
#include <string>
#include "../include/log.hpp"
#include "../include/logging.hpp"

void math::rechner::backup_logfile(shell::arg args) {
	
	math::log log(_rechner_log);
	log.backup(args.exist("-v","--verbose"));
}

math::string math::rechner::delete_zeros(double result) {
	
	math::string		result_str	= math::to_string(result);
	math::string		temp		= result_str.substr(result_str.find('.')+1);
	math::string		temp1		= result_str.substr(0,result_str.find('.')+1);
	std::size_t		length		= temp.size();
	bool			comma		= false; // only delete zeros after the comma not before
	
	for(uint32_t i = length - 1; i > 0; i--) {
		if(temp[i] == '.')
			comma = true;
		if(!comma)
			continue;
		if(temp[i] == '0')
			temp = temp.substr(0,temp.size() - 1);
		else
			break;
	}
	
	return temp1 + temp;
}

void math::rechner::reset_logfile(shell::arg args) {
	
	math::log log(_rechner_log);
	
	log.reset(args.exist("-v","--verbose"));
}


void math::rechner::save_result(math::shell::arg args, math::string equation, double result) {
	
	math::log log(_rechner_log);
	
	equation = equation + " = " + delete_zeros(result).str();
	if(args.exist("-k","--comment"))
		equation = equation + "  \\\\ " + args[args.positionArg("-k","--comment") + 1].str();
	
	log << equation;
}

void math::rechner::save_matrix(math::shell::arg args, math::string matrix_equation, math::string matrix_result) {
	
	math::log log(_rechner_log);
	
	if(args.exist("-k","--comment"))
		matrix_result = matrix_result + "  \\\\ " + args[args.positionArg("-k","--comment") + 1].str();
	
	log << matrix_equation + " = " + matrix_result.str();
}
