#include <iostream>
#include <fstream>
#include <math.h>
//#include <format>
#include <limits>
#include "../include/math.hpp"
#include "../include/functions.hpp"
#include "../include/basic_operations.hpp"

double math::rechner::add_oldresult(math::shell::arg args, double result) {
	
	if(math::Unix::exist(_rechner_log) && args.findArg("-o+", "-o-", "-o*", "-o/")){
			
			math::string	temp_value	= "";
			
			math::log log(_rechner_log);
			math::string temp = log.lastline();
			uint32_t it = 1;
			while(temp.exist("[")) {
				temp = log.getline_back(it);
				it++;
			}
			
			temp_value = temp.substr(temp.find("=")+2);

			if(args.findArg("-o+"))
				return result	+ temp_value.atof();
			else if(args.findArg("-o-"))
				return result	- temp_value.atof();
			else if(args.findArg("-o*"))
				return result	* temp_value.atof();
			else if(args.findArg("-o/"))
				return result	/ temp_value.atof();
	}
	
	return result;
}


math::string math::rechner::operations(math::shell::arg args, math::string equation, math::string command, uint16_t tabs) {
	bool	verbose = args.exist("-v","--verbose");
	double	temp	= 0.0;
	// Verbose, Say what command is checked.
	if(verbose) {
		for(uint16_t i = 0; i < tabs; i++)
			std::cout << "\t";
		std::cout << "[" << command << "] " << equation;
		if(equation.exist(command.str()))
			std::cout << " " << ": Command exists!";
		std::cout << std::endl;
	}
	
	// ==================
	// Conditions to work
	// ==================
	if (command == "^") {
		command = "^(";
		// NOTE When there is no bracket => Error message
		if(equation[equation.find('^')+1] != '(' && equation.exist("^"))
			math::shell::error("[Operations] After ^ is not a closing bracket!");
	}
	
	// ==========
	// Operations
	// ==========
	for (uint32_t i = 0; i < equation.size(); i++) {
		if(equation.substr(i,command.size()) == command) {
			std::size_t pos_auf = i;
			std::size_t pos_auf_others = 0;
			for (uint32_t j = i+command.size(); j < equation.size(); j++) {
				
				if(equation[j] == '('){
					pos_auf_others++;
				}
				else if(equation[j] == ')') { 
					if(pos_auf_others == 0) {
						// BEGIN Calculate all the operators and replace
						if(command =="ln(")
							equation.replace(equation.sub(pos_auf,j),math::to_string(std::log(rechnung(args,equation.sub(pos_auf+command.size(),j-1),tabs+1).atof())),true, pos_auf);
						else if(command == "log(")
							equation.replace(equation.sub(pos_auf,j),math::to_string(std::log10(rechnung(args,equation.sub(pos_auf+command.size(),j-1),tabs+1).atof())),true, pos_auf);
						else if(command == "log2(")
							equation.replace(equation.sub(pos_auf,j),math::to_string(std::log2(rechnung(args,equation.sub(pos_auf+command.size(),j-1),tabs+1).atof())),true, pos_auf);
// 						else if(command == "l")
// 							equation.replace(equation.sub(pos_auf,j),math::to_string(std::log(rechnung(args,equation.sub(pos_auf+command.size(),j-1)).atof())),true, pos_auf);
						else if(command == "sqrt(") {
							temp = rechnung(args,equation.sub(pos_auf+command.size(),j-1),tabs+1).atof();
							if(temp < 0)
								math::shell::warn("[sqrt] Domain error");
							equation.replace(equation.sub(pos_auf,j),math::to_string(std::sqrt(temp)),true, pos_auf);
						}
						else if(command == "exp(")
							equation.replace(equation.sub(pos_auf,j),math::to_string(std::exp(rechnung(args,equation.sub(pos_auf+command.size(),j-1),tabs+1).atof())),true, pos_auf);
						else if(command == "sin(") {
							temp = math::rechner::rad_or_grad(args,rechnung(args,equation.sub(pos_auf+command.size(),j-1),tabs++).atof());
							equation.replace(equation.sub(pos_auf,j),math::to_string(std::sin(temp)),true, pos_auf);
						}
						else if(command == "arcsin(") {
							temp = rechnung(args,equation.sub(pos_auf+command.size(),j-1),tabs+1).atof();
							if (temp > 1 || temp < -1) // arcsin nur von -1 bis 1 definiert
								math::shell::warn("[arcsin] Domain error");
							equation.replace(equation.sub(pos_auf,j),math::to_string(math::rechner::grad_or_rad(args,std::asin(temp))),true, pos_auf);
						}
						else if(command == "cos(") {
							temp = math::rechner::rad_or_grad(args,rechnung(args,equation.sub(pos_auf+command.size(),j-1),tabs++).atof());
							equation.replace(equation.sub(pos_auf,j),math::to_string(std::cos(temp)),true, pos_auf);
						}
						else if(command == "arccos(") {
							temp = rechnung(args,equation.sub(pos_auf+command.size(),j-1),tabs++).atof();
							if (temp > 1 || temp < -1)
								math::shell::warn("[arcsin] Domain error");
							equation.replace(equation.sub(pos_auf,j),math::to_string(math::rechner::grad_or_rad(args,std::acos(temp))),true, pos_auf);
							
						}
						else if(command == "tan(") {
							temp = math::rechner::rad_or_grad(args,rechnung(args,equation.sub(pos_auf+command.size(),j-1),tabs++).atof());
							equation.replace(equation.sub(pos_auf,j),math::to_string(math::rechner::grad_or_rad(args,std::tan(temp))),true, pos_auf);
						}
						else if(command == "arctan(") {
							temp = rechnung(args,equation.sub(pos_auf+command.size(),j-1),tabs++).atof();
							equation.replace(equation.sub(pos_auf,j),math::to_string(math::rechner::grad_or_rad(args,std::atan(temp))),true, pos_auf);
						}
						else if(command == "^(") {
							
							if(equation.exist("^(")) {
								
								// NOTE Determine base
								double base = 0;
								std::size_t base_pos = 0;
								
								// BEGIN base with brakets
								if( equation[pos_auf-1] == ')') {
									std::size_t pos_zu = pos_auf-1;
									uint32_t pos_zu_others = 0;
									
									for (uint32_t n = pos_zu-1; n >= 0; n--) {
										
										if(equation[n] == ')')
											pos_zu_others++;
										else if(equation[n] == '(') {
											if (pos_zu_others == 0) {
												base = math::rechner::calculate_intern(equation.sub(n+1,pos_zu-1),verbose);
												
												// BEGIN Correctur so that with the exponent being even the base is positive and not negative
												if(base < 0) {
													if(verbose)
														std::cout << "base negative" << std::endl;
													if(equation.sub(pos_auf+command.size(),j-1).atoi() % 2 == 0)
														base = base*(-1);
												}
												// END Correctur...
												equation.replace(equation.sub(n,pos_zu),math::to_string(base),true, n, false); // replace base
												return math::rechner::rechnung(args,equation);
												break;
											}
											else
												pos_zu_others--;
										}
									}
								}
								// END base with brakets
															
								for(int32_t t = pos_auf; t >= 0; t--){
									if ( t == 0 ) {
										if(equation[0] == '(') {
											base = equation.sub(t+1,pos_auf-1).atof();
											base_pos = 1;
											break;
										}
										else if(equation[0] != '-') {
											base = equation.sub(t,pos_auf-1).atof();
											break;
										}
										else {
											base = equation.sub(t,pos_auf-1).atof();
											base_pos = t;
											break;
										}
									}
									if(((equation[t] == '+' && equation[t-1] != 'e') || (equation[t] == '-'  && equation[t-1] != 'e')  || equation[t] == '/' || equation[t] == '(' || equation[t] == '*')) {
										base = equation.sub(t+1,pos_auf-1).atof();
										base_pos = t+1;
										break;
									}
																
								}
								
								// NOTE If in the exponent is an equation
								if(equation.sub(pos_auf+command.size(),j-1).exist("+","-","/","*")) {
									equation.replace(equation.sub(base_pos,j),math::to_string(std::pow(base,math::rechner::rechnung(args,equation.sub(pos_auf+command.size(),j-1)).atof())),true, base_pos);
								}
								else
									equation.replace(equation.sub(base_pos,j),math::to_string(std::pow(base,equation.sub(pos_auf+command.size(),j-1).atof())),true, base_pos);
								equation = math::rechner::rechnung(args,equation);
 								}
						}
						else if(command == "(") {
							equation = equation.replace(equation.sub(pos_auf,j),rechnung(args,equation.sub(pos_auf+1,j-1)),true, pos_auf);
						}
						else
							math::shell::error("[Operations] " + command.str() + " is not working or not implemented. Please contact programmer");
						
						// END Calculate all the operators and replace
						i = 0;
						break;
						
					}
					else
						pos_auf_others--;
						
				}
			}
			
			i = 0;
		}
	}
	
	return equation;
}


