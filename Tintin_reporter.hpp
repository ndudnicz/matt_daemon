/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tintin_reporter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmanet <tmanet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 09:23:52 by tmanet            #+#    #+#             */
/*   Updated: 2017/12/06 09:47:42 by tmanet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Tintin_reporter_HPP
# define Tintin_reporter_HPP
# include <fstream>
# include <iostream>

#include <string>

using namespace std;

class Tintin_reporter {

private:

protected:

public:
	Tintin_reporter( void );
	Tintin_reporter( std::string str );
	Tintin_reporter( Tintin_reporter const & src );
	~Tintin_reporter( void );



	void info(std::string	str);
	void warning(std::string str);
	void error(std::string str);
	Tintin_reporter &setFilename(std::string	str);
	ofstream		getFile( void ) const;

private:
	ofstream		file;
	void write(std::string errortype, std::string str);
	Tintin_reporter &	operator=( Tintin_reporter const & rhs );

};

#endif
