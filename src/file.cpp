/**
 * @author Mike Moser
 * 
 * @file: file.cpp
 * Enthält verschiedene Funktionen für Dateien
 * 
*/
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/stat.h>	// struct stat
#include <dirent.h>
#include <bits/local_lim.h>	// Limits für Hostname und Loginname
#include <string.h>
#include <functional>	// std::function
#include <sys/ioctl.h>	// winsize
#include <iomanip>      // std::setfill, std::setw
#include <thread>
#include <limits.h>
#include <filesystem>

#include "../include/math.hpp"
#include "../include/file.hpp"

math::string math_actual_file = "";
bool math_progressing = false;




std::size_t math::file::num_lines(std::string path) {
	
	int num = 0;
	std::ifstream input(path.c_str());
	
	while(!input.eof()) {
		input.ignore( INT_MAX, '\n' );
		num++;
	}
	
	return num;
}


std::size_t math::file::size(const std::string& filename) {
	// if file is not a file  or does not exists => return 0
    if((!math::Unix::exist(filename)) || (math::Unix::filetype(filename) != 8))
        return 0;
    
    std::ifstream ifile(filename, std::ifstream::ate | std::ifstream::binary);

    return ifile.tellg();    
}