std::size_t math::rechner::pos_bracket(math::string equation, std::size_t temp, std::size_t temp_end) {
	
	int		a			= 0;	// Anzahl an Klammer zu
	int		b			= 1;	// Anzahl Klammer auf
	
	
	for(uint32_t i = temp; i < equation.size(); i++) {
		// Abbruchbedingung:
		if(a >= b && a > 0)
			break;
		
		// Zählen wie viele Klammer auf
		if(equation[i] == '(')
			b++;
		// Zählen wie viele Klammer zu
		else if(equation[i] == ')') {
			a++;
		}
			
	}
	// Position von richtigem ) bestimmen:
	for(uint32_t i = temp; i < equation.size(); i++) {
		if(equation[i] == ')')
			a--;
		if(a <= 0)
			return i;
	}
	
	return temp_end;
}

double math::rechner::rad_or_grad(math::shell::arg args, double grad) {return args.exist("--grad") ? (double) grad/180*M_PI : grad;}
double math::rechner::grad_or_rad(math::shell::arg args, double grad) {return args.exist("--grad") ? (double) grad*180/M_PI : grad;}

// double math::rechner::rad_to_grad(double rad) {return (double) 180 * rad/M_PI;}

math::string math::rechner::rechnung(math::shell::arg args, math::string equation, uint16_t tabs) {
		
 		bool	verbose	= args.exist("-v","--verbose");
		
	/*  ==================
	 *  NOTE bracket check 
	 *  ================== */
		uint8_t bra_o = 0; // brackets open
		uint8_t bra_c = 0; // brackets closed
		for(uint16_t i = 0; i < equation.size(); i++) {
			if(equation[i] == '(')
				bra_o++;
			else if(equation[i] == ')')
				bra_c++;
		}
		
		if (bra_o != bra_c)
			math::shell::warn("[rechnung] number of open and closed brackets don't match!");
	//	END bracket check
		equation = operations(args,equation,"ln(", tabs);
		equation = operations(args,equation,"log(", tabs);
		equation = operations(args,equation,"log2(", tabs);
		equation = operations(args,equation,"sqrt(", tabs);
		equation = operations(args,equation,"exp(", tabs);
		equation = operations(args,equation,"arcsin(", tabs);
		equation = operations(args,equation,"sin(", tabs);
		equation = operations(args,equation,"arccos(", tabs);
		equation = operations(args,equation,"cos(", tabs);
		equation = operations(args,equation,"arctan(", tabs);
		equation = operations(args,equation,"tan(", tabs);
		equation = operations(args,equation,"^", tabs);
		equation = operations(args,equation,"(", tabs);
		if(verbose)
			std::cout << "Operations done" << std::endl;
 		equation = math::to_string(math::rechner::calculate_intern(equation,verbose,false));
		
		return equation;
}

