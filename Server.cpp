#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#include "Server.hpp"
#include "Tintin_reporter.hpp"
#include "Connection.hpp"
#include "error.hpp"


/* STATIC VARIABLES ==========================================================*/

unsigned int		Server::_nChild = 0;
std::string const	Server::_SERVERNAME ="matt_daemon";
std::string const	Server::_LOCKFILEDIR = "/var/lock/";
std::string const	Server::_LOCKFILENAME = Server::_LOCKFILEDIR +Server::_SERVERNAME + ".lock";
std::list<int>		*Server::_pidList = new std::list<int>;
Tintin_reporter		*Server::_reporter = NULL;

/* CONSTRUCTORS ==============================================================*/

Server::Server( void ) :
_socketMaster(0),
_inetAddr(0),
_port(0)
{
	int		pid = 0;

	this->_fdLock = open(Server::_LOCKFILENAME.c_str(), O_CREAT, 0666);
	if (flock(this->_fdLock, LOCK_EX | LOCK_NB)) {
		std::cout << "Cannot acquire exclusive lock on /var/lock/matt_daemon.lock" << std::endl;
		this->getReporter()->error("Error file locked.");
		exit(EXIT_FAILURE);
	} else {
		pid = fork();
		if (pid < 0) {
			exit(EXIT_FAILURE);
		} else if (pid == 0) {
			this->getReporter()->info("Server Initialized");
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
		} else {
			exit(0);
		}
	}
}

// Server::Server( Server const & src ) {}
Server::SyscallException::SyscallException( void ) throw() {}
// Server::SyscallException::SyscallException( SyscallException const & src ) throw() {}

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


/* MEMBER FUNCTIONS ==========================================================*/
/*
** Open a connection on port 4242
*/
void
Server::openConnection( void ) {
	struct protoent		*proto;
	struct sockaddr_in	 sin;
	int					enable;

	enable = 1;
	sin.sin_port = htons((unsigned short)DEFAULT_LISTENING_PORT);
	// ntohs(sin.sin_port);
	if ((proto = getprotobyname("tcp")) == 0) {
		Server::_reporter->error(GPBN_FAIL);
		throw Server::SyscallException();
	}
	if ((this->_socketMaster = socket(PF_INET, SOCK_STREAM, proto->p_proto)) < 0) {
		Server::_reporter->error(SOCKET_FAILED);
		throw Server::SyscallException();
	}
	if (setsockopt(this->_socketMaster, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
		Server::_reporter->error(SETSOCKOPT_FAILED);
		throw Server::SyscallException();
	}
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(this->_socketMaster, (const struct sockaddr*)&sin, sizeof(sin)) < 0) {
		Server::_reporter->error(BIND_FAILED);
		throw Server::SyscallException();
	}
	if (listen(this->_socketMaster, LISTEN_MAX) < 0) {
		Server::_reporter->error(LISTEN_FAILED);
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
		if (Server::_nChild < 3) {
			pid = fork();
			if (newSocket > 0 && pid == 0) {
				Connection *connect = new Connection(newSocket, Server::_reporter);
				connect->handle();
			} else if (newSocket > 0 && pid) {
				Server::_nChild += 1;
				Server::_pidList->push_front(pid);
			}
		} else {
			close(newSocket);
			std::cout << "nope, too many child :D" << std::endl;
		}
	}
	close(this->_socketMaster);
	return (0);
}

void
Server::_erasePid( int pid ) {
	for (std::list<int>::iterator it = Server::_pidList->begin(); it != Server::_pidList->end(); ++it) {
		if (*it == pid) {
			Server::_pidList->erase(it);
			break ;
		}
	}
}

void
Server::_killAllChilds( void ) {
	for (std::list<int>::iterator it = Server::_pidList->begin(); it != Server::_pidList->end(); ++it) {
		kill(*it, SIGINT);
	}
}

void
Server::signalHandler( int sig ) {
	int	stat_loc;
	int	pid;
	if (sig != SIGCHLD) {
		Server::_reporter->info( "Signal handler." );
	}
	switch (sig) {
		case SIGHUP:
		exit(0);
		break;
		case SIGINT:
		exit(0);
		break;
		case SIGQUIT:
		exit(0);
		break;
		case SIGILL:
		exit(0);
		break;
		case SIGABRT:
		exit(0);
		break;
		case SIGFPE:
		exit(0);
		break;
		case SIGSEGV:
		exit(0);
		break;
		case SIGPIPE:
		exit(0);
		break;
		case SIGALRM:
		exit(0);
		break;
		case SIGTERM:
		exit(0);
		break;
		case SIGUSR1:
		break;
		case SIGUSR2:
		break;
		case SIGCHLD:
		pid = wait(&stat_loc);
		if (WIFEXITED(stat_loc) && WEXITSTATUS(stat_loc) == Connection::EXIT_QUIT) {
			Server::_killAllChilds();
			Server::_reporter->info("Quitting.");
			exit(0);
		} else {
			Server::_erasePid( pid );
			Server::_reporter->info( "Signal handler." );
			Server::_nChild -= Server::_nChild > 0 ? 1 : 0;
		}
		break;
		case SIGKILL:
		exit(0);
		break;
		case SIGSTKFLT:
		exit(0);
		break;
		case SIGURG:
		exit(0);
		break;
		case SIGXCPU:
		exit(0);
		break;
		case SIGXFSZ:
		exit(0);
		break;
		case SIGVTALRM:
		exit(0);
		break;
		case SIGPROF:
		exit(0);
		break;
		case SIGWINCH:
		exit(0);
		break;
		case SIGIO:
		exit(0);
		break;
		case SIGPWR:
		exit(0);
		break;
		case SIGSYS:
		exit(0);
		break;
		case SIGTRAP:
		exit(0);
		break;
		default:
		break;
	}
}

Tintin_reporter*
Server::getReporter( void ) {
	if (Server::_reporter == NULL) {
		Server::_reporter = new Tintin_reporter( Server::_SERVERNAME );
	}
	return (Server::_reporter);
}

const char*
Server::SyscallException::what( void ) const throw() {
	return ("");
}

/* NON MEMBER FUNCTIONS ======================================================*/


/* OPERATOR ==================================================================*/
