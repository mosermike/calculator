/**
 * : @author Mike Moser
 * : 
 * : @file help.cpp
 * : Enthält verschiedene Hilfeseiten
 * : 
*/
#include <iostream>
#include <fstream>
#include "../include/help.hpp"
#include "../include/math.hpp"


void math::help::header(std::string befehl, std::string option) {
	
	math::shell::chapter("NAME",true);
	std::cout << "\t" << "calc" << " - " << befehl << std::endl;
	math::shell::letter("white");
	math::shell::chapter("AUFRUF",true);
	math::shell::chapter("\tcalc",false);
	std::cout << option << " [OPTION]" << std::endl;
	math::shell::chapter("BESCHREIBUNG",true);
}

void math::help::foot() {
	
	math::shell::chapter("AUTOR",true);
	std::cout << "Written from Mike Moser" << std::endl << std::endl;
	std::cout << "Version: " << 1.0 << "\t\t\t\t\t\t" << "March" << " " << 2024 << std::endl;
}


void math::help::rechner() {

	//math::help::status("rechner");

	header("Führt verschiedene Berechungen durch.");
	
	std::cout << "\tThis program performs different calculations depending on the given arguments." << std::endl;
	std::cout << "\tNecessary arguments for long options are also needed for short options.." << std::endl;
	math::shell::option("-a, --adjunct" , "Adjunkte einer Matrix berechnen. Mit -m Matrix übergeben");
	math::shell::option("-c, --calculate" , "Gleichung lösen mit allen Operatoren.");
	math::shell::option("-cs, --calculations" , "Mehrere verschiedene Gleichungen lösen mit allen Operatoren.");
	math::shell::option("-d, --det" , "Determinante einer Matrix berechnen. Mit -m Matrix übergeben");
	math::shell::option("    --grad" , "Ausgabe oder Angabe in Grad in Abhängigkeit des Operators.");
	math::shell::option("-h, --help" , "Ausgabe dieser Hilfe.");
	math::shell::option("-i, --inverse" , "Inverse einer Matrix berechnen. Mit -m Matrix übergeben");
	math::shell::option("-k, --comment"  , "Kommentar in die Logdatei hinter Rechnung.");
	math::shell::option("-l, --log"  , "Ausgabe der Logdatei.");
	math::shell::option("-lr, --log_reset", "Zurücksetzen der Logdatei");
	math::shell::option("-lb, --log_backup", "Backup der Logdatei");
	math::shell::option("-m, --matrix" , "Multiplzieren und Addieren von Matrizen.Matrixform [[1.,Reihe],[2.,Reihe]]");
	math::shell::option("-ml, --matrix-latex" , "Format in Latex. (Achtung Matrix endet und startet mit '|')");
	math::shell::option("-o, --oldresult" , "Übernahme des alten Resultates an der Stelle &.");
	math::shell::option("-o+" , "Übernahme des alten Resultates und addieren zum Ergebnis.");
	math::shell::option("-o-" , "Übernahme des alten Resultates und subtrahieren zum Ergebnis.");
	math::shell::option("-o*" , "Übernahme des alten Resultates und multiplizieren zum Ergebnis.");
	math::shell::option("-o/" , "Übernahme des alten Resultates und dividieren zum Ergebnis.");
	math::shell::option("-t, --timer", "Ausgabe der Dauer der Berechnung");
	math::shell::option("-tr, --transpose" , "Transponierte einer Matrix berechnen. Mit -m Matrix übergeben");
	math::shell::option("-u" , "Wertzuweisung einer oder mehreren unbekannten Variable(n) mit Trennzeichen ':' in der Gleichung. z.B. -u x=2");
	math::shell::chapter("Verfügbare Operationen:", true);
	math::shell::option("+" , "Addition verschiedener Werte.");
	math::shell::option("-" , "Subtrahieren verschiedener Werte.");
	math::shell::option("*", "Multiplikation verschiedener Werte.");
	math::shell::option("/" , "Dividieren von zwei Zahlen(alt).");
	math::shell::option("//" , "Dividieren von mehreren Zahlen(alt).");
	math::shell::option("d" , "Ableiten einer Funktion. Direkt als Parameter übergeben(alt).");
	math::shell::option("^" , "Multiplizieren einer Basis zu einem Exponenten.");
	math::shell::option("^(" , "Multiplizieren einer Basis zu einem Exponenten.");
	math::shell::option("exp(x)" , "Berechnen von exp^(x).");
	math::shell::option("sin(x)" , "sin berechnen von x.");
	math::shell::option("cos(x)" , "cos berechnen von x.");
	math::shell::option("tan(x)" , "tan berechnen von x.");
	math::shell::option("arcsin(x)" , "arcsin berechnen von x.");
	math::shell::option("arccos(x)" , "arccos berechnen von x.");
	math::shell::option("arctan(x)" , "arctan berechnen von x.");
	math::shell::option("ln(x)" , "logarithmus naturalis berechnen von x.");
	math::shell::option("log(x)" , "10er Logarithmus berechnen von x.");
	math::shell::option("log2(x)" , "2er Logarithmus berechnen von x.");
	math::shell::option("logb(x)" , "Logarithmus zur Basis b berechnen von x.");
	math::shell::option("sqrt(x)" , "Wurzel ziehen von x.");
	std::cout << std::endl;
	math::shell::chapter("Optionen bei --calculations:",true);
	math::shell::option("rx", "Verwendung des Ergebnisses von Gleichung x, rx wird in der neuen Gleichung ersetzt");
	std::cout << std::endl;
	math::shell::chapter("Sonstiges:", true);
	std::cout << "\tVorhandene Konstanten: pi, exp,E , P_c, P_k, P_m, P_e, P_G, P_h, P_µ, P_NA, P_u" << std::endl;
	std::cout << "\tVorhandene Potenzen:   p, n, µ, m, k, M, G, T oder mit e" << std::endl << std::endl;
		
	foot();
}

