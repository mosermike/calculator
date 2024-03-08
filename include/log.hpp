/**
 * @author Mike Moser
 * 
 * @file log.hpp
 * @note Enthält verschiedene Funktionen fürs logging
 * 
*/

#ifndef __LOG_HPP_CALC_
#define __LOG_HPP_CALC_

#include <iostream>
#include <fstream>
#include <string>

#include "standards.hpp"

// TODO an letzter Zeile etwas anhängen
// TODO bei Ausgabe anhand von Dateigröße entscheiden wie viele führende Nullen
// TODO backup => verschieben der Logdatei und nicht einfach löschen
// TODO Ordner erstellen, Berechtigung prüfen Ausgabe prüfen => als root bei pi geht nicht, Ordner auch nicht vorhanden
namespace math{
	
	namespace Unix{
		extern bool exist(std::string path);
	}
	
	namespace file {
		extern void remove(std::string file);
	}
	
	namespace shell {
		void error(std::string text);
	}
	
	class log{
		private:
			math::string logpath = "";
			std::ofstream output;
			
		public:
			
			// Constructor
			log(math::string path) : logpath(path), output() {
				logpath = path;
				if(!math::Unix::exist(logpath.str())) {
					output.open(logpath.c_str(),std::ios::out | std::ios::app);
					header();
					if(!math::Unix::exist(logpath.str()))
						math::shell::error("[LOG] Erstellung der Logdatei nicht möglich. Berechtigung prüfen!");
				}
				else
					output.open(logpath.c_str(),std::ios::out | std::ios::app);
			}
			log() : logpath(""), output() {}
		
			// Deconstructor
			~log(){output.close();}
			
 			/**
			 * @note Neue Logfile öffnen;
			 * 
			 * @author Mike
			 */
			math::log &operator=(std::string value);
			
// 			friend void operator<< (math::log &log, const char* &String){
// 				log.output << String << std::endl;
// 				return;
// 			}
			
// 			friend void operator<< (math::log &log, const std::string &String){
// 				log.output << String << std::endl;
// 				return;
// 			}
			
			friend void operator<< (math::log &log, const math::string &String){
				log.output << String << std::endl;
				return;
			}

			
			friend void operator<< (math::log &log, const double &String){
				log.output << String << std::endl;
				return;
			}
			
			/**
			 * @note Logdatei sichern
			 * 
			 * @author Mike
			 */
			void backup(bool verbose = true, bool Reset = true);
			
			/**
			 * @note Logdatei schließen
			 * 
			 * @author Mike
			 */
			void close();
			
			/**
			 * @note in Logdatei schreiben
			 * 
			 * @author Mike
			 */
			void cout(math::string value,bool newline=false);
// 			void cout(std::string value,bool newline=false);
			void cout(double value,bool newline=false);
			void cout(int value,bool newline=false);
			//void compress(std::size_t filesize);
			
			/**
			 * @note Eine bestimmte Zeile aus der Logdatei zurückgeben
			 * 
			 * @return string
			 * @author Mike
			 */
			math::string getline(uint32_t line);
			
			/**
			 * @note Eine bestimmte Zeile aus der Logdatei zurückgeben, dabei vom Ende aus Rechnen
			 * 
			 * @return string
			 * @author Mike
			 */
			math::string getline_back(uint32_t line);
			
			/**
			 * @note Die letzte Zeile aus der Logdatei zurückgeben
			 * 
			 * @return stirng
			 * @author Mike
			 */
			math::string lastline();
			/**
			 * @note Neuen Kopf erstellen
			 * 
			 * @author Mike
			 */
			void header();
			/**
			 * @note Logdatei öffnen
			 * 
			 * @author Mike
			 */
			void open(math::string path = "");
			
			/**
			 * @note Logdatei ausgeben
			 * 
			 * @author Mike
			 */
			void print(bool linenumber = false);
			
			/**
			 * @note Logdatei neu erstellen
			 * 
			 * @author Mike
			 */
			void reset(bool verbose = true);
			
			
		};
		
}
#endif // __LOG_HPP__
