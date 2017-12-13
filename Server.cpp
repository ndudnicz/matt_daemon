#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sstream>
#include <string.h>
// #include <asm-generic/signal.h>

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
unsigned int		Server::_nClients = 3;

/* CONSTRUCTORS ==============================================================*/

Server::Server( void ) :
_socketMaster(0)
{
	int		pid = 0;

	this->_fdLock = open(Server::_LOCKFILENAME.c_str(), O_CREAT, 0666);
	if (chdir("/") < 0) {
		this->getReporter()->error("chdir() failed.");
		exit(EXIT_FAILURE);
	}
	if (flock(this->_fdLock, LOCK_EX | LOCK_NB)) {
		std::cout << "Cannot acquire exclusive lock on /var/lock/matt_daemon.lock" << std::endl;
		this->getReporter()->error("Error file locked.");
		exit(EXIT_FAILURE);
	} else {
		if (setsid() < 0) {
			this->getReporter()->error("setsid() failed.");
		}
		pid = fork();
		if (pid < 0) {
			exit(EXIT_FAILURE);
		} else if (pid == 0) {
			this->getReporter()->info("Server Initialized");
			for (int i = 1; i <= _NSIG; i++) {
				signal(i, &Server::signalHandler);
			}
		} else {
			exit(0);
		}
	}
}

Server::SyscallException::SyscallException( void ) throw() {}

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
		if (Server::_nChild < Server::_nClients) {
			pid = fork();
			if (newSocket > 0 && pid == 0) {
				Connection *connect = new Connection(newSocket, Server::_reporter);
				connect->handle();
			} else if (newSocket > 0 && pid) {
				Server::_nChild += 1;
				Server::_pidList->push_front(pid);
			}
		} else {
			std::stringstream	s;
			s << "Max number of clients already logged in (" << Server::_nClients << ")";
			Server::_reporter->warning(s.str());
			close(newSocket);
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
	/* http://man7.org/linux/man-pages/man7/signal.7.html */

	int	stat_loc = 0;
	int	pid = 0;
	if (sig != SIGCHLD) {
		Server::_reporter->info( "Signal handler." );
	}
	switch (sig) {
		case SIGCHLD:
		while (1) {
			pid = waitpid(-1, &stat_loc, WNOHANG);
			if (pid < 0) {
				break ;
			}
			Server::_nChild -= Server::_nChild > 0 ? 1 : 0;
			if (WIFEXITED(stat_loc) && WEXITSTATUS(stat_loc) == Connection::EXIT_QUIT) {
				Server::_killAllChilds();
				Server::_reporter->info("Quitting.");
				Server::_dellock();
				exit(0);
			} else {
				Server::_erasePid( pid );
			}
		}
		break;
		default:
		std::stringstream	msg;
		msg << "Signal handler: " << std::to_string(sig) << std::endl;
		Server::_reporter->error(msg.str());
		Server::_killAllChilds();
		Server::_dellock();
		exit(0);
		break;
	}
}

void
Server::_dellock( void ) {
	unlink(Server::_LOCKFILENAME.c_str());
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
