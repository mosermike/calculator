/**
 * @author Mike Moser
 * 
 * @file string.cpp
 * Enthält verschiedene Funktionen
 * 
*/ 

#include "../include/standards.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <shell.hpp>
#include <sys/stat.h>
#include <stdlib.h>	//atoi()
#include <math.hpp>
#include <string.h>
#include <fstream>
#include <sstream>
#include <Unix.hpp>
#include <filesystem> // for ls function

bool math::string::operator==(const std::string test){
	return (this->value == test) ? true : false;
}

bool math::string::operator==(math::string test){
	return (this->value == test.str()) ? true : false;
}

bool math::string::operator==(const char* test){
	return (this->value == test) ? true : false;
}

bool math::string::operator!=(const std::string test){
	if(this->value != test)
		return true;
	return false;
}

bool math::string::operator!=(math::string test){
	return (this->value != test.str()) ? true : false;
}

bool math::string::operator!=(const char* test){
	return (this->value != test) ? true : false;
}

char &math::string::operator[](int index){		// bestimmte Position ausgeben
		if (index < 0)
			index = value.size() + index;
		
		return value[index];
}

math::string math::string::operator+(const char* add) {
	return (value + add);
}

math::string math::string::operator+(const std::string add) {
	return (this->value + add);
}
math::string math::string::operator+(math::string add) {
	return (value + add.str());
}

math::string math::string::operator+=(const char* add) {
	this->value = value + add;
	return (value + add);
}

math::string math::string::operator+=(const std::string add) {
	this->value = value + add;
	return (value + add);
}
math::string math::string::operator+=(math::string add) {
	this->value = value + add.str();
	return (value + add.str());
}
std::string &math::string::operator()(){		// value zurückgeben, wenn in Klammern
	return value;
}

double math::string::atof() {
	
	
	return std::atof(this->value.c_str());
}

int math::string::atoi() {
	return std::atoi(this->value.c_str());
}
// Funktionen:

std::size_t math::string::find_back(math::string name, std::size_t pos, std::size_t low) {
	// pos is defined as the position where to start. If pos is too close to the size of the string
	// it should be reset
	if(pos == 0 || (value.size() - name.size()) < pos)
		pos = this->value.size() - name.size() - pos;
	for(std::size_t i = pos; i >= low; i--) {
		if(value.substr(i,name.size()) == name.str())
			return i;
	}
	return std::string::npos;
}

