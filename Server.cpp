#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "Server.hpp"
#include "Tintin_reporter.hpp"
#include "error.hpp"

#include <sys/wait.h>

/* STATIC VARIABLES ==========================================================*/

unsigned int       Server::_nChild = 0;
std::string const  Server::_SERVERNAME ="matt_daemon";
std::string const  Server::_LOCKFILEDIR = "/var/lock/";
std::string const  Server::_LOCKFILENAME = Server::_LOCKFILEDIR +Server::_SERVERNAME + ".lock";
std::string const  Server::_LOGNAME = "matt_daemon";

/* CONSTRUCTORS ==============================================================*/
Server::Server( void ) :
reporter( new Tintin_reporter(Server::_LOGNAME) ),
_socketMaster(0),
_inetAddr(0),
_port(0)
{
  this->_fdLock = open(Server::_LOCKFILENAME.c_str(), O_CREAT, 0666);
  if (flock(this->_fdLock, LOCK_EX | LOCK_NB)) {
      throw Server::AlreadyRunningException();
  }
  this->reporter = new Tintin_reporter(Server::_SERVERNAME);
  this->reporter->info("Server Initialized");
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

Server::AlreadyRunningException::AlreadyRunningException(){}
Server::AlreadyRunningException::AlreadyRunningException( AlreadyRunningException const &orig){
	(void) orig;
}
Server::SyscallException::SyscallException( void ) throw() {}
Server::SyscallException::SyscallException( SyscallException const & src ) throw() {}

/* MEMBER OPERATORS OVERLOAD =================================================*/
Server    &Server::operator=( Server const & rhs ) {
	(void)rhs;
	return *this;
}


/* DESTRUCTOR ================================================================*/
Server::~Server( void ) {
	flock(this->_fdLock, LOCK_UN);
}
Server::SyscallException::~SyscallException( void ) {}
Server::AlreadyRunningException::~AlreadyRunningException( void ) {}


/* MEMBER FUNCTIONS ==========================================================*/
/*
** Open a connection on port 4242
*/
void
Server::openConnection( void ) {
  struct protoent      *proto;
  struct sockaddr_in   sin;
  // struct timeval       timeout;
  int                  enable;

  enable = 1;
  // timeout.tv_sec = 10;
  // timeout.tv_usec = 0;
  sin.sin_port = htons((unsigned short)DEFAULT_LISTENING_PORT);
  // ntohs(sin.sin_port);
  if ((proto = getprotobyname("tcp")) == 0) {
    this->reporter->error(GPBN_FAIL);
    throw Server::SyscallException();
  }
  if ((this->_socketMaster = socket(PF_INET, SOCK_STREAM, proto->p_proto)) < 0) {
    this->reporter->error(SOCKET_FAILED);
    throw Server::SyscallException();
  }
  if (setsockopt(this->_socketMaster, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
    this->reporter->error(SETSOCKOPT_FAILED);
    throw Server::SyscallException();
  }
  sin.sin_family = AF_INET;
  if ((sin.sin_addr.s_addr = inet_addr(DEFAULT_BINDING_IP)) == INADDR_NONE) {
    this->reporter->error(INET_ADDR_FAILED);
    throw Server::SyscallException();
  }
  if (bind(this->_socketMaster, (const struct sockaddr*)&sin, sizeof(sin)) < 0) {
    this->reporter->error(BIND_FAILED);
    throw Server::SyscallException();
  }
  if (listen(this->_socketMaster, LISTEN_MAX) < 0) {
    this->reporter->error(LISTEN_FAILED);
    throw Server::SyscallException();
  }
  this->_inetAddr = sin.sin_addr.s_addr;
  this->_port = sin.sin_port;
}

int
Server::masterLoop(void) {
  int                 pid;
  struct sockaddr_in  csin;
  unsigned int        cslen;
  int                 newSocket;

  cslen = sizeof(csin);
  while ((newSocket = accept(this->_socketMaster, (struct sockaddr*)&csin, &cslen)))
  {
    if (Server::_nChild < 3) {
      pid = fork();
      if (newSocket > 0 && pid == 0) {
        /* CHILD */
        while (1);
      } else if (newSocket > 0 && pid) {
        /* PARENT */
        Server::_nChild += 1;
        std::cout << "add child, nChild = " << Server::_nChild << '\n';
      }
    } else {
      std::cout << "nope, too many child :D" << '\n';
    }
  }
  close(this->_socketMaster);
  return (0);
}

void
Server::signalHandler( int sig ) {
  std::cout << getpid() << " ";
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
    case SIGCHLD:
    std::cout << LOG_SIGCHLD << '\n';
    Server::_nChild -= Server::_nChild > 0 ? 1 : 0;
    std::cout << "del child, nChild = " << Server::_nChild << '\n';
    exit(0);
    case SIGKILL:
    std::cout << "KILLLL" << '\n';
    exit(0);
    default:
    std::cout << "UNKNOWN" << '\n';
    exit(0);
  }
}

const char*
Server::SyscallException::what( void ) const throw() {
  return ("");
}

const char *
Server::AlreadyRunningException::what() const throw(){
	return ("Cannot acquire exclusive lock on /var/lock/matt_daemon.lock");
}
/* NON MEMBER FUNCTIONS ======================================================*/


/* OPERATOR ==================================================================*/