/*
void math::help::statistik() {

	math::help::status("statistik");

	header("Berechnung von statistischen Werten.");
	
	std::cout << "\tDieses Programm führt verschiedene Berechnungen im Rahmen der Statistik durch abhängig von den Argumenten." << std::endl;
	std::cout << "\tHier können wichtige Berechnungen für Auswertungen durchgeführt werden, die aus der Statistik kommen."	<< std::endl << std::endl;
	std::cout << "\tErforderliche Argumente für lange Optionen sind auch für kurze erforderlich." << std::endl;
	std::cout << "\tMögliche Berechnungen für Partitionen, Ordner, Dateien." << std::endl;
	math::shell::option("-f, --file" , "Daten aus einer Datei importieren");
	math::shell::option("-f1, --file1", "x-Werte für die lineare Regression");
	math::shell::option("-f2, --file2", "y-Werte für die lineare Regression");
	math::shell::option("-h, --help" , "Ausgabe dieser Hilfe.");
	math::shell::option("-k, --Kovarianz","Berechnung der Kovarianz");
	math::shell::option("-l, --log"  , "Ausgabe der Logdatei.");
	math::shell::option("-lr, --log_reset", "Zurücksetzen der Logdatei");
	math::shell::option("-lb, --log_backup", "Backup der Logdatei");
	math::shell::option("-L, --lineareRegression","Durchführung einer linearen Regression der Form y = a + b*x. Eingabe der Werte über std::cin");
	math::shell::option("-o, --oldData","Übernahme der alten Werte");
	math::shell::option("-p, --python","Übernahme der Daten aus einem mitgegebenen Python Array.");
	math::shell::option("-r, --round","Gerundete Ausgaben");
	math::shell::option("-s, --Standardabweichung","Durchführung der Berechnung des Mittelwertes mit der Standardabweichung. Eingabe der Werte über std::cin");
	math::shell::option("-T, --t-value","t-value berechnen");
	math::shell::option("-t, --t-value","Zeit ausgeben (ohne Eingabe)");
	math::shell::option("-wm, --weightmean", "Gewichteter Mittelwert berechnen.");
	math::shell::option("-v, --verbose","Ausgabe der Berechnungsschritten.");
	
	
	
	foot();
}*/