void math::string::log(std::string logpath) {
	
	math::Unix::User user;
	std::size_t		folder = 0;
	
	if(!math::Unix::exist(logpath)) {
		// Ordner der Logdatei bestimmen
		for(uint32_t i = logpath.size(); i > 0; i--) {
			if(logpath[i] == '/') {
				folder = i;
				break;
			}
		}
		// Überprüfen der vorhandenen Berechtigung für den Zielpfad
		if(!user.check("uid",0) && !user.check("uid",math::Unix::getFileUID(logpath.substr(0,folder).c_str()))) {
		
			std::cout << "log: Die Erstellung der Logdatei '" << logpath << "' ist nicht möglich: Keine Berechtigung" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	
	std::ofstream output;
	output.open(logpath.c_str(),std::ios::out | std::ios::app);   // mit ans Ende springen
	output << value << std::endl;
}

std::string math::string::lower(){
	for(uint16_t i = 0; i < (uint16_t) value.size(); i++){
			temp[i] = tolower(value[i]);
	}
	return temp;
}

std::vector<std::string> math::string::ls(math::shell::arg args, std::string name_include, std::string name_exclude, std::vector<std::string> &directories) {
	std::vector<std::string> 	LS;
	math::string 				temp1;
	math::string 				temp2; // for name_include used
	bool					save_dirs = false; // Save directories separately

	// Check if the vector is actually defined as a parameter => otherwise do not change it to save memory.
	if (directories.size() > 0) {
		if(directories[0] != "---")
			save_dirs = true;
	}
	else
		save_dirs = true;

	std::string path = this->value;

	// Add ./ to make it more clear, if needed
	if (path[0] != '/' && path.substr(0,2) != "./" && path.substr(0,3) != "../")
		path = "./" + path;

	// If path does not exists => Print error
	if (!math::Unix::exist(path)) {
		math::shell::error("[ls] Given string does not exist!");
	}

	// If file is not a directory, only add path and return it (it must exists as checked before)
	if (math::Unix::filetype(path) != 4) {
		LS.push_back(path);
		return LS;
	}

	// Go through the directory and add the desired directories and files
	for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{path}) {
		// Remove leading and ending '"'
		temp1 = math::to_string(dir_entry).sub(1,-2);

		// Don't add files recursively
		if (args.notArg("-r","--recursive") && temp1.substr(path.size()+2).exist("/"))
			continue;
		// Don't add hidden files
		if (args.notArg("-a","--all") && (temp1.exist("/.")))
			continue;
		// Exclude a string
		if (name_exclude != "" && temp1.exist(name_exclude))
			continue;
		// Object is a directory
		if (math::Unix::filetype(temp1.str()) == 4) {
			temp1 += "/";
			if (save_dirs)
				directories.push_back(temp1.str());
		}
		// If directory in path does not contain include => skip everything
		temp2 = temp1.replace(path,"");
		if (temp2.substr(0,temp2.find('/')).exist(name_include))
			LS.push_back(temp1.str());
	}

	return LS;
}

std::vector<std::string> math::string::ls(math::shell::arg args, std::string name_include, std::string name_exclude) {
	std::vector<std::string> directories = {"---"};
	std::vector<std::string> LS = ls(args, name_include, name_exclude, directories);
	return LS;
}

std::string math::string::replace (char sign_old, char sign_new, bool save, std::size_t pos, bool all){
	return replace(math::to_string(sign_old), math::to_string(sign_new), pos, save, all);
}

std::string math::string::replace(math::string str_old, math::string str_new, bool save, std::size_t pos, bool all){
	if(all) {
		math::string temp1 = this->value;
		std::size_t temp2 = temp1.size();

		// Go through the string and replace all appearances once
		for (uint32_t i = 0; i < temp2; i++) {
			if(temp1.substr(i).exist(str_old.str())) {
				temp1 = temp1.replace_intern(str_old, str_new,i,true);
				temp2 = temp1.size();
				i = i + str_new.size();
			}

		}

		if(save)
			this->value = temp1.str();
		return temp1.str();
	}
	else
		return replace_intern(str_old, str_new, pos, save);
}

std::string math::string::replace_intern(math::string str_old, math::string str_new, std::size_t pos, bool save){
	std::string temp = this->value;
	pos = temp.find(str_old.str(),pos);
	temp = temp.substr(0,pos) + str_new.str() + temp.substr(pos+str_old.size());
	if(save)
		this->value = temp;
	
	return temp;
}

std::string math::string::replace(std::string str_old1, std::string str_new1,std::string str_old2, std::string str_new2, bool save, std::size_t pos, bool all) {
	
	math::string temp1 = this->value; // Store value in new string
	
	// @note Replace strings if exist
	temp1.replace(math::to_string(str_old1), math::to_string(str_new1), true, pos, all);
	temp1.replace(math::to_string(str_old2), math::to_string(str_new2), true, pos, all);
	
	// @note Save in this class
	if(save)
		this->value = temp1.str();
	return temp1.str();
	
}
	
math::string math::string::substr(std::size_t pos1, std::size_t length1){
	return value.substr(pos1,length1);
}

math::string math::string::substr(std::size_t pos1){
	
	return value.substr(pos1);
}

math::string math::string::sub(std::size_t beg, int32_t end) {
	if (end < 0)
		end = value.size() + end;
	return value.substr(beg,end-(beg-1));
}

bool math::string::mkdir_p() {

	std::string make_dir = "";	// string to create the directories
	temp=value;					// Save the value to create directories step by step
	std::size_t pos;			// Position of '/'
	pos = temp.find("/",1);
	
	// Check if the directory exists already to save computing time
	if (math::Unix::exist(value) && math::Unix::filetype(value) == 4)
		return 0;
	
	do{
		make_dir += temp.substr(0,pos);
		temp = temp.substr(pos,temp.size()-pos);

		// Check if the dir exists as a non-directory, otherwise create it
		if(math::Unix::exist(make_dir.c_str()) && math::Unix::filetype( make_dir ) != 4)
			math::shell::error("[mkdir_p] Cannot create directory " + make_dir + " as it already exists as a non-directory!");
		else
			mkdir(make_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

		// Determine new position for the new directory if not at the end
		pos = temp.find("/",1);
	} while(pos < std::string::npos);

	// Now create the last directory
	if(!math::Unix::exist(value.c_str()))
				mkdir(value.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

	return 0;
}

bool math::string::remove() {
	return math::Unix::filetype(this->value) == 8 ? std::remove(this->value.c_str()) : false;
	
}


std::string math::string::getValue() {
	return this->value;
}

std::size_t math::string::size() {
	return value.size();
}

std::string math::string::str() {
	return this->value;
}

void math::string::writeline(std::ofstream& output) {
	output << this->value << std::endl;
}

void math::string::writeline(std::string file) {
	std::ofstream output(file);
	output << this->value << std::endl;
	output.close();
}

const char* math::string::c_str() {
	return this->value.c_str();
}

std::string math::string::setString(std::string new_value) {
	this->value = new_value;
	return this->value;
}

int math::string::stoi() {
	return std::stoi(this->value);
}

uint32_t math::string::hash() {
	return functions::const_string_hash(this->value.c_str());
}

/*
std::string math::string::getline(std::ifstream& input) {
	std::getline(input,this->value);
	return this->value;
}*/

void math::string::getline(std::ifstream& input) {
	std::getline(input,this->value);
}


std::string math::string::getline(std::string input) {
	
	std::ifstream input1(input);
	std::getline(input1,value);
	input1.close();
	
	return value;
}

std::string math::string::getline(std::string input, uint32_t line){
	std::ifstream input1(input);
	std::string del;
	for(uint32_t i = 0; i < line - 1; i++)
		std::getline(input1,del);
	std::getline(input1,value);
	input1.close();
	
	return value;
}

uint32_t math::string::count(math::string name) {
	uint32_t counts = 0;
	std::size_t pos = value.find(name.str());
	if(range(pos)) {
			counts++;
	}
	do {
		pos = value.find(name.str(),pos+1);
		if(range(pos)) {
			counts++;
		}
		
	} while(pos < name.size());
	
	return counts;
}
void math::string::cout(bool newline, std::string text) {
	std::cout << text << this->value;
	if(newline)
		std::cout << std::endl;
	return;
}
std::string math::string::del_letter(bool save) {
	std::string temp = "";
	for(uint32_t i = 0; i < this->value.length(); i++) {
		if(value[i] < 65 || value[i] > 122)
			temp += value[i];
	}
	if(save)
		this->value = temp;
	return temp;
}

std::string math::string::del_number(bool save) {
	std::string temp = "";
	for(uint32_t i = 0; i < this->value.length(); i++) {
		if(value[i] < 48 || value[i] > 57)
			temp += value[i];
	}
	if(save)
		this->value = temp;
	return temp;
}

std::string math::string::del_space(bool save) {
	std::string temp = "";
	for(uint32_t i = 0; i < this->value.length(); i++) {
		temp += value[i];
		if(value[i] == ' ')
			break;
	}
	if(save)
		this->value = temp;
	return temp;
}

bool math::string::exist(std::string search) {
	return value.find(search) < std::string::npos ? true : false;
}

bool math::string::exist(std::string name1, std::string name2) {
	return exist(name1) || exist(name2);
}

bool math::string::exist(std::string name1, std::string name2, std::string name3) {
	return exist(name1) || exist(name2) || exist(name3);
}

bool math::string::exist(std::string name1, std::string name2, std::string name3, std::string name4) {
	return exist(name1) || exist(name2) || exist(name3) || exist(name4);
}

