#ifndef SERVER_HPP
# define SERVER_HPP

# include <signal.h>
# include <list>
#include <sys/file.h>

# include "Tintin_reporter.hpp"

# define DEFAULT_LISTENING_PORT	4242
# define LISTEN_MAX				3


// # define LOG_SIGHUP "SIGHUP"
// # define LOG_SIGINT "SIGINT"
// # define LOG_SIGQUIT "SIGQUIT"
// # define LOG_SIGILL "SIGILL"
// # define LOG_SIGABRT "SIGABRT"
// # define LOG_SIGFPE "SIGFPE"
// # define LOG_SIGSEGV "SIGSEGV"
// # define LOG_SIGPIPE "SIGPIPE"
// # define LOG_SIGALRM "SIGALRM"
// # define LOG_SIGTERM "SIGTERM"
// # define LOG_SIGUSR1 "SIGUSR1"
// # define LOG_SIGUSR2 "SIGUSR2"
// # define LOG_SIGCHLD "SIGCHLD"
// # define LOG_SIGCONT "SIGCONT"
// # define LOG_SIGTSTP "SIGTSTP"
// # define LOG_SIGTTIN "SIGTTIN"
// # define LOG_SIGTTOU "SIGTTOU"
// # define LOG_SIGBUS "SIGBUS"
// # define LOG_SIGSTKFLT "SIGSTKFLT"
// # define LOG_SIGURG "SIGURG"
// # define LOG_SIGXCPU "SIGXCPU"
// # define LOG_SIGXFSZ "SIGXFSZ"
// # define LOG_SIGVTALRM "SIGVTALRM"
// # define LOG_SIGPROF "SIGPROF"
// # define LOG_SIGWINCH "SIGWINCH"
// # define LOG_SIGIO "SIGIO"
// # define LOG_SIGPWR "SIGPWR"
// # define LOG_SIGSYS "SIGSYS"
// # define LOG_SIGTRAP "SIGTRAP"

class Server {

	private:
		Server( Server const & src );

		int							_fdLock;
		static unsigned int			_nChild;
		static std::string const	_LOCKFILEDIR;
		static std::string const	_LOCKFILENAME;
		static std::string const	_LOGNAME;
		int							_socketMaster;
		int							_inetAddr;
		int							_port;
		static std::list<int>		*_pidList;
		static std::string const	_SERVERNAME;
		static unsigned int			_nClients;

		static void					_erasePid( int pid );
		static void					_killAllChilds( void );
		static void					_dellock( void );
		static Tintin_reporter		*_reporter;

	protected:

	public:
		Server( void );
		~Server( void );


		int					masterLoop( void );
		void				openConnection( void );
		static void			signalHandler( int sig );
		Tintin_reporter		*getReporter( void );

		Server &	operator=( Server const & rhs );

	/* Exceptions ============================================================*/

	class SyscallException : public std::exception {
	private:
		SyscallException &	operator=( SyscallException const & rhs );
	protected:
	public:
		SyscallException( void ) throw();
		SyscallException( SyscallException const & src ) throw();
		~SyscallException( void ) throw();
		virtual const char *what( void ) const throw();
	};

};

#endif
