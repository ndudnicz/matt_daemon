#include <iostream>
#include <signal.h>

#include "Server.hpp"
#include "Tintin_reporter.hpp"

/* STATIC VARIABLES ==========================================================*/

std::string const  Server::_LOGNAME = "matt_daemon";


/* CONSTRUCTORS ==============================================================*/
Server::Server( void ) : reporter( new Tintin_reporter(Server::_LOGNAME) ) {
  signal(SIGHUP, &Server::signalHandler);
  signal(SIGINT, &Server::signalHandler);
  signal(SIGQUIT, &Server::signalHandler);
  signal(SIGILL, &Server::signalHandler);
  signal(SIGABRT, &Server::signalHandler);
  signal(SIGFPE, &Server::signalHandler);
  signal(SIGSEGV, &Server::signalHandler);
  signal(SIGPIPE, &Server::signalHandler);
  signal(SIGALRM, &Server::signalHandler);
  signal(SIGTERM, &Server::signalHandler);
  signal(SIGUSR1, &Server::signalHandler);
  signal(SIGUSR2, &Server::signalHandler);
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

void  Server::signalHandler( int sig ) {
  switch (sig) {
    case SIGHUP:
      std::cout << LOG_SIGHUP << '\n';
      exit(0);
    case SIGINT:
      std::cout << LOG_SIGINT << '\n';
      exit(0);
    case SIGQUIT:
      std::cout << LOG_SIGQUIT << '\n';
      exit(0);
    case SIGILL:
      std::cout << LOG_SIGILL << '\n';
      exit(0);
    case SIGABRT:
      std::cout << LOG_SIGABRT << '\n';
      exit(0);
    case SIGFPE:
      std::cout << LOG_SIGFPE << '\n';
      exit(0);
    case SIGSEGV:
      std::cout << LOG_SIGSEGV << '\n';
      exit(0);
    case SIGPIPE:
      std::cout << LOG_SIGPIPE << '\n';
      exit(0);
    case SIGALRM:
      std::cout << LOG_SIGALRM << '\n';
      exit(0);
    case SIGTERM:
      std::cout << LOG_SIGTERM << '\n';
      exit(0);
    case SIGUSR1:
      std::cout << LOG_SIGUSR1 << '\n';
      exit(0);
    case SIGUSR2:
      std::cout << LOG_SIGUSR2 << '\n';
      exit(0);
    default:
      std::cout << "UNKNOWN" << '\n';
      exit(0);
  }
}

/* NON MEMBER FUNCTIONS ======================================================*/


/* OPERATOR ==================================================================*/
