#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "Server.hpp"
#include "Tintin_reporter.hpp"

/* STATIC VARIABLES ==========================================================*/

std::string const  Server::_LOGNAME = "matt_daemon";


/* CONSTRUCTORS ==============================================================*/
Server::Server( void ) :
reporter( new Tintin_reporter(Server::_LOGNAME) ),
_socketMaster(0),
_inetAddr(0),
_port(0)
{
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
  signal(SIGCHLD, &Server::signalHandler);
  signal(SIGCONT, &Server::signalHandler);
  signal(SIGTSTP, &Server::signalHandler);
  signal(SIGTTIN, &Server::signalHandler);
  signal(SIGTTOU, &Server::signalHandler);
  signal(SIGKILL, &Server::signalHandler);
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
int
Server::openConnection( void ) {
  struct protoent      *proto;
  struct sockaddr_in   sin;
  struct timeval       timeout;
  int                  enable;

  enable = 1;
  timeout.tv_sec = 10;
  timeout.tv_usec = 0;
  sin.sin_port = htons((unsigned short)DEFAULT_LISTENING_PORT);
  ntohs(sin.sin_port));
  if ((proto = getprotobyname("tcp")) == 0) {
    return (ft_error("Serveur", "", GPBN_FAIL, 1));
  }
  if ((this->_socketMaster = socket(PF_INET, SOCK_STREAM, proto->p_proto)) < 0) {
    // return (ft_error("Serveur", "open_connection:", SOCKET_FAILED, 1));
  }
  // if (setsockopt(this->socketMaster, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
  //   return (ft_error("ERROR", "my_setsockopt()", SETSOCKOPT, 1));
  // }
  // if (setsockopt(this->socketMaster, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
  //   return (ft_error("ERROR", "my_setsockopt()", SETSOCKOPT, 1));
  // }
  if (setsockopt(this->socketMaster, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
    // return (ft_error("ERROR", "my_setsockopt()", SETSOCKOPT, 1));
  }
  sin.sin_family = AF_INET;
  if ((sin.sin_addr.s_addr = inet_addr(DEFAULT_BINDING_IP)) == INADDR_NONE) {
    // return (ft_error("Serveur", "open_connection:", INET_ADDR_FAILED, 1));
  }
  if (bind(this->socketMaster, (const struct sockaddr*)&sin, sizeof(sin)) < 0) {
    // return (ft_error("Serveur", "open_connection:", CONNECT_ERROR, 1));
  }
  if (listen(this->socketMaster, LISTEN_MAX) < 0) {
    // return (ft_error("Serveur", "open_connection:", BIND_ERROR, 1));
  }
  this->inet_addr = sin.sin_addr.s_addr;
  this->port = sin.sin_port;
  return (0);
}

void
Server::masterLoop(void) {
  /*
  ** Open a connection on port 4242
  */

}

void
Server::signalHandler( int sig ) {
  int const  ppid = getppid();

  switch (sig) {
    case SIGHUP:
    std::cout << LOG_SIGHUP << '\n';
    exit(0);
    case SIGINT:
    std::cout << LOG_SIGINT << " pid:" << getpid() << '\n';
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
    case SIGKILL:
    std::cout << "KILLLL" << '\n';
    exit(0);
    default:
    std::cout << "UNKNOWN" << '\n';
    exit(0);
  }
}

/* NON MEMBER FUNCTIONS ======================================================*/


/* OPERATOR ==================================================================*/
