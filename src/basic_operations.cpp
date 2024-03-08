
#include <iostream>
#include <fstream>
#include <math.h>
#include <limits>
#include "../include/functions.hpp"
#include "../include/basic_operations.hpp"
#include <signal.h>

math::vector<double> math::rechner::add(math::shell::arg args) {
// 	std::string				temp_str	= "";
	math::string				temp_s		= "";
	math::vector<double>		values;
	
	
// 	if(args.findArg("-f","--file")) {
// 		
// 		if(!math::Unix::exist(args[args.positionArg("-f","--file")+1]))
// 			math::shell::error("[input] Datei nicht vorhanden!");
// 		
// 		std::ifstream input(args[args.positionArg("-f","--file")+1].c_str());
// 		
// 		while(!input.eof()) {
// 			std::getline(input,temp_str);
// 			
// 			if(temp_str == "")
// 				continue;
// 			temp_s = temp_str;
// 			temp_s.replace(",",".",true);
// 			values.push_back(((math::string)temp_str).atof());
// 		}
// 	}
// 	
// 	else {
		std::cout << "[Addition] Eingabe der Werte (Beenden mit 'a', zurück mit 'z'): " << std::endl;
		
		double temp_d = 0.0;
		for(uint32_t i = 0;; i++) {
			std::cin >> temp_s;
			
			if(temp_s.exist("a")){
				break;
			}
			if(temp_s.exist("z") && values.size() > 0){
				values.pop_back();
				continue;
			}
			temp_s.replace(",",".",true);
			temp_d = math::rechner::calculate_intern(temp_s);
			values.push_back(temp_d);
		}
		
// 	}

	return values;
}

