/*
 * Autor: Mike Moser
 * 
 * Library definitions.h
 * Different definitions
 * 
*/
#ifndef _DEFINITIONS_CALC__
#define _DEFINITIONS_CALC__


#define _sec		0	// Sekunde
#define _min		1	// Minute
#define _hour		2	// Stunde
#define _day		3	// Tag
#define _month		4	// Monat
#define _year		5	// Jahr
#define _st1		6	// Summertime
#define _weekday	7  // Wochentag
#define _yearday	8  // Tage seit Neujahr
#define _kw			9   // Kalenderwoche

#define _y_sec 60*60*24*365 
#define _m_sec
#define _d_sec
// Logdateien
#ifndef _rechner_log 
#define _rechner_log 		"/tmp/.calc.log"
#endif

#endif

