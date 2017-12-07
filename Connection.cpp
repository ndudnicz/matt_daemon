
#include <stdlib.h>
# include "Connection.hpp"

Connection::Connection( int socket, Tintin_reporter *reporter ) {
	this->_socket = socket;
	this->_reporter = reporter;
	this->_reporter->info("Client connected.");
}

void 		Connection::handle( void ) {

	char	buffer[BUFF_SIZE];

	std::string recv;
	int 	receivedSize = 0;

	::bzero(buffer, BUFF_SIZE);
	while (
		(
			receivedSize = ::recv(
				this->_socket,
				buffer,
				BUFF_SIZE,
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
	this->_reporter->info("Client disconnected.");
	exit(0);
}


void 		Connection::handleData(std::string data) {

	size_t startPos = 0;
	size_t endPos = 0;
	while (((endPos = data.find('\n', startPos)) != std::string::npos))
	{
		this->handleLine(data.substr(startPos, endPos - startPos));
		startPos = endPos + 1;
	}

}

void 		Connection::handleLine(std::string line) {
	if (line.compare(0, 4, "quit") == 0) {
		this->_reporter->info("Request quit.");
		this->sendMsg("Ok, thanks bye");
		close(this->_socket);
		exit(42);
	}
	if (!line.empty())
	this->_reporter->log(line);
}


void 		Connection::sendMsg(std::string msg){
	::send(this->_socket, msg.c_str(), msg.length(), 0);
}
