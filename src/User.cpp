/**
 * @author Mike Moser
 * 
 * Quelldatei User.cpp
 * 
 * Enthält verschiedene Funktionen für die Klasse User
 * 
*/
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <string>
#include "../include/math.hpp"
#include <string.h>
#include <fstream>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <locale>	// for getting language of the user

#include "Unix.hpp"


bool math::Unix::User::operator==(const std::string testuser){
 	if( this->username.str() == testuser)
		return true;
	return false;
}

bool math::Unix::User::operator==(uid_t testuid){
	if( this->uid == testuid)
		return true;
	return false;
}

bool math::Unix::User::operator!=(const std::string testuser){
	if((this->username).str() != testuser)
		return true;
	return false;
	
}
bool math::Unix::User::operator!=(uid_t testuid){
	if( this->uid != testuid)
		return true;
	return false;
}



std::string math::Unix::User::geteuser() {
	return std::string(pw->pw_name);
}
std::string math::Unix::User::gethomedir() {
	return std::string(pw->pw_dir);
}

std::string math::Unix::User::getip(std::string iface ) {
	
	int		n		= 0;
	struct	ifreq	ifr;
    
	n = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name , iface.c_str() , IFNAMSIZ - 1);
		ioctl(n, SIOCGIFADDR, &ifr);
	close(n);
    
	return  inet_ntoa(( (struct sockaddr_in *) & ifr.ifr_addr )->sin_addr);
}

std::string math::Unix::User::getdefaultshell() {
	return std::string(pw->pw_shell);
}

std::string math::Unix::User::getdomain() {

	math::string temp;
	temp = temp.getline("/etc/resolv.conf",2);
	temp = temp.replace("domain ", "");
	
	return temp.str();
}

uid_t math::Unix::User::getloginuid() {
	const passwd *pw1 = getpwnam(getlogin());
	
	if(pw1 == NULL)
		math::shell::error("ERROR: user function getloginuid @ User.cpp line 101");
	
	return pw1->pw_uid;
}

gid_t math::Unix::User::getlogingid() {
	const passwd *pw1 = getpwnam(getlogin());
	
	if(pw1 == NULL)
		math::shell::error("ERROR: user function getlogingid @ User.cpp line 110");
	
	return pw1->pw_gid;    
}

std::string math::Unix::User::getlocale() {
	return std::locale("").name().c_str();
}

std::string math::Unix::User::getloginname() {
	return (this->loginname).str();
}

std::vector<gid_t> math::Unix::User::getgrgid(){
	
	std::vector <gid_t> gids_temp (math::file::num_lines("/etc/group"));
	gid_t* groups_gid = gids_temp.data();
		
	if(groups_gid == NULL)
		math::shell::error("ERROR: user function getgrgid @ User.cpp line 118");
	
	if ((getgrouplist(geteuser().c_str(), uid, groups_gid, &ngroups)) == -1)
		getgrouplist(geteuser().c_str(), uid, groups_gid, &ngroups);
	
	return gids_temp;
}


const std::vector<math::string> math::Unix::User::getgrlist()
{
	
	struct group *gr;
	std::vector<math::string>	groups;
			
	for ( int j = 0; j < ngroups; j++) {
		gr = ::getgrgid(this->gids[j]);
		if (gr != NULL)
			groups.push_back((math::string) gr->gr_name);
	}
	
	return groups;
}

std::string math::Unix::User::gethost(){
	std::string temp;
	char hostname[HOST_NAME_MAX];
		
	gethostname(hostname, HOST_NAME_MAX);
	
	temp = hostname;
	
	return temp;	
}

math::string math::Unix::User::setPWD() {
	this->pwd = get_current_dir_name();
	return this->pwd;
}

