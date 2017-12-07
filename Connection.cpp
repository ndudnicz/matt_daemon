

#include <stdlib.h>
# include "Connection.hpp"

Connection::Connection( int socket, Tintin_reporter *reporter ) {
	this->_socket = socket;
	this->_reporter = reporter;
}

void 		Connection::handle( void ) {

	char	buffer[BUFF_SIZE];

	std::string recv;
	int 	receivedSize = 0;

	::bzero(buffer, BUFF_SIZE);
	while (
		(
			receivedSize = ::recv(
				(int)this->_socket,
				(char*)buffer,
				(size_t)BUFF_SIZE,
				0
			)
		)
	)
	{
		recv.append(buffer, receivedSize);

		if (receivedSize < BUFF_SIZE) {
			this->handleData(recv);
			recv.clear();
		}
		::bzero(buffer, BUFF_SIZE);
	}
}


void 		Connection::handleData(std::string data) {

	int startPos = 0;
	int endPos = 0;
	while (((endPos = data.find('\n', startPos)) != std::string::npos))
	{
		this->handleLine(data.substr(startPos, endPos - startPos));
		startPos = endPos + 1;
	}

}

void 		Connection::handleLine(std::string line) {

	this->_reporter->log(line);
}
