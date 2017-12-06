#ifndef SERVER_HPP
# define SERVER_HPP

# include "Tintin_reporter.hpp"
#include <sys/file.h>


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


		class AlreadyRunningException : public std::exception
	{
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
