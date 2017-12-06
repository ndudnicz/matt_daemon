#include "Server.hpp"
#include "Tintin_reporter.hpp"

#include <iostream>

/* STATIC VARIABLES ==========================================================*/


/* CONSTRUCTORS ==============================================================*/
Server::Server( void ) {
	struct stat buffer;   
	  if (stat ("/var/lock/matt_daemon" , &buffer) == 0)
		throw Server::AlreadyRunningException();
	this->reporter = new Tintin_reporter("matt_daemon");
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
}

Server::AlreadyRunningException::~AlreadyRunningException(){
}

const char *Server::AlreadyRunningException::what() const throw(){
return ("The Daemon is already in use");
}

/* MEMBER FUNCTIONS ==========================================================*/
void  Server::run(void) {
  std::cout << "ruuuun" << std::endl;
}

/* NON MEMBER FUNCTIONS ======================================================*/


/* OPERATOR ==================================================================*/
