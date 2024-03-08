/**
 * @author Mike Moser
 * 
 * Quelldatei args.cpp
 * Enthält verschiedene Funktionen für die Klasse args
 * 
*/

#include <iostream>
#include <filesystem> 
#include "../include/log.hpp"
#include "../include/standards.hpp"
#include "../include/file.hpp"
#include "../include/shell.hpp"



	math::log &math::log::operator=(std::string value){
		math::log temp(value);
		return *this;
		
	}

	void math::log::backup(bool verbose, bool Reset) {
		math::shell::arg Empty;
		
		Empty.push_back("-f");
		if(verbose)
			std::cout << "| Backup logfile...";
		
		// Write into log that a backup is executed
		cout("[check_root] backup is executed at " + math::date("Datum+Uhrzeit") + ".", true);
		
		if(math::Unix::exist(logpath.str() + ".bak5"))
			std::remove((logpath + ".bak5").c_str());

		// Execute backup
		for(int32_t i = 4; i >= 0; i--) {
			// NOTE 0 is for logpath.bak and logpath.bak1
			if(i == 0) {
				if(math::Unix::exist(logpath.str() + ".bak"))
					std::filesystem::copy((logpath + ".bak").c_str(),(logpath + ".bak1").c_str());
				std::filesystem::copy(logpath.c_str() ,(logpath + ".bak").c_str());
			}
			else if(math::Unix::exist(logpath.str() + ".bak" + std::to_string(i))) {
				std::filesystem::copy((logpath + ".bak"  + std::to_string(i)).c_str() ,(logpath + ".bak" + std::to_string((i+1))).c_str());
			}
		}
		if(verbose)
			std::cout << std::endl;
		
		// Reset first file to have a new file if wished
		if(Reset)
			reset(verbose);
	}
	
	void math::log::close() {
		this->output.close();
	}
	
	void math::log::cout(math::string value,bool newline) {
		this->output << value;
		if(newline)
			this->output << std::endl;
	}
// 	void math::log::cout(std::string value,bool newline) {
// 		this->output << value;
// 		if(newline)
// 			this->output << std::endl;
// 	}
	void math::log::cout(double value,bool newline) {
		this->output << value;
		if(newline)
			this->output << std::endl;
	}
	void math::log::cout(int value,bool newline) {
		this->output << value;
		if(newline)
			this->output << std::endl;
	}
	
	math::string math::log::getline(uint32_t line) {
		math::string value;
		
		value.getline(logpath.str(), line);
		
		return value;
	}
	
	math::string math::log::getline_back(uint32_t line) {
		std::vector<std::streampos>		lines	;
		std::string						value	= "";
		std::ifstream					input	(this->logpath.c_str());
		
		// Zeilenpositionen bestimmen
		while(!input.eof()) {
			lines.push_back(input.tellg());	// Zeilenposition speichern
			std::getline(input,value);		// zur nächsten Zeile
		}
		
		// letzte Zeile eine newline => Zeilenposition entfernen
		if(value == "")
			lines.pop_back();
		
		// Machbarkeit prüfen
		if(line > lines.size())
			math::shell::error("[getline_back] Falsche Zeilenangabe!");
		
		// Position setzen
		input.clear();	// error state zurücksetzen, sonst bei seekg und tellg error
		input.seekg(lines[lines.size() - 1 - line]);
		
		std::getline(input,value);
		return value;
		
	}
	
	math::string math::log::lastline() {
		std::string value;
		
		value = getline_back(0).str();
		
		return value;
	}
	
	void math::log::header() {
		output << "| Log-Datei " << logpath << std::endl;
		output << "| Zeitstempel: " << date("Datum+Uhrzeit") << std::endl;
	}

	void math::log::open(math::string path) {
		// Logdatei öffnen
		if(path == "")
			path = this->logpath;
		
		this->output.close();
		this->output.open(path.c_str(),std::ios::out | std::ios::app);
		this->logpath = path;
	}
	
	void math::log::print(bool linenumber) {
		output.close();
		std::ifstream input(logpath.c_str());
		uint32_t lines = 1;
		while(!input.eof()) {
			
			math::string value;
			value.getline(input);
			
			// verbose: Print linenumber
			if(linenumber) {
				if(lines < 10)
					std::cout << "0";
				std::cout << lines << ": ";
			}
// 			if(value[0] == '[' && value[value.size()-1] == ']')
// 				math::shell::letter("lightgreen");
			math::shell::cout(value,true);
// 			math::shell::normal();
			lines++;
		}
	}
	
	void math::log::reset(bool verbose) {
		if(verbose)
			std::cout << "| Reset logfile...";
		this->output.close();
		this->output.open(this->logpath.c_str());
		header();
		if(verbose)
			std::cout << std::endl;
	}


	

