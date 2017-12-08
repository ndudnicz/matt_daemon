#ifndef Tintin_reporter_HPP
# define Tintin_reporter_HPP

# include <fstream>
# include <iostream>
# include <sys/stat.h>
# include <sys/types.h>

# include <string>
# include <ctime>

class Tintin_reporter {

private:

protected:

public:
	Tintin_reporter( void );
	Tintin_reporter( std::string str );
	Tintin_reporter( Tintin_reporter const & src );
	~Tintin_reporter( void );

	void log(std::string	str);
	void info(std::string	str);
	void warning(std::string str);
	void error(std::string str);
	Tintin_reporter &setFilename(std::string	str);
	std::ofstream		getFile( void ) const;

private:
	std::ofstream		file;
	void 			write(std::string errortype, std::string str);
	Tintin_reporter 	&operator=( Tintin_reporter const & rhs );
	std::string		getTime( void );

};

#endif
