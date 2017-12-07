#ifndef SERVER_HPP
# define SERVER_HPP

# include <signal.h>

# include "Tintin_reporter.hpp"
#include <sys/file.h>


# define DEFAULT_LISTENING_PORT 4242
# define LISTEN_MAX             3
# define DEFAULT_BINDING_IP     "127.0.0.1"

# define LOG_SIGHUP "SIGHUP"
# define LOG_SIGINT "SIGINT"
# define LOG_SIGQUIT "SIGQUIT"
# define LOG_SIGILL "SIGILL"
# define LOG_SIGABRT "SIGABRT"
# define LOG_SIGFPE "SIGFPE"
# define LOG_SIGSEGV "SIGSEGV"
# define LOG_SIGPIPE "SIGPIPE"
# define LOG_SIGALRM "SIGALRM"
# define LOG_SIGTERM "SIGTERM"
# define LOG_SIGUSR1 "SIGUSR1"
# define LOG_SIGUSR2 "SIGUSR2"
# define LOG_SIGCHLD "SIGCHLD"
# define LOG_SIGCONT "SIGCONT"
# define LOG_SIGTSTP "SIGTSTP"
# define LOG_SIGTTIN "SIGTTIN"
# define LOG_SIGTTOU "SIGTTOU"

# define LOG_SIGBUS "SIGBUS"
# define LOG_SIGUSR2 "SIGUSR2"
# define LOG_SIGUSR2 "SIGUSR2"
# define LOG_SIGUSR2 "SIGUSR2"
# define LOG_SIGUSR2 "SIGUSR2"
# define LOG_SIGUSR2 "SIGUSR2"
# define LOG_SIGUSR2 "SIGUSR2"
# define LOG_SIGUSR2 "SIGUSR2"
# define LOG_SIGUSR2 "SIGUSR2"
# define LOG_SIGUSR2 "SIGUSR2"
# define LOG_SIGUSR2 "SIGUSR2"
# define LOG_SIGUSR2 "SIGUSR2"
# define LOG_SIGUSR2 "SIGUSR2"

class Server {

	private:
		Server( Server const & src );

		Tintin_reporter     *reporter;
    int                 _fdLock;
    static std::string  _LOCKFILENAME;
    static std::string  _LOCKFILEDIR;
    static std::string  _SERVERNAME;

	protected:

	public:
		Server( void );
		~Server( void );

		void  run(void);

		Server &  operator=( Server const & rhs );




  /* Exceptions ==============================================================*/

  class SyscallException : public std::exception {
  private:
    SyscallException &  operator=( SyscallException const & rhs );
  protected:
  public:
    SyscallException( void ) throw();
    SyscallException( SyscallException const & src ) throw();
    ~SyscallException( void ) throw();
    virtual const char *what( void ) const throw();
  };

  class AlreadyRunningException : public std::exception {
		public:
			AlreadyRunningException();
			~AlreadyRunningException();
			AlreadyRunningException(AlreadyRunningException const &orig);
			virtual const char* what() const throw();
		private:
			AlreadyRunningException	&operator=(AlreadyRunningException const &orig);
	};

};

#endif
