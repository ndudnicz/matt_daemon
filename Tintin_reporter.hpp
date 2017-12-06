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

class Tintin_reporter {

private:

protected:

public:
	Tintin_reporter( void );
	Tintin_reporter( Tintin_reporter const & src );
	~Tintin_reporter( void );

	Tintin_reporter &	operator=( Tintin_reporter const & rhs );


	void info(std::string	str);
	void warning(std::string str);
	void error(std::string str);
	Tintin_reporter &setFilename(std::string	str);

private:
	std::string	fileName;
	void write(std::string errortype, std::string str);

};

#endif
