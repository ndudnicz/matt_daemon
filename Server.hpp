#ifndef SERVER_HPP
# define SERVER_HPP

# include <signal.h>

# include "Tintin_reporter.hpp"

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

  static std::string const  _LOGNAME;
  Tintin_reporter           *reporter;
  int                       _socketMaster;
  int                       _inetAddr;
  int                       _port;

protected:

public:
  Server( void );
  ~Server( void );

  static void  signalHandler( int sig );
  void  openConnection( void );
  void  masterLoop( void );

  Server &  operator=( Server const & rhs );

};

#endif
