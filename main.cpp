#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>

#include "Server.hpp"

int	main( void )
{
	if (getuid()) {
		std::cout << "Permission denied. Run as root." << std::endl;
		return (1);
	} else {
		if (fork() > 0) {
			exit(0);
		} else {
			Server	*s = new Server();
			try {
				s->openConnection();
				s->masterLoop();
			} catch (Server::SyscallException & e) {
				exit(EXIT_FAILURE);
			}
			return 0;
		}
	}
}
