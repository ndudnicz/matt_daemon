#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>

#include "Server.hpp"

int	main()
{
	int	pid = 0;

	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	else if (pid == 0) {
		try {
			Server	*s = new Server();
			s->openConnection();
			s->masterLoop();
		} catch (Server::AlreadyRunningException & e) {
			std::cout <<std::endl<< e.what() << std::endl;
			exit(EXIT_FAILURE);
		} catch (Server::SyscallException & e) {
			exit(EXIT_FAILURE);
		}
	} else {
		// std::cout << "launcher exit" << '\n';
		// exit(0);
	}
	// std::cout << "return main" << '\n';
	return 0;
}
