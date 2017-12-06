#include "Server.hpp"
#include "Tintin_reporter.hpp"

#include <iostream>

/* STATIC VARIABLES ==========================================================*/


/* CONSTRUCTORS ==============================================================*/
Server::Server( void ) {
	this->reporter = new Tintin_reporter("matt_daemon");
	this->reporter->info("Server Initialized");
}
Server::Server( Server const & src ) {
}

/* MEMBER OPERATORS OVERLOAD =================================================*/
Server    &Server::operator=( Server const & rhs ) {
}


/* DESTRUCTOR ================================================================*/
Server::~Server( void ) {
}

/* MEMBER FUNCTIONS ==========================================================*/
void  Server::run(void) {
  std::cout << "ruuuun" << std::endl;
}

/* NON MEMBER FUNCTIONS ======================================================*/


/* OPERATOR ==================================================================*/