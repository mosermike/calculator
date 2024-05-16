/**
 * : @author Mike Moser
 * : @date 07.01.2024
 * : @version 3.0.4
 * : @name rechner
 * : @file rechner.cpp
 * : 
 * : Versionsverlauf:
 * : @version  3.0.4 (07.01.2024) Fixed some bugs
 * : @version  3.0.3 (27.01.2023) Determinantenberechnung korrigiert; Korrektur, wenn eine Matrix ohne Anfangs- bzw. Endklammer übergeben wird.
 * : @version  3.0.2 (06.09.2022) Atomare Masseneinheit hinzugefügt, logb todo gelöscht => einfach als log(x)/log(b) berechnen, Log Datei nur 0 löschen nach dem Komma
 * : @version  3.0.1 (04.03.2022) Bracket Problem gelöst (bracket check); arcsin, arccos, sqrt domain error hinzugefügt, grad <=> rad korrigiert
 * : @version  3.0   (17.09.2021) Komplette Operatoren überarbeitet und ein paar Fehler behoben.
 * : @version  2.5.5 (03.02.2021) Grad zu Radiant, Radiant zu Grad, arcsin, arccos, arctan hinzugefügt
 * : @version  2.5.4 (30.10.2020) Verbesserungen, Gauß-Verfahren korrigiert bei Nullspalte, logging verbessert, old_add reingemacht
 * : @version  2.5.3 (08.07.2020) Logging für Matrix
 * : @version  2.5.2 (30.04.2020) Fehlerüberarbeitung summation neu gemacht, da mittlerer Term ignoriert
 * : @version  2.5.1 (14.04.2020) Fehlerüberarbeitung
 * ; @version  2.5   (07.03.2020) Logdatei überarbeitet durch Klasse log
 * : @version  2.4   (22.12.2019) Kommentarfunktion für die logdatei
 * : @version  2.3   (10.08.2019) Logdatei implementiert mit Ausgabe über -l, --logfile
 * : @version  2.2   (14.03.2019) Physikalische Konstanten hinzugefügt, -u überarbeitet mit anstatt , , getArg implementiert
 * : @version  2.1   (27.02.2019) pos_bracket überarbeitet und verbessert, Klammerbehandlung hinzugefügt, --calculations erstellt, --cs mit alten Resultaten und ersetzen von o ohne -o bei --cs,
 * : 							  save_result als inline, calculations erstellt mit Übernahme von den Ergebnissen bei -cs
 * : @version  2.0   (22.02.2019) Funktion replace und add_oldresult, sqrt integriert, ^ integriert, si-präfixe, mehrere Variablen verwenden; log, ln, log2, logb, exp, sin, cos, tan integriert,
 * : 							  calculate erstellt, -t hinzugefügt
 * : @version  1.6   (17.02.2019) e Funktion hinzugefügt, exponentenfunktion durch pow ersetzt, if Bedingungen für pi und ki hinzugefügt
 * : @version  1.5   (10.12.2018) Pi Funktion hinzugefügt für basic_operations, sowie Verbesserungen von -s und -m, Übernahme der alten Werte, Variable x verwenden, mml::string integriert
 * : @version  1.4   (09.11.2018) basic_operations.hpp erstellt mit -s und -m
 * : @version  1.3   (01.11.2018) Grenzen angepasst, die erlaubt sind. Hat bei der Multiplikation 9 nicht erlaubt
 * : @version  1.2   (15.10.2018) Neue Funktion rechnen mit Exponenten zur Basis, Hilfeseite nach Muster, uintX_t angepasst
 * : @version  1.1   (22.05.2018) Ableiten einer einfachen Funktion
 * : @version  1.0   (21.05.2018)
 * :
 * :
 * : @details:
 * : Berechnung von verschiedenen Sachen mit der Konsole
 * : 
 * :
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>	// setprecision
#include <mml.hpp>
#include <mml-math.hpp>

#ifndef rechner_log
#define rechner_log 		"/tmp/rechner.log"
#endif

// NOTE Rechner
// TODO Bruchform ausgeben
// TODO log(12) in -cs geht nicht (behoben, aber nur notlösung => works no?)
// TODO nachschauen welche Variablen gehen und welche nicht => Warnung ausgeben
// TODO grad in rad umrechner
// TODO Warnung wenn Buchstabe in Rechnung : rechnung -c asd
// TODO look at logb
// TODO Ausgaben werden gekürzt

// NOTE Matrix
// TODO Matrix old rechnung (bzw. in matrixform ausgeben temporär)
// TODO Matrixnorm
// TODO Skalarmultiplikation matrix
// TODO rechner -m "[0,0,1]*[[0,2,0],[3,0,2],[0,3,0]]"

int main(int argc, char **argv) {
	
	mml::shell::arg		args		(argc,argv);
	mml::Timer			time		;
	double				result		= 0;
	mml::string			matrix		= "";
	
	if(args.notArg("-t","--timer"))
		time.stop();
		
	if(args.size() == 1){
	
		mml::shell::letter("red");
		std::cout << "Fehlende Eingabe der gewünschten Operation!" << std::endl;
		mml::shell::normal();
		
		mml::help::help("rechner");
		
		return 1;
	}
	
	else if (args.findArg("--help","-h"))
		mml::help::help("rechner");
	
	else if ((args.findArg("-c","--calculate") /*|| args.findArg("-s","--summation")*/ || args.findArg("-m","--matrix")) && args.size() == 2) {
		mml::shell::warn("Fehlende Eingabe!");
	}
	
	else if (args.findArg("-c","--calculate")) {
		result = mml::rechner::calculate(args,args[args.positionArg("-c") + 1], true);
        mml::rechner::save_result(args, args[args.positionArg("-c","--calculate") + 1],result,rechner_log);
    }
	
	else if (args.findArg("-cs","--calculations")) {
		std::vector<mml::string> equations;
		std::vector<double> results = mml::rechner::calculations(args, equations);
		
		// Save the results and equations
		for(uint32_t i = 0; i < equations.size(); i++)
			mml::rechner::save_result(args, equations[i], results[i],rechner_log);
	}
	else if(args.findArg("-m","--matrix")) {

		/*
		 * NOTE Transform from latex matric to compatible format
		 */
		if(args.findArg("-ml","--matrix-latex")) {
			mml::string temp = args[args.positionArg("-m","--matrix") + 1]; // Line with the matrix calculations
			mml::string temp_s = ""; // string to build the new calculations line
			bool braket = false;	// to indicate whether the matrix is finished or starts
			// Build string in compatible format:
			for(uint32_t i = 0; i < temp.size();i++) {
				if(temp[i] == '&')
					temp_s += ",";
				else if(temp[i] == '\\')
					temp_s += "],[";
				else if(temp[i] == '|' and braket){
					temp_s += "]]";
					braket = false;
				}
				else if(temp[i] == '|' and not braket) {
					temp_s += "[[";
					braket = true;
				}
				else
					temp_s = temp_s.str() + (char) temp[i];
			}
			args[args.positionArg("-m","--matrix") + 1] = temp_s;
		}

		args[args.positionArg("-m","--matrix") + 1] = args[args.positionArg("-m","--matrix") + 1].replace(" ",""); // Leerzeichen ersetzen, aber Achtung Matrix falsch wenn nicht mit " "
		
		if (args.findArg("-a","--adjunct"))
			matrix = mml::rechner::matrix_adjunct(args[args.positionArg("-m","--matrix") + 1],true);
		else if (args.exist("-d","--det")) 
			matrix = "det(" + args[args.positionArg("-m","--matrix")+1].str() + ") = " + std::to_string(mml::rechner::matrix_determinante(args[args.positionArg("-m","--matrix") + 1],true));
		else if (args.findArg("-i","--inverse"))
			matrix = mml::rechner::matrix_inverse(args[args.positionArg("-m","--matrix") + 1],true);
		else if (args.findArg("-tr","--transpose"))
			matrix = mml::rechner::matrix_transposed(args[args.positionArg("-m","--matrix") + 1],true);
		else {
			matrix = mml::rechner::matrix_calculation(args, args[args.positionArg("-m","--matrix") + 1]);
			mml::rechner::save_matrix(args, args[args.positionArg("-m","--matrix") + 1], matrix,rechner_log);
		}
	}
	else if (args.findArg("-s","--summation")){
		mml::vector<double> values = mml::rechner::add(args);
		result = values.sum();
		values.log(rechner_log, false, "[summation] ", " = " + std::to_string(result));
	}
	else if (args.findArg("-l","--log","-lv")) {
		if(mml::Unix::exist(rechner_log)) {
			
			mml::log log(rechner_log);
			log.print(args.exist("-v","--verbose","-lv"));
		}
		else
			std::cout << "Es existiert keine Logdatei!" << std::endl;
		return 0;
		
	}
	else if(args.findArg("-lr", "log_reset")) {
		mml::rechner::reset_logfile(args,rechner_log);
		return 0;
	}
	
	else if(args.findArg("-lb","--log-backup")) {
		mml::rechner::backup_logfile(args, rechner_log);
		return 0;
	}
	else {
		result = mml::rechner::calculate(args,args[1]);
        mml::rechner::save_result(args, args[1],result,rechner_log);
    }
	
	if ((args.findArg("-c","--calculate") || args.findArg("-s", "--summation") || args.exist("+", "-", "*", "/")) && !args.findArg("-cs", "--calculations") && args.notArg("-m","--matrix")) {
		if(args.findArg("-p","--digits")) {
			// TODO betaphase
			std::cout << "Result (eventually wrong decimal point numbers!): " << std::setprecision(args[args.positionArg("-p","--digits")+1].atoi()) << result << std::endl;
			
		}
		else
			std::cout << "Result: " << result << std::endl;
	}
	
	
	if(args.findArg("-t","--timer"))
		std::cout << "Calculation time: " << time.range() << std::endl;

	return 0;
	
}
