#ifndef SERVER_HPP
# define SERVER_HPP

# include "Tintin_reporter.hpp"

class Server {

	private:
		Server( Server const & src );

		Tintin_reporter *reporter;

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
