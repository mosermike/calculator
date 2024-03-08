/**
 * @author Mike Moser
 * 
 * @name functions.cpp
 * Enthält verschiedene Funktionen
 * 
*/

#include <iostream>
#include <string>
#include <vector>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <bits/local_lim.h>
#include <unistd.h> // getlogin_r
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sys/types.h>
#include <time.h>
#include <utime.h>
#include <filesystem>

#include "../include/math.hpp"
#include "../include/file.hpp"
#include "../include/Unix.hpp"

uid_t math::Unix::getUserIdByName(const char *name)
{
    struct passwd *pwd = getpwnam(name); /* don't free, see getpwnam() for details */
    if(pwd == NULL) {
		std::cout << "Failed to get userId from groupname : " <<  name;
	} 
    return pwd->pw_uid;
}

gid_t math::Unix::getGroupIdByName(const char *name)
{
    struct group *grp = getgrnam(name); /* don't free, see getgrnam() for details */
    if(grp == NULL) {
        std::cout << "Failed to get groupId from groupname : " <<  name;
    } 
    return grp->gr_gid;
	
}

uid_t math::Unix::getFileUID (const char* file){
	struct stat sb;
	
	if (stat(file, &sb) == -1) {
		perror("stat");
		exit(EXIT_FAILURE);
	}
	return sb.st_uid;
}

bool math::Unix::exist(std::string path){
	struct stat sb;
	
	if (stat(path.c_str(), &sb) == -1)
		return false;
	return true;
}

bool math::Unix::exist(std::string path1, std::string path2) {
	return exist(path1) && exist(path2);
}

bool math::Unix::exist(std::string path1, std::string path2, std::string path3) {
	return exist(path1) && exist(path2) && exist(path3);
}

int32_t math::Unix::filetype(std::string filepath){
	struct stat sb;
	
	if (stat(filepath.c_str(), &sb) == -1) {
		//perror("stat");
		std::cout << "[fileType] No such file or directory (" << filepath << ")" << std::endl;
		exit(EXIT_FAILURE);
	}

    switch (sb.st_mode & S_IFMT) {
		case S_IFBLK:	return 6;		// Gerät oder Partition
		case S_IFCHR:	return 2;		// Maus, seriell angeschlossene Geräte, etc.
		case S_IFDIR:	return 4;			// Ordner
		case S_IFIFO:	return 1;
		case S_IFLNK:	return 10;		// symbolischer Link
		case S_IFREG:	return 8;			// normale Datei
		case S_IFSOCK:	return 12;  		// Socket
		default:		return EXIT_FAILURE;	// nicht bekannt
    }
}

char* math::Unix::getHost () {
	char hostname[HOST_NAME_MAX];
	char* return_name = new char[HOST_NAME_MAX];
	
	gethostname(hostname, HOST_NAME_MAX);
	
	strcpy(return_name,hostname);
	
	return return_name;
}

math::string math::Unix::get_process_name_by_pid(const pid_t pid){
	std::ifstream	process			((std::string)"/proc/" + std::to_string(pid) + "/comm");
	std::string		process_name	= "";
	
	std::getline(process,process_name);
	
	return process_name;
}

pid_t math::Unix::get_pid_by_process_name(const std::string process, int start) {
	
	for( pid_t i = start; i < 10000; i++) {
		if(math::Unix::exist((std::string)"/proc/" + std::to_string(i))) {
			std::ifstream	process1			((std::string)"/proc/" + std::to_string(i) + "/comm");
			std::string		process_name	= "";
			std::getline(process1,process_name);
			
			// process found?
			if(process_name == process)
				return i;
		}
	}
	
	math::shell::warn("[PID] keine PID gefunden");
	return 0;
}

bool math::Unix::mkdir_p(std::string value) {
	math::string make_dir = "";	// string to create the directories
	math::string temp=value;					// Save the value to create directories step by step
	std::size_t pos;			// Position of '/'
	pos = temp.find("/",1);

	// Check if the directory exists already to save computing time
	if (math::Unix::exist(value) && math::Unix::filetype(value) == 4)
		return 0;

	do{
		make_dir += temp.substr(0,pos);
		temp = temp.substr(pos,temp.size()-pos);

		// Check if the dir exists as a non-directory, otherwise create it
		if(math::Unix::exist(make_dir.c_str()) && math::Unix::filetype(make_dir.str()) != 4)
			math::shell::error("[mkdir_p] Cannot create directory " + make_dir.str() + " as it already exists as a non-directory!");
		else
			mkdir(make_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

		// Determine new position for the new directory if not at the end
		pos = temp.find("/",1);
	} while(pos < std::string::npos);

	// Now create the last directory
	if(!math::Unix::exist(value))
				mkdir(value.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

	return math::Unix::exist(value);
	
}

math::string math::Unix::perms(std::string path) {
	struct stat sb;
	stat(path.c_str(), &sb);	// Zuordnung der Infos
	int number = sb.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
	
	// convert decimal to octal
	int temp; 
	int base = 8;
    int octal = 0;
	int i = 1;
	
    while(number != 0) {
        temp = number % base;
        number = number / base;
        octal = octal + (temp*i);
        i = i*10;
    }
	
	return math::to_string(octal);
}

bool math::Unix::perm_to_write(std::string path) {
	math::Unix::User user;
	
	if(user.get_user() == "root")
		return true;
	
	math::string perm = perms(path);

	struct stat sb;
	stat(path.c_str(), &sb);	// Zuordnung der Infos
	
	if(sb.st_uid == user.get_uid()) {
		if(perm[1] == '2' || perm[1] == '3' || perm[1] == '6' || perm[1] == '7')
			return true;
	}
	else if(sb.st_uid != user.get_uid()) {
		if(perm[2] == '2' || perm[2] == '3' || perm[2] == '6' || perm[2] == '7')
			return true;
	}
	
	return false;
	
}

void math::Unix::remove(std::string file) {
	std::filesystem::remove_all(file);
}

bool math::Unix::set_date(std::string src_file, std::string dst_file, bool verbose) {
	
	struct stat			src;
	struct stat			dst;
	time_t				mtime;
	time_t				atime;
	time_t				mtime_new;
	struct	utimbuf		new_times;

	stat(src_file.c_str(), &src);
	mtime = src.st_mtime; // Sekunde seit 1970 // mtime nicht ctime!
	atime = src.st_atime; // Sekdunde seit 1970

	// Check if the times are already the same
	stat(dst_file.c_str(), &dst);
	if (mtime == dst.st_mtime && atime == dst.st_atime) {
		return true;
	}
	
	// Zeiten in struct schreiben:
	new_times.actime = atime;		// Zeit von letzter Zugriff
	new_times.modtime = mtime;		// Zeit von letzter Statusänderung
	
	// Zeiten schreiben in neue Datei:
	utime(dst_file.c_str(), &new_times);
	stat(dst_file.c_str(), &dst);
	mtime_new = dst.st_mtime;

	if(verbose)
		std::cout << "Change time of " << dst_file << " to time of " << src_file << std::endl;
	
	return mtime_new == mtime ? true : false;
}

