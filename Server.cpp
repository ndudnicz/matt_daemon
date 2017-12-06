#include "Server.hpp"
#include "Tintin_reporter.hpp"

#include <iostream>

/* STATIC VARIABLES ==========================================================*/

std::string Server::_SERVERNAME ="matt_daemon";
std::string Server::_LOCKFILEDIR = "/var/lock/";
std::string Server::_LOCKFILENAME = Server::_LOCKFILEDIR +Server::_SERVERNAME + ".lock";

/* CONSTRUCTORS ==============================================================*/
Server::Server( void ) {
/*	struct stat buffer;
	if (stat (Server::_LOCKFILENAME.c_str() , &buffer) == 0) {
	throw Server::AlreadyRunningException();
}*/
	this->_fdLock = open(Server::_LOCKFILENAME.c_str(), O_CREAT, 0666);
	if (flock(this->_fdLock, LOCK_EX | LOCK_NB)) {
			throw Server::AlreadyRunningException();
		}
	this->reporter = new Tintin_reporter(Server::_SERVERNAME);
	this->reporter->info("Server Initialized");
}
Server::Server( Server const & src ) {
}

Server::AlreadyRunningException::AlreadyRunningException(){
}

Server::AlreadyRunningException::AlreadyRunningException( AlreadyRunningException const &orig){
	(void) orig;
}
/* MEMBER OPERATORS OVERLOAD =================================================*/
Server    &Server::operator=( Server const & rhs ) {
	(void)rhs;
	return *this;
}


/* DESTRUCTOR ================================================================*/
Server::~Server( void ) {
	flock(this->_fdLock, LOCK_UN);
}

Server::AlreadyRunningException::~AlreadyRunningException(){
}


/* MEMBER FUNCTIONS ==========================================================*/
void  Server::run(void) {
	std::cout << "ruuuun" << std::endl;
}

const char *Server::AlreadyRunningException::what() const throw(){
	return ("Cannot acquire exclusive lock on /var/lock/matt_daemon.lock");
}
/* NON MEMBER FUNCTIONS ======================================================*/


/* OPERATOR ==================================================================*/