double math::rechner::calculate(math::shell::arg args, math::string equation, bool replace){
	
	bool verbose = args.exist("-v","--verbose");
	// NOTE Nur eine Zahl mit e => Keine Berechnungen oder sonstiges durchführen, wegen dem - oder plus würde es aber durhcgeführt werden
	// BEGIN number with e
	if(equation.exist("e") && !equation.exist("(","*","/")) {
		std::size_t			e_plus		= equation.find("e+");
		std::size_t			e_minus		= equation.find("e-");
		
		if(verbose) {
			std::cout << "Präfix with e exist" << std::endl;
			std::cout << "-: " << equation.count("-") << std::endl;
			std::cout << "+: " << equation.count("+") << std::endl;
			std::cout << "Range e-: " << math::range(e_minus) << std::endl;
			std::cout << "Range e+: " << math::range(e_plus) << std::endl;
		}
		
		// only plus no minus
		if(math::range(e_plus) && !(math::range(e_minus))) {
			if (equation.count("+") == 1 && equation.count("-") == 0) {
				double ret = equation.atof();
				return ret;
			}
		}
		// only minus no plus
		else if(!math::range(e_plus) && math::range(e_minus)) {
			if (equation.count("-") == 1 && equation.count("+") == 0) {
				double ret = equation.atof();
				return ret;
			}
		}
		if(verbose)
			std::cout << "e exist finished with " << equation << std::endl;
		
		
	}
	// END number with e
	
    math::string equation_save = equation;
	
	if(replace)	// als erstes das Replace, da sonst die Positionen falsch sein können
		equation = math::rechner::replace(args, equation);
	
	double				result		= 0;
	std::size_t			plus		= equation.find("+");
	std::size_t			minus		= equation.find("-");
	std::size_t			mal			= equation.find("*");
	std::size_t			teilen		= equation.find("/");
	std::size_t			before		= 0;
	std::size_t			after		= 0;
	math::string			temp		= "";

	if(!equation.exist("+","-","*","/") && !equation.exist("^","(") && args.notArg("-o+","-o-","-o*","-o/"))
		return equation.atof();
	else if(!equation.exist("+","-","*","/") && !equation.exist("^","(") && args.findArg("-o+","-o-","-o*","-o/"))
		return math::rechner::add_oldresult(args,equation.atof());

	// Keine Addition:
	if(!math::range(plus,minus))
		return multiply(args,equation,false);
	
	// Keine Multiplikation:
	if(!math::range(mal,teilen))
		return summation(args,equation,false);
		
	// Multiplikationen durchführen:
	if(math::range(mal)) {
		// neue Position, falls Gleichung geändert
		mal = equation.find("*");
		do {
			// before bestimmen:
			
			for(uint32_t i = mal - 1; i > 0; i--) {
				if(((int) equation[i] < 46  || (int) equation[i] > 64) && equation[i-1] != 'e' && equation[i] != 'e') {
					before = i;
					break;
				}
			}
			// after bestimmen:
			for(uint32_t i = mal + 2;; i++) {
				if(i >= equation.size()) {
					after = equation.size();
					break;
				}
				if(((int) equation[i] < 46  || (int) equation[i] > 64) && equation[i-1] != 'e' && equation[i] != 'e') {
					after = i;
					break;
				}
				
			}
			// Korrektur, wenn vorher kein Operator:
			if(before == 0)
				before--;
			
			// Berechnung:
			temp = math::rechner::multiply(args,equation.substr(before + 1, after - before - 1), false);
			
			equation.replace(equation.substr(before + 1, after - before - 1), temp, true);
			
			mal = equation.find("*");
			
			
		} while(math::range(mal));
		
	}
	
	// Teilen-Positionsbestimmung wiederholen, falls bereits bei Multiply berechnet:
	teilen		= equation.find("/");
	
	// Dividieren:
	if(math::range(teilen)) {
		// neue Position, falls Gleichung geändert
		teilen = equation.find("/");
		
		do {
			// before bestimmen:
			for(uint32_t i = teilen - 1; i > 0; i--) {
				if((int) equation[i] < 46 || (int) equation[i] > 64) {
					before = i;
					break;
				}
				else
					before = 0;
			}
			// after bestimmen:
			for(uint32_t i = teilen + 2;; i++) {
				if(i == equation.size()) {
					after = i;
					break;
				}
				if((int) equation[i] < 46 || (int) equation[i] > 64) {
					after = i;
					break;
				}
				
			}
			
			// Wenn kein anderer Operator vorher:
			if(before == 0)
				before--;
			
			// Berechnung:
			temp = multiply(args,equation.substr(before + 1, after - before - 1),false);
			
			// Ersetzen
			equation.replace(equation.substr(before + 1, after - before - 1), temp, true);
			
			teilen = equation.find("/");
			
			
		} while(math::range(teilen));
		
	}
	
	result = summation(args,equation,false);
	
	// Altes Ergebnis hinzuaddieren
	result = add_oldresult(args,result);

	return result;
}


double math::rechner::calculations(math::shell::arg args) {
	
	double					result			= 0;
	math::string				equation		= "";
    math::string				equation_save	= "";
	std::vector<double>		results;
		
		while(1) {
			std::cout << "Eingabe der Gleichung " + std::to_string(results.size() + 1) << ": ";
			

			std::cin >> equation;
			if(equation == "q")
				return 0;
			equation_save = equation;
			// Eingabe von Strg+D:
			if(equation == "") {
				std::cout << std::endl;
				kill(getpid(),SIGTERM);
			}
			// Übernahme des Ergebnis von anderen Eingaben:
			if(equation.exist("r")) {
				std::size_t pos = equation.find("r");
				
				do {
					
					// Verwendung von sqrt:
					if(equation[pos+1] == 't') {
						pos = equation.find("r",pos + 1);
						continue;
					}
					// Verwendung von arcsin, arccos, arctan
					if(equation[pos+1] == 'c') {
						pos = equation.find("r",pos + 1);
						continue;
					}
					uint32_t temp	= 0;
					
					for(uint32_t a = pos + 1; temp == 0; a++) {
						if(equation[a] < 47 || equation[a] > 57) {
								if(equation.substr(pos+1,a - pos)[0] > 57 || equation.substr(pos+1,a - pos)[0] < 48)
									math::shell::error("| Error: Falsches Format für Option rx! x muss eine Zahl sein");
								temp = equation.substr(pos+1,a - pos).stoi();
						}
					}
					
					equation.replace("r" + math::to_string(temp).str(), math::to_string(results[temp - 1]),true);
					
					pos = equation.find("r");
				} while(math::range(pos));
			}
			
			
			result = math::rechner::calculate(args,equation);
			results.push_back(result);
			std::cout << "Result: " << result << std::endl;
			
            math::rechner::save_result(args, equation_save, result);
            
			// Strg-D abfangen:
			equation = "";
		}
	
	return result;
}