void math::Unix::User::cout(math::string output) {

	switch(output.hash()) {
		case math::functions::const_string_hash("uid") :
			std::cout << "UID: \t\t\t"		<< this->uid << std::endl;
			break;
			
		case math::functions::const_string_hash("login_uid") :
			std::cout << "LOGIN_UID: \t\t"	<< this->login_uid << std::endl;
			break;
			
		case math::functions::const_string_hash("gid") :
			std::cout << "GID: \t\t\t"		<< this->gid << std::endl;
			break;
			
		case math::functions::const_string_hash("login_gid") :
			std::cout << "LOGIN_GID \t\t"		<< this->login_gid << std::endl;
			break;
			
		case math::functions::const_string_hash("username") :
			std::cout << "USERNAME: \t\t"		<< this->username<< std::endl;
			break;
			
		case math::functions::const_string_hash("loginname") :
			std::cout << "LOGINNAME: \t\t"	<< this->loginname << std::endl;
			break;
			
		case math::functions::const_string_hash("home") :
			std::cout << "HOME: \t\t\t"		<< this->home << std::endl;
			break;
			
		case math::functions::const_string_hash("defaultshell") :
			std::cout << "SHELL: \t\t\t"		<< this->defaultshell << std::endl;
			break;
		
		case math::functions::const_string_hash("host") :
			std::cout << "HOSTNAME: \t\t" 	<< this->hostname << std::endl;
			break;
			
		case math::functions::const_string_hash("hostname") :
			std::cout << "HOSTNAME: \t\t" 	<< this->hostname << std::endl;
			break;
			
		case math::functions::const_string_hash("iw") :
			std::cout << "WLAN IP-ADRESSE: \t" 	<< this->ip_w << std::endl;
			break;
		
		case math::functions::const_string_hash("ie") :
			std::cout << "LAN IP-ADRESSE: \t" 	<< this->ip_e << std::endl;
			break;
			
		case math::functions::const_string_hash("domain") :
			std::cout << "DOMAIN: \t\t" 	<< this->domain << std::endl;
			break;
		
		case math::functions::const_string_hash("path") :
			std::cout << "PATH: \t\t\t" 	<< this->path << std::endl;
			break;
			
		case math::functions::const_string_hash("pwd") :
			std::cout << "PWD: \t\t\t"		<< this->pwd << std::endl;
			break;
			
		case math::functions::const_string_hash("groups") :
			std::cout << "GROUPS: \t\t";
			for(uint32_t i = 0; i < this->groups.size(); i++)
				std::cout << this->gids[i] << " (" << this->groups[i] << ") ";
			std::cout << std::endl;
			break;

		default:
			std::cout << "Wrong Parameter"	<< std::endl;
			break;
		
	}
	
	
}
bool math::Unix::User::check(math::string option, int value) {
	return check(option,std::to_string(value));
}
bool math::Unix::User::check(math::string option, std::string value) {
	
	std::vector<std::string> paths;
	std::string path_str = this->path.str();
	std:: string temp = "";
	
	switch(option.hash()) {
		case math::functions::const_string_hash("uid") :
			return std::to_string(this->uid) == value ? true : false;
			break;
			
		case math::functions::const_string_hash("login_uid") :
			return std::to_string(this->login_uid) == value ? true : false;
			break;
			
		case math::functions::const_string_hash("gid") :
			return std::to_string(this->gid) == value ? true : false;
			break;
			
		case math::functions::const_string_hash("login_gid") :
			return std::to_string(this->login_gid) == value ? true : false;
			break;
			
		case math::functions::const_string_hash("username") :
			return this->username == value ? true : false;
			break;
			
		case math::functions::const_string_hash("loginname") :
			return this->loginname == value ? true : false;
			break;
			
		case math::functions::const_string_hash("home") :
			return this->home == value ? true : false;
			break;
			 
		case math::functions::const_string_hash("defaultshell") :
			return this->defaultshell == value ? true : false;
			break;
		
		case math::functions::const_string_hash("host") :
			return this->hostname == value ? true : false;
			break;
			
		case math::functions::const_string_hash("hostname") :
			return this->hostname == value ? true : false;
			break;
			
		case math::functions::const_string_hash("iw") :
			return this->ip_w == value ? true : false;
			break;
			
		case math::functions::const_string_hash("ie") :
			return this->ip_e == value ? true : false;
			break;
			
		case math::functions::const_string_hash("domain") :
			return this->domain == value ? true : false;
			break;
		
		case math::functions::const_string_hash("pwd") :
			return this->pwd == value ? true : false;
			break;
			
		case math::functions::const_string_hash("path") :

			
			for(uint32_t i = 0; i < path_str.size(); i++) {
				if(path_str[i] == ':') {
					paths.push_back(path_str.substr(0,i));
					path_str = path_str.substr(i+1);
					i = 0;
				}
				
			}
			for(uint32_t i = 0; i < paths.size(); i++) {
				if(paths[i] == value)
					return true;
			}
			return false;
			
			break;
		
		case math::functions::const_string_hash("groups") :
			
			for(uint32_t i = 0; i < this->groups.size(); i++) {
				if(this->groups[i] == value)
					return true;
			}
			return false;
			break;

		default:
			std::cout << "Wrong Parameter"	<< std::endl;
			break;
		
	}
	return false;
}
