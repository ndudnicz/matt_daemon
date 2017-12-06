#ifndef SERVER_HPP
# define SERVER_HPP

# include <signal.h>

# include "Tintin_reporter.hpp"

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

class Server {

private:
  Server( Server const & src );

  Tintin_reporter           *reporter;
  static std::string const  _LOGNAME;


protected:

public:
  Server( void );
  ~Server( void );

  static void  signalHandler( int sig );
  void  run( void );

  Server &  operator=( Server const & rhs );

};

#endif