double math::rechner::calculate_intern(math::string equation, bool verbose, bool replace){
	
	// BEGIN number with e
	if(equation.exist("e")) {
		
		std::size_t			e_plus		= equation.find("e+");
		std::size_t			e_minus		= equation.find("e-");
		
		if(verbose) {
			std::cout << "Präfix with e exist" << std::endl;
			std::cout << "-: " << equation.count("-") << std::endl;
			std::cout << "+: " << equation.count("+") << std::endl;
			std::cout << "*: " << equation.count("*") << std::endl;
			std::cout << "/: " << equation.count("/") << std::endl;
			std::cout << "Range e-: " << math::range(e_minus) << std::endl;
			std::cout << "Range e+: " << math::range(e_plus) << std::endl;
		}
		
		// times and divide
		if(equation.count("*") == 0 && equation.count("/") == 0){
				
			// only plus no minus
			if(math::range(e_plus) && !(math::range(e_minus))) {
				if (equation.count("+") == 1 && equation.count("-") == 0) {
					double ret = equation.atof();
					return ret;
				}
			}
			// only minus no plus
			else if(!math::range(e_plus) && math::range(e_minus)) {
				if (equation.count("-") == 1 && equation.count("+") == 0) {
					double ret = equation.atof();
					return ret;
				}
			}
		}
		
		if(verbose)
			std::cout << "e exist finished with " << equation << std::endl;
	}
	// END number with e
	
    math::string equation_save = equation;
	math::shell::arg args;
	if(replace)
		equation = math::rechner::replace(args, equation);
	if(verbose)
		args.push_back("-v");
	
	
	double				result		= 0;
	std::size_t			plus		= equation.find("+");
	std::size_t			minus		= equation.find("-");
	std::size_t			mal			= equation.find("*");
	std::size_t			teilen		= equation.find("/");
	std::size_t			before		= 0;
	std::size_t			after		= 0;
	math::string			temp		= "";
	
	if(!equation.exist("+","-","*","/") && !equation.exist("^","("))
		return equation.atof();
	
	// Keine Addition:
	if(!math::range(plus,minus))
		return multiply(args,equation,false);
	
	// Keine Multiplikation:
	if(!math::range(mal,teilen))
		return summation(args,equation,false);
		
	
	// Multiplikationen durchführen:
	if(math::range(mal)) {
		// neue Position, falls Gleichung geändert
		mal = equation.find("*");
		do {
			// before bestimmen:
			
			for(uint32_t i = mal - 1; i > 0; i--) {
				if(((int) equation[i] < 46  || (int) equation[i] > 64) && equation[i-1] != 'e' && equation[i] != 'e') {
					before = i;
					break;
				}
			}
			// after bestimmen:
			for(uint32_t i = mal + 2;; i++) {
				if(i >= equation.size()) {
					after = equation.size();
					break;
				}
				if(((int) equation[i] < 46  || (int) equation[i] > 64) && equation[i-1] != 'e' && equation[i] != 'e') {
					after = i;
					break;
				}
				
			}
			// Korrektur, wenn vorher kein Operator:
			if(before == 0)
				before--;
			
			// Berechnung:
			temp = math::to_string(math::rechner::multiply(args,equation.substr(before + 1, after - before - 1), false));
			
			equation.replace(equation.substr(before + 1, after - before - 1), temp, true);
			
			mal = equation.find("*");
			
			
		} while(math::range(mal));
		
	}
	
	// Teilen-Positionsbestimmung wiederholen, falls bereits bei Multiply berechnet:
	teilen		= equation.find("/");
	
	// Dividieren:
	if(math::range(teilen)) {
		// neue Position, falls Gleichung geändert
		teilen = equation.find("/");
		
		do {
			// before bestimmen:
			for(uint32_t i = teilen - 1; i > 0; i--) {
				if(((int) equation[i] < 46  || (int) equation[i] > 64) && equation[i-1] != 'e' && equation[i] != 'e') {
					before = i;
					break;
				}
				else
					before = 0;
			}
			// after bestimmen:
			for(uint32_t i = teilen + 2;; i++) {
				if(i == equation.size()) {
					after = i;
					break;
				}
				if(((int) equation[i] < 46  || (int) equation[i] > 64) && equation[i-1] != 'e' && equation[i] != 'e') {
					after = i;
					break;
				}
				
			}
			
			// Wenn kein anderer Operator vorher:
			if(before == 0)
				before--;
			
			// Berechnung:
			temp = math::to_string(math::rechner::multiply(args,equation.substr(before + 1, after - before - 1),false));
			
			// Ersetzen
			equation.replace(equation.substr(before + 1, after - before - 1), temp, true);
			
			teilen = equation.find("/");
			
			
		} while(math::range(teilen));
		
	}

	result = summation(args,equation,false);

	return result;
}


