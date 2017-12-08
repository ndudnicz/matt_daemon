#include <stdlib.h>

#include "Connection.hpp"

const std::string	Connection::_GREETINGS = "Welcome to matt_daemon, type help to learn commands.";
int const			Connection::EXIT_QUIT = 42;

Connection::Connection( int socket, Tintin_reporter *reporter ) :
_reporter(reporter),
_socket(socket)
{
	std::stringstream stream;
	stream << "client_"<< ::getpid();
	this->_userName = new std::string(stream.str());
	stream << " connected.";
	this->_reporter->info(stream.str());
	this->sendMsg(Connection::_GREETINGS);
	this->prompt();
}

void
Connection::handle( void ) {
	char		buffer[BUFF_SIZE];
	std::string	recv;
	int 		receivedSize = 0;

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
			this->_handleData(recv);
			recv.clear();
		}
		::bzero(buffer, BUFF_SIZE);
		this->prompt();
	}
	std::stringstream stream;
	stream << *this->_userName << " disconnected.";
	this->_reporter->info(stream.str());
	exit(0);
}

void
Connection::_handleData( std::string data ) {

	size_t startPos = 0;
	size_t endPos = 0;
	while (((endPos = data.find('\n', startPos)) != std::string::npos))
	{
		this->_handleLine(data.substr(startPos, endPos - startPos));
		startPos = endPos + 1;
	}
}

void
Connection::_handleLine(std::string line) {
	if (line.compare(0, 4, "quit") == 0)
	this->quit();
	else if (line.compare(0, 5, "user ") == 0)
	this->user(line.substr(5));
	else if (line.compare(0, 4, "help") == 0)
	this->help();
	else if (!line.empty())
	this->log(line);
}

void
Connection::quit( void ){
	std::stringstream stream;
	stream << *this->_userName << " request server to quit.";
	this->_reporter->info(stream.str());
	this->sendMsg(Connection::_QUIT);
	close(this->_socket);
	stream.clear();
	stream << *this->_userName << " disconnected.";
	this->_reporter->info(stream.str());
	exit(Connection::EXIT_QUIT);
}

void
Connection::user( std::string user){
	if (!user.empty()){
		std::stringstream stream;
		stream << *this->_userName << " change username to " << user;
		this->_reporter->info(stream.str());
		delete(this->_userName);
		this->_userName = new std::string(user);
	}
}

void
Connection::sendMsg( std::string msg ){
	::send(this->_socket, (msg + '\n').c_str(), msg.length() + 1, 0);
}

void
Connection::prompt( void ) {
	std::string prompt = *this->_userName;
	prompt.append("> ");
	::send(this->_socket, prompt.c_str(), prompt.length(), 0);
}

void
Connection::help( void ){
	this->sendMsg("quit: Stop the daemon.");
	this->sendMsg("help: Display this message.");
}

void
Connection::log( std::string msg ) {
	std::stringstream stream;
	stream << *this->_userName << ": " << msg;
	this->_reporter->log(stream.str());
}
