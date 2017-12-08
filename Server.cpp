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

unsigned int		Server::_nChild = 0;
std::string const	Server::_SERVERNAME ="matt_daemon";
std::string const	Server::_LOCKFILEDIR = "/var/lock/";
std::string const	Server::_LOCKFILENAME = Server::_LOCKFILEDIR +Server::_SERVERNAME + ".lock";
std::string const	Server::_LOGNAME = "matt_daemon";
std::list<int>		*Server::_pidArray = new std::list<int>;

/* CONSTRUCTORS ==============================================================*/
Server::Server( void ) :
reporter( new Tintin_reporter(Server::_LOGNAME) ),
_socketMaster(0),
_inetAddr(0),
_port(0)
// _pidArray( new std::list<int> )
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
	signal(SIGBUS, &Server::signalHandler);
	signal(SIGSTKFLT, &Server::signalHandler);
	signal(SIGURG, &Server::signalHandler);
	signal(SIGXCPU, &Server::signalHandler);
	signal(SIGXFSZ, &Server::signalHandler);
	signal(SIGVTALRM, &Server::signalHandler);
	signal(SIGPROF, &Server::signalHandler);
	signal(SIGWINCH, &Server::signalHandler);
	signal(SIGIO, &Server::signalHandler);
	signal(SIGPWR, &Server::signalHandler);
	signal(SIGSYS, &Server::signalHandler);
	signal(SIGTRAP, &Server::signalHandler);
}

Server::Server( Server const & src ) {}

Server::AlreadyRunningException::AlreadyRunningException(){}
Server::AlreadyRunningException::AlreadyRunningException( AlreadyRunningException const &orig){
	(void) orig;
}
Server::SyscallException::SyscallException( void ) throw() {}
Server::SyscallException::SyscallException( SyscallException const & src ) throw() {}

/* MEMBER OPERATORS OVERLOAD =================================================*/
Server		&Server::operator=( Server const & rhs ) {
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
	struct protoent			*proto;
	struct sockaddr_in	 sin;
	// struct timeval			 timeout;
	int									enable;

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
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	// if ((sin.sin_addr.s_addr = htonl(INADDR_ANY))== INADDR_NONE) {
	// 	this->reporter->error(INET_ADDR_FAILED);
	// 	throw Server::SyscallException();
	// }
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
	int								 pid;
	struct sockaddr_in	csin;
	unsigned int				cslen;
	int								 newSocket;

	cslen = sizeof(csin);
	while ((newSocket = accept(this->_socketMaster, (struct sockaddr*)&csin, &cslen)))
	{
		std::cout << "list:" << Server::_pidArray->size() << '\n';
		if (Server::_nChild < 3) {
			pid = fork();
			if (newSocket > 0 && pid == 0) {
				/* CHILD */
				// close(newSocket);
				// exit(0);
				while (1);
			} else if (newSocket > 0 && pid) {
				/* PARENT */
				Server::_nChild += 1;
				Server::_pidArray->push_front(pid);
				std::cout << "add child, nChild = " << Server::_nChild << std::endl;
			}
		} else {
			std::cout << "nope, too many child :D" << std::endl;
		}
	}
	close(this->_socketMaster);
	return (0);
}

void
Server::_erasePid( int pid ) {
	for (std::list<int>::iterator it = Server::_pidArray->begin(); it != Server::_pidArray->end(); ++it) {
		if (*it == pid) {
			Server::_pidArray->erase(it);
			break ;
		}
	}
}

void
Server::_killAllChilds( void ) {
	for (std::list<int>::iterator it = Server::_pidArray->begin(); it != Server::_pidArray->end(); ++it) {
		kill(*it, SIGINT);
	}
}

void
Server::signalHandler( int sig ) {
	int	stat_loc;
	int	pid;
	std::cout << getpid() << " ";
	switch (sig) {
		case SIGHUP:
		std::cout << LOG_SIGHUP << std::endl;
		exit(0);
		break;
		case SIGINT:
		std::cout << LOG_SIGINT << std::endl;
		exit(0);
		break;
		case SIGQUIT:
		std::cout << LOG_SIGQUIT << std::endl;
		exit(0);
		break;
		case SIGILL:
		std::cout << LOG_SIGILL << std::endl;
		exit(0);
		break;
		case SIGABRT:
		std::cout << LOG_SIGABRT << std::endl;
		exit(0);
		break;
		case SIGFPE:
		std::cout << LOG_SIGFPE << std::endl;
		exit(0);
		break;
		case SIGSEGV:
		std::cout << LOG_SIGSEGV << std::endl;
		exit(0);
		break;
		case SIGPIPE:
		std::cout << LOG_SIGPIPE << std::endl;
		exit(0);
		break;
		case SIGALRM:
		std::cout << LOG_SIGALRM << std::endl;
		exit(0);
		break;
		case SIGTERM:
		std::cout << LOG_SIGTERM << std::endl;
		exit(0);
		break;
		case SIGUSR1:
		std::cout << LOG_SIGUSR1 << std::endl;
		exit(0);
		break;
		case SIGUSR2:
		std::cout << LOG_SIGUSR2 << std::endl;
		exit(0);
		case SIGCHLD:
		std::cout << LOG_SIGCHLD << std::endl;
		pid = wait(&stat_loc);
		if (WIFEXITED(stat_loc) && WEXITSTATUS(stat_loc) == EXIT_QUIT) {
			Server::_killAllChilds();
			exit(0);
		} else {
			Server::_erasePid( pid );
			Server::_nChild -= Server::_nChild > 0 ? 1 : 0;
		}
		// std::cout << "del child, nChild = " << Server::_nChild << std::endl;
		break;
		case SIGKILL:
		std::cout << "KILLLL" << std::endl;
		exit(0);
		break;
		case SIGSTKFLT:
		std::cout << LOG_SIGSTKFLT << std::endl;
		exit(0);
		break;
		case SIGURG:
		std::cout << LOG_SIGURG << std::endl;
		exit(0);
		break;
		case SIGXCPU:
		std::cout << LOG_SIGXCPU << std::endl;
		exit(0);
		break;
		case SIGXFSZ:
		std::cout << LOG_SIGXFSZ << std::endl;
		exit(0);
		break;
		case SIGVTALRM:
		std::cout << LOG_SIGVTALRM << std::endl;
		exit(0);
		break;
		case SIGPROF:
		std::cout << LOG_SIGPROF << std::endl;
		exit(0);
		break;
		case SIGWINCH:
		std::cout << LOG_SIGWINCH << std::endl;
		exit(0);
		break;
		case SIGIO:
		std::cout << LOG_SIGIO << std::endl;
		exit(0);
		break;
		case SIGPWR:
		std::cout << LOG_SIGPWR << std::endl;
		exit(0);
		break;
		case SIGSYS:
		std::cout << LOG_SIGSYS << std::endl;
		exit(0);
		break;
		case SIGTRAP:
		std::cout << LOG_SIGTRAP << std::endl;
		exit(0);
		break;
		default:
		std::cout << "UNKNOWN" << std::endl;
		exit(0);
		break;
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
