#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# define BUFF_SIZE 2048

# include <string>
# include <strings.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>

# include "Tintin_reporter.hpp"

class Connection {

	private:
		Connection( Connection const & src );
		Connection( void );
		Connection & operator=( Connection const & rhs );
		void 					handleData(std::string data);
		void 					handleLine( std::string line );
		void 					sendMsg( std::string msg );
		void 					help( void );

		Tintin_reporter           *_reporter;
		int                       _socket;

	public:
		~Connection( void );
		Connection( int socket, Tintin_reporter *reporter );
		void handle( void );
		static std::string const	_GREETINGS;
		static std::string const	_QUIT;
		static int const			EXIT_QUIT;

};
#endif