double math::rechner::summation(math::shell::arg args, math::string equation, bool replace) {
	
	bool verbose = args.exist("-v","--verbose");

	if(verbose)
		std::cout << "[summation] Before: " << equation << std::endl;
	
	// Vorverarbeitung:
	if(replace)
		equation = math::rechner::replace(args, equation);
	
	//if(equation[0] == '0')
	//	equation = equation.substr(1);
	
	double					result		= 0;
	std::size_t pos_temp  = equation.find("+");
	std::size_t pos_temp1 = equation.find("-");
	
	// Falls der erste Wert kein Minus als Vorzeichen hat, muss der erste Wert separat addiert werden:
	if(equation[0] != '-'){
		if(equation[pos_temp1] == '-' && equation[pos_temp1-1] == 'e') 
			pos_temp1 = equation.find("-",pos_temp1+2);
		if(equation[pos_temp] == '+' && equation[pos_temp-1] == 'e') 
			pos_temp = equation.find("+",pos_temp+2);
		
		if(pos_temp < pos_temp1) // als erstes kommt ein +:
			result += equation.substr(0,pos_temp).atof();
		
		else if (pos_temp1 < pos_temp)   // als erstes kommt ein -:  else sehr wichtig hier, ansonsten Zahl zwei mal addiert!
			result += equation.substr(0,pos_temp1).atof();

		else  // nur eine Zahl übergeben
			return equation.atof();
	}
	
	
	// Alle Zahlen, die addiert werden herausfinden:
	for(uint32_t i = 0; i < equation.size(); i++){
		
		if(equation[i] == '+' && equation[i-1] != 'e'){

			pos_temp = equation.find("+",i+1);
			pos_temp1 = equation.find("-",i+1);

			// BEGIN no addition with an e before
			while(equation[pos_temp-1] == 'e')
				pos_temp = equation.find("+",pos_temp+1);

			while(equation[pos_temp1-1] == 'e')
				pos_temp1 = equation.find("-",pos_temp1+1);
			// END no addition with an e before
			
			// BEGIN Additions
			if(pos_temp < pos_temp1)
				result = result + equation.sub(i+1,pos_temp-1).atof();
			else if(pos_temp > pos_temp1)
				result = result + equation.sub(i+1,pos_temp1-1).atof();
			else
				result = result + equation.sub(i+1,equation.size()-1).atof();
			// END Additions
		}
	}
	

// 	Alle Zahlen, die subtrahiert werden herausfinden:
	for(uint32_t i = 0; i < equation.size(); i++){

		if(equation[i] == '-' && equation[i-1] != 'e' && equation[i] != 'e'){
			
			pos_temp = equation.find("-",i+1);
			pos_temp1 = equation.find("+",i+1);
			
			// BEGIN no addition with an e before
			while(equation[pos_temp-1] == 'e')
				pos_temp = equation.find("-",pos_temp+1);

			while(equation[pos_temp1-1] == 'e')
				pos_temp1 = equation.find("+",pos_temp1+1);
			// END no addition with an e before
			
			if(pos_temp < pos_temp1)
				result = result - equation.sub(i+1,pos_temp-1).atof();
			else if(pos_temp > pos_temp1)
				result = result - equation.sub(i+1,pos_temp1-1).atof();
			else
				result = result - equation.sub(i+1,equation.size()-1).atof();
		}
	}
	if(verbose)
		std::cout << "[summation] After: " << result << std::endl;
	
	result = add_oldresult(args,result);
	
	return result;
} 

