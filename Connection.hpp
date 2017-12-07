#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# define BUFF_SIZE 2048

# include <string>
# include <strings.h>
# include "Tintin_reporter.hpp"
# include <sys/types.h>
# include <sys/socket.h>


class Connection {

	private:
		Connection( Connection const & src );
		Connection( void );
		Connection &  operator=( Connection const & rhs );
		void 					handleData(std::string data);
		void 					handleLine( std::string line );

		Tintin_reporter           *_reporter;
    	int                       _socket;

	public:
		~Connection( void );
		Connection( int socket, Tintin_reporter *reporter );
		void handle( void );

};
#endif