math::string math::rechner::replace(math::shell::arg args, math::string equation) {
	/**
	 * @note , durch . ersetzen
	 */
	equation.replace(",",".",true,0,true);

	/** 
	 * @note Alte Rechnungsergebnis verwenden
	 */
	if(math::Unix::exist(_rechner_log) && (args.findArg("-o") || args.findArg("-cs","--calculations")) ){
			
			math::log log(_rechner_log);
			math::string	temp_value	= log.lastline();
			temp_value = temp_value.sub(temp_value.find("=") + 2, temp_value.size()-1);
			
			if(equation.exist("&")) {
				while(equation.exist("&")) 
					equation.replace("&",temp_value,true);
			}
		}
	else if(equation.exist("&"))
		math::shell::error("[replace] & found and the missing argument -o");
		
	/** 
	 * @note Variablen ersetzen
	 */
	
	if(args.findArg("-u")){
			
		math::string					variable				= "";
		math::string					variables				= args[args.positionArg("-u") + 1];
		math::string					value	 				= "";
		math::string					var						= "";
		std::size_t					temp					= variables.find("=");
		std::size_t					pos						= variables.find(":");
		
		do{
			// die aktuelle Variable finden (alles bis zum =) und String kürzen inklusive =
			var = variables.substr(0,temp);
			
			// falls : vorhanden ist => weitere Variablen vorhanden
			if(pos < std::string::npos) {
				// Value bestimmen vor :
				value = variables.substr(temp + 1, pos - temp - 1).str();
				
				equation.replace(var,value,true);
			}
			// kein : vorhanden, nur noch eine Variable übrig
			else {
				equation.replace(var, variables.substr(temp + 1), true);		// Variable durch den Wert ersetzen
				break;
			}
			// variables kürzen
			variables = variables.substr(pos + 1);
			
			// Neue Positionen von = und , bestimmen
			temp = variables.find("=");
			pos = variables.find(",");
			
		} while(temp < std::string::npos);
	}
	
	/** 
	 * @note Physikalische Werte ersetzen
	 */
		equation.replace("P_c", "299792458", true);										// Lichtgeschwindigkeit
		equation.replace("P_µ", "1.256637e-6", true);								// Magnetische Permeabilität
		equation.replace("P_e", "1.602177e-19", true);								// Elementarladung
		equation.replace("P_k", "1.380648e-23", true);								// Boltzmann-Konstanten
		equation.replace("P_h", "6.626070e-34", true);								// Planksches Wirkungsquantum
		equation.replace("P_m", "9.109384e-31", true);								// Elektronenmasse
		equation.replace("P_G", "6.67e-11", true);									// Gravitationskonstante
		equation.replace("P_u", "1.660539066e-27", true);							// Atomare Masseneinheit
		equation.replace("P_NA", "6.02214076e+23", true);							// Avogadro Konstante
	
	/**
	 * @note Mathematische Sachen ersetzen
	 */
		std::string pi = "3.14159265358979323846"; //std::format("{}\n", M_PI);
		equation.replace("P_Pi",pi,"Pi",pi,true);		// PI herausfiltern und durch Wert ersetzen
		equation.replace("pi",pi,"PI",pi,true);	// Pi herausfiltern und durch Wert ersetzen
	
	/**
	 * @note e ersetzen
	 */
		if(equation.exist("E") || equation.exist("exp")) {
			
			if(equation.exist("E"))
				equation.replace("E",std::to_string(M_E),true);	// e herausfiltern und durch Wert ersetzen
			
			else {
			
				for(uint32_t i = 0; i < equation.size(); i++) {
				
					if(equation[i] == 'E' && equation[i + 1] != 'x') {
					
						equation = equation.substr(0, i) + std::to_string(M_E) + equation.substr(i + 1).str();
						
						i += std::to_string(M_E).size();
						
						if(!equation.exist("E"))
							break;
					}
					
				}
			}
		
		}
	
		
	
	/** 
	 * @note SI-Präfixe ersetzen
	 */
		
		while(equation.exist("p","n","µ","m") || equation.exist("k","M","G","T")) {
			int32_t praefix = 0;
		
			if(equation.exist("p") && ((int) equation[equation.find("p")-1] > 47 && (int) equation[equation.find("p")-1] < 58))
				praefix = -12;
			else if(equation.exist("n") && ((int) equation[equation.find("n")-1] > 47 && (int) equation[equation.find("n")-1] < 58))
				praefix = -9;
			else if(equation.exist("µ") && ((int) equation[equation.find("µ")-1] > 47 && (int) equation[equation.find("µ")-1] < 58))
				praefix = -6;
			else if(equation.exist("m") && ((int) equation[equation.find("m")-1] > 47 && (int) equation[equation.find("m")-1] < 58))
				praefix = -3;
			else if(equation.exist("k") && ((int) equation[equation.find("k")-1] > 47 && (int) equation[equation.find("k")-1] < 58))
				praefix = 3;
			else if(equation.exist("M") && ((int) equation[equation.find("M")-1] > 47 && (int) equation[equation.find("M")-1] < 58))
				praefix = 6;
			else if(equation.exist("G") && ((int) equation[equation.find("G")-1] > 47 && (int) equation[equation.find("G")-1] < 58))
				praefix = 9;
			else if(equation.exist("T") && ((int) equation[equation.find("T")-1] > 47 && (int) equation[equation.find("T")-1] < 58))
				praefix = 12;
			
			if(!math::atob(praefix))
				break;
			
			switch(praefix) {
				case -12 : equation.replace("p","*0.000000000001",true)				; break;
				case -9  : equation.replace("n","*0.000000001",true)					; break;
				case -6  : equation.replace("µ","*" + std::to_string(pow(10,praefix)),true)	; break;
				case -3  : equation.replace("m","*" + std::to_string(pow(10,praefix)),true)	; break;
				case  3  : equation.replace("k","*" + std::to_string(pow(10,praefix)),true)	; break;
				case  6  : equation.replace("M","*" + std::to_string(pow(10,praefix)),true)	; break;
				case  9  : equation.replace("G","*" + std::to_string(pow(10,praefix)),true)	; break;
				case  12 : equation.replace("T","*" + std::to_string(pow(10,praefix)),true)	; break;
				default  : break;
			}
			
		}

	equation = math::rechner::rechnung(args,equation);
	return equation;
}


std::size_t math::functions::digits(double number){
	math::string temp = math::to_string(number);
	
	// Korrektur, wenn eine ganze Zahl vorhanden
	if(temp[temp.size()-1]== '.')
		temp = temp.sub(0,temp.size()-2);
	
	return temp.size();
}

int math::functions::get_random(int min, int max) {
	
	std::random_device rd;     // only used once to initialise (seed) engine
	
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	
	std::uniform_int_distribution<int> uni(min,max); // guaranteed unbiased

	return uni(rng);
}
