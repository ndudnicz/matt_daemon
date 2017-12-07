#include "Tintin_reporter.hpp"

/* STATIC VARIABLES ==========================================================*/


/* CONSTRUCTORS ==============================================================*/
Tintin_reporter::Tintin_reporter( void ) {
}
Tintin_reporter::Tintin_reporter( Tintin_reporter const & src ) {
}

Tintin_reporter::Tintin_reporter( std::string fn) {
	this->setFilename(fn);
}
/* MEMBER OPERATORS OVERLOAD =================================================*/


/* DESTRUCTOR ================================================================*/
Tintin_reporter::~Tintin_reporter( void ) {
	this->file.close();
}

/* MEMBER FUNCTIONS ==========================================================*/
void  Tintin_reporter::log(std::string str){

	this->write("LOG", str);
}

void  Tintin_reporter::info(std::string str){

	this->write("INFO", str);
}

void  Tintin_reporter::warning(std::string str){

	this->write("WARNING", str);
}

void  Tintin_reporter::error(std::string str){

	this->write("ERROR", str);
}

void  Tintin_reporter::write(std::string errortype, std::string str){
	if (this->file)
	{
		this->file << this->getTime() << " [ " << errortype << " ] - " << str << std::endl;
		this->file.flush();
	}
}

Tintin_reporter   &Tintin_reporter::setFilename(std::string fn){
	if (this->file)
		this->file.close();

	mkdir(("/var/log/" + fn).c_str(), 0640);
	std::string fullPath = "/var/log/" + fn + "/" + fn + ".log";
	this->file = std::ofstream(fullPath, std::ios::out | std::ios::app);

	return *this;
}

std::string	Tintin_reporter::getTime( void ) {

	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time (&rawtime);
timeinfo = localtime(&rawtime);

	strftime(buffer,sizeof(buffer),"[%d/%m/%Y-%H:%M:%S]",timeinfo);
	return std::string(buffer);
}
/* NON MEMBER FUNCTIONS ======================================================*/


/* OPERATOR ==================================================================*/
