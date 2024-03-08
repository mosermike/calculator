/**
 * @author Mike Moser
 * 
 * @name standards.cpp
 * Enthält verschiedene Standardfunktionen
 * 
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unistd.h>		// fsync(),stdin_fileno,getpass
#include <definitions.hpp>
#include <algorithm>	// std::replace
#include <sys/mount.h>
#include <signal.h>

#include "../include/math.hpp"
#include "../include/standards.hpp"


// Uhrzeit:
uint32_t math::date(int info, time_t time1){
	
	std::tm*		nun;
	if(time1 == -1) {
		std::time_t t = std::time(0);   // get time now
		nun = std::localtime(&t);
	}
    else
		nun = std::gmtime(&time1);
	
	constexpr int DAYS_PER_WEEK = 7 ;

    const int wday = nun->tm_wday ;
    const int delta = wday ? wday-1 : DAYS_PER_WEEK-1 ;
    
	
	switch(info){
		case _sec:  return (uint32_t) nun->tm_sec;
		case _min:  return (uint32_t) nun->tm_min;	// Minute
		case _hour:  return (uint32_t) nun->tm_hour;	// Stunde
		case _day:  return (uint32_t) nun->tm_mday;	// Tag
		case _month:  return (uint32_t) nun->tm_mon+1;	// Monat
		case _year:  return (uint32_t) nun->tm_year+1900;	// Jahr
		case _st1:  return (uint32_t) nun->tm_isdst;	// Summertime
		case _weekday:  return (uint32_t) nun->tm_wday;	// Tage seit Sonntag
		case _yearday:  return (uint32_t) nun->tm_yday;	// Tage seit Neujahr
		case _kw:  return (uint32_t) (nun->tm_yday + DAYS_PER_WEEK - delta ) / DAYS_PER_WEEK;	// week number
		default: return 0;
	}
}

std::string math::date(math::string value, std::string trennzeichen1, std::string trennzeichen2) {


	std::string year = std::to_string(math::date(_year)); // Get actual year
	// Da ansonsten zum Beispiel 2 anstatt 02 ausgegeben wird
	// Werte bilden
	uint32_t info = _month;
		std::string month = math::date(info) > 9		?	std::to_string(math::date(info)) : "0" + std::to_string(math::date(info));
	info = _day;
		std::string day = math::date(info) > 9 		?	std::to_string(math::date(info)) : "0" + std::to_string(math::date(info));
	info = _hour;
		std::string hour = math::date(info) > 9		?	std::to_string(math::date(info)) : "0" + std::to_string(math::date(info));
	info = _min;
		std::string minute = math::date(info) > 9	?	std::to_string(math::date(info)) : "0" + std::to_string(math::date(info));
	info = _sec;
		std::string second = math::date(info) > 9	?	std::to_string(math::date(info)) : "0" + std::to_string(math::date(info));	

	switch(value.hash()) {
		
		case math::functions::const_string_hash("Datum") :	// YY.MM.DD
			return year + trennzeichen2 + month + trennzeichen2 + day;
		
		case math::functions::const_string_hash("Uhrzeit") :	// HH:MM:SS
			return hour + trennzeichen1 + minute + trennzeichen1 + second; 
		
		case math::functions::const_string_hash("Datum+Uhrzeit") :	// DD MM YYYY HH:MM:SS
			return day + trennzeichen2 + month + trennzeichen2 + year + " " + hour + trennzeichen1 + minute + trennzeichen1 + second;
		
		case math::functions::const_string_hash("Datum1+Uhrzeit") :	// YYYY MM DD HH:MM:SS
			return year + trennzeichen2 + month + trennzeichen2 + day + " " + hour + trennzeichen1 + minute + trennzeichen1 + second; 
		
		default:
			return year + trennzeichen2 + month + trennzeichen2 + day;
	}
}

bool math::range(std::size_t number) {
	return number < std::string::npos ? true : false;
}

bool math::range(std::size_t number1, std::size_t number2) {
	return range(number1) || range(number2);
}
