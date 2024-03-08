/**
 * @author Mike Moser
 * 
 * Programm shell.cpp
 * Enthält verschiedene Funktionen
 * 
*/ 

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unistd.h> // getpass
#include <chrono>  //sleep function
#include <thread>
#include "../include/definitions.hpp"
#include "../include/shell.hpp"
#include "../include/math.hpp"

//FORMATIERUNGEN DER SHELL ÄNDERN:
/*
namespace functions{
	class string;
}*/
void math::shell::letter(math::string colour) {
//DEFINITION
    std::map<std::string , std::string > letter;
    
//BUCHSTABENFARBEN
	letter[ "black" ] = "30";
	letter[ "darkgray" ] = "1;30";
	letter[ "red" ] = "31";
	letter[ "lightred" ] = "1;31";
	letter[ "green" ] = "32";
	letter[ "lightgreen" ] = "1;32";
	letter[ "brown" ] = "33";
	letter[ "yellow" ] = "1;33";
	letter[ "blue" ] = "34";
	letter[ "lightblue" ] = "1;34";
	letter[ "purple" ] = "35";
	letter[ "lightpurple" ] = "1;35";
	letter[ "cyan" ] = "36";
	letter["lightcyan" ] = "1;36";
	letter[ "white" ] = "37";
	letter[ "lightgray" ] = "1;37";
	letter[ "normal" ] = "0";

	if ( letter[colour.getValue()] == "" ){
		std::cout << "Your colour doesn't exist or something went wrong." << "\nPlease check the uncompiled file, if available. Otherwise use another code for the colour!" << std::endl;
		return;
	}
	
	std::cout << "\e[" << letter[colour.getValue()] << "m";
}

void math::shell::formation(math::string format1) {

	//DEFINITION
	std::map < std::string , int > format;

	//FORMATIERUNGEN
    format [ "normal" ] = 0;
	format [ "fat" ] = 1;
	format [ "cursive" ] = 2;          //Kursiv
    format [ "underline"] = 4;
    format[ "blink" ] = 5;
    format[ "rblink" ] = 25;
    format[ "reverse" ] = 7;
    format[ "rreverse" ] = 27;
	
	if ( format[format1.getValue()] == 0 && format1 != "normal" ) {
		std::cout << "Your colour doesn't exist or something went wrong." << "\nPlease check the uncompiled file, if available. Otherwise use another code for the colour!" << std::endl;
		return;
	}
	std::cout << "\e[" << format[format1.getValue()] << "m";
}
void math::shell::background(math::string colour) {

	//DEFINITION
    std::map < std::string , uint32_t > background;

	//HINTERGRUNDFARBEN
	background[ "black" ] = 40;
    background[ "red" ] = 41;
    background[ "green" ] = 42;
    background[ "brown" ] = 43;
    background[ "blue" ] = 44;
    background[ "purple" ] = 45;
    background[ "cyan" ] = 46;
    background[ "gray" ] = 47;		//=^weiß
    background[ "normal" ] = 49;
	
	if ( background[colour.getValue()] == 0 ){
		std::cout << "Your colour doesn't exist or something went wrong." << "\nPlease check the uncompiled file, if available. Otherwise use another code for the colour!" << std::endl;
		return;
	}
	std::cout << "\e[" << background[colour.getValue()] << "m";
}

// Kapitel hinzufügen bei einer Hilfeseite
void math::shell::chapter(std::string text,bool newline){
	math::shell::letter("lightgray");
	std::cout << text;
	if(newline)
		std::cout << std::endl;
	math::shell::normal();
}

void math::shell::cout(math::string text, bool newline, std::string colour) {
	math::string output = "";
	// kein [ vorhanden => normale Ausgabe
	if(!text.exist("["))
		std::cout << text;
	else {
		// Wert vor [ finden und ausgeben
		std::size_t temp = text.find("[");
		if(temp != 0) {
			output = text.sub(0,temp-1);
			text.replace(output,"",true);
			std::cout << output;
		}
		// Farbe ändern
		math::shell::letter(colour);
		
		// Ende der Klammer finden
		temp = text.find("]");
		output = text.sub(0,temp+1);
		text.replace(output,"",true);
		std::cout << output;
		math::shell::normal();
		std::cout << text;
	}
	
	if(newline)
		std::cout << std::endl;
}

void math::shell::normal(){
	std::cout << "\e[0m";
}

void math::shell::note(std::string text, bool newline) {
	math::shell::letter("lightgray");
	std::cout << text;
	math::shell::normal();
	if(newline)
		std::cout << std::endl;
	math::shell::normal();
}



// Eine Option hinzufügen bei einer Hilfeseite
void math::shell::option(std::string chapter,std::string option){
	math::shell::letter("lightgray");
	std::cout << "\t" << chapter << std::endl;
	math::shell::normal();
	std::cout << "\t\t" << option << std::endl;
}

// Warnungen für Programme, wenn etwas nicht geht
void math::shell::warn(std::string text,bool newline){
	math::shell::letter("red");
	std::cout << text;
	if(newline)
		std::cout << std::endl;
	math::shell::normal();
	exit(EXIT_FAILURE);
}

// Warnungen um Hinweise weiterzugeben/ auszugeben
void math::shell::warn_opt(std::string text,bool newline){
	math::shell::letter("red");
	std::cout << text;
	math::shell::normal();
	if(newline)
		std::cout << std::endl;
	math::shell::normal();
}

void math::shell::error(std::string text){
	math::shell::letter("red");
	std::cout << text<< std::endl;
	math::shell::normal();
	exit(EXIT_FAILURE);
}

extern math::string math_pass;

// Passwort von der Shell lesen
std::string math::shell::password(std::string ausgabe, std::string source){
	std::string data = ausgabe + source + ": ";
	math_pass = getpass(data.c_str());
	return math_pass.str();
}