double math::rechner::multiply(math::shell::arg args, math::string equation, bool replace){
	bool			verbose		= args.exist("-v","--verbose");
	double			result		= 1;	
	std::size_t		pos_temp	= equation.find("*");
	std::size_t		pos_temp1	= equation.find("/");
	
	
	if(verbose)
		std::cout << "[multiply] Before: " << equation << std::endl;

	// Replace variables and other parameters
	if(replace)
		equation = math::rechner::replace(args, equation);

	// Check if division by zero exists
	if(equation.exist("/0") && !equation.exist("/0."))
		math::shell::warn("This is a division by 0!",true);
	
	// If somewhere multiplied by 0 => Return 0
	if(equation.exist("*0") &! equation.exist("*0."))
		return 0;
	
	// Get first value before the first multiplication or division
	if(pos_temp < pos_temp1)
		result = equation.substr(0,pos_temp).atof();
	else if (pos_temp1 < pos_temp)
		result = equation.substr(0,pos_temp1).atof();
	else // nur eine Zahl übergeben
		return equation.atof();
	
	//	*********************
	//	* 	MULTIPLICATION	*
	//	*********************
	for(uint32_t i = 0; i < equation.size(); i++){
	
		// Multiply where index is a star
		if(equation[i] == '*'){
		
			// * Determine next position of * or / *
			pos_temp = equation.find("*",i+1);
			pos_temp1 = equation.find("/",i+1);
			// * or / exists
			if((pos_temp < std::string::npos) || pos_temp1 < std::string::npos)
				result *= equation.substr(i+1,std::min(pos_temp, pos_temp1)-(i+1)).atof();
			else
				result *= equation.substr(i+1).atof();
		}
	}
	
	//	*****************
	//	*	DIVISION	*
	//	*****************
	for(uint32_t i = 0; i < equation.size(); i++){
		
		if(equation[i] == '/'){
		
			// Find positions of * and / (* should not exist anymore but just to be sure)
			pos_temp = equation.find("/",i+1);
			pos_temp1 = equation.find("*",i+1);

			// Divide until next mathematical sign (* or /)
			if((pos_temp < std::string::npos) || pos_temp1 < std::string::npos)
				result /= equation.substr(i+1,std::min(pos_temp,pos_temp1)-(i+1)).atof();
			else
				result /= equation.substr(i+1).atof();
		}
	}

	// Altes Ergebnis hinzuaddieren
	result = add_oldresult(args, result);
	
	if(verbose)
		std::cout << "[multiply] After: " << result << std::endl;

	return result;
} 



