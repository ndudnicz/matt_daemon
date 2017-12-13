#ifndef SERVER_HPP
# define SERVER_HPP

# include <signal.h>
# include <list>
#include <sys/file.h>

# include "Tintin_reporter.hpp"

# define DEFAULT_LISTENING_PORT	4242
# define LISTEN_MAX				3

class Server {

	private:
		Server( Server const & src );

		int							_fdLock;
		static unsigned int			_nChild;
		static std::string const	_LOCKFILEDIR;
		static std::string const	_LOCKFILENAME;
		static std::string const	_LOGNAME;
		int							_socketMaster;
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