void derivate_old(math::string function , bool start){
    

	math::string		addition		= "";
	math::string		sin_klammer		= "";
	int16_t						exponent		= 0;
	int16_t						faktor			= 0;

	
	if(start){
		std::cout << "Ableitung von einer Funktion bestimmen." << std::endl;
		std::cout << "Funktion: " << function << std::endl;
		//std::cin >> function;	//Funktion einlesen
	}
	
	
	uint32_t pos = function.find("*x");  //Faktor finden  
	uint32_t pos1 = function.find("^(");	//Exponentenanfang finden
	uint32_t pos2 = function.find(")");	//Exponentenende finden
	uint32_t pos3 = function.find("+");	//Zusätzliche Additionen finden
	uint32_t pos4 = function.find("x");	//x vorhanden? sonst nur eine Konstante
	uint32_t sin = function.find("sin");	//sinus vrhanden? Ableitung eine andere
    
	if(start)
		std::cout << "Ergebnis: ";
	//Funtion nur eine Konstante:
	if ( pos4 == UINT32_MAX && start){		//kein x vorhanden => nur eine konstante
		std::cout << "0" << std::endl;
	}
	
	else if( pos3 == UINT32_MAX && start && sin == UINT32_MAX)
		std::cout << function.replace("*x","");
	
	//Funktion ein Sinus
	else if (sin < UINT32_MAX) {											//"sin" im String vorhanden
        uint16_t sin_pos = function.find(")");						//Ende der inneren Klammer
		std::string sin_innen = function.substr(sin+4,(sin_pos-(sin+4))).str();		//Innere Klammer als Substring speichern
		faktor = function.substr(0,sin-1).atoi();			//Faktor bestimmen: Unterstring bilden von Anfang des Strings bis zur Position des Faktors, dann umwandeln
		std::cout << faktor << "* cos(" << sin_innen << ") * ";
		if(sin_innen != "x")
            derivate_old(sin_innen, false);	//Rekursion
	}
	
	//Faktor bilden falls es einen Exponenten gibt: Ansonsten handelt es sich nur um eine Konstante
	else if (pos1 == UINT32_MAX && start && sin == UINT32_MAX){
			std::cout << "0";
	}
	
	//Konstanten mitten in der Funktion ignorieren/löschen:
	else if ( pos3 < pos && !start){
		//Ist nur eine Konstante
	}
    else if(pos4 < UINT32_MAX && pos1 == UINT32_MAX && !start){
        std::string factor = function.substr(pos3+1,pos4-pos3-1).str();
        std::cout << factor << std::endl;
    }
    else if(pos4 < UINT32_MAX && pos1 == UINT32_MAX && start){
        std::string factor = function.substr(pos4-1).str();
        std::cout << factor << std::endl;
    }
	//Normale Ableitung der Funktion der Form: N*x^(n)
	else {
		exponent = std::atoi(function.substr(pos1+2, pos2).c_str());	//Exponent bestimmen (nicht abgeleitet): Unterstring bilden von Exponentenanfang bis Expontenende, dann umwandeln
		faktor =   std::atoi(function.substr(0,pos).c_str());			//Faktor bestimmen: Unterstring bilden von Anfang des Strings bis zur Position des Faktors, dann umwandeln
		long dev_factor = (faktor * exponent);						//Faktor der abgeleiteten Funktion bestimmen
		int dev_exponent = exponent-1;								//Exponent der abgeleiteten Funktion berechnen
    
    
        if(dev_factor == 0 && exponent != 0 && exponent != 1 )		//Kein Faktor vorhanden, aber einen Exponent
            dev_factor = exponent;
    
        if(dev_factor != 0)											//Faktor nur ausgeben, wenn er nicht null ist
            std::cout << dev_factor << "*";
	
        std::cout << "x^" << dev_exponent;	//Ausgabe des Ergebnises
	}
	
	// + in der sinus Funktion
	if(pos2 < pos3)
            pos3 = function.find("+",pos3+1);
        
	//Weitere Anhägsel vorhanden oder am Ende der Funktion angekommen?
    if(pos3 < UINT32_MAX){
		
        addition = function.substr(pos3+1);	//kürzen des Strings zu den noch nicht behandelten Teilen
		pos4 = addition.find("x");	//x vorhanden? sonst nur eine Konstante
		if ( pos4 < UINT32_MAX ){		//x vorhanden => nicht nur eine Konstante
				if( !( pos3 < pos && !start))	//Nur + ausgeben, wenn mitten in der Funktion keine Konstante existiert
					std::cout << " + ";
				derivate_old(addition, false);	//Rekursion
		}
		
	}
    
}