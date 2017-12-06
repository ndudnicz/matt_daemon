/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmanet <tmanet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 09:16:36 by tmanet            #+#    #+#             */
/*   Updated: 2017/12/06 09:18:24 by tmanet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>

#include "Server.hpp"

void ft(int sig) {
  std::cout << "Child died!" << '\n';
}

int	main()
{
  int  pid = 0;

  if (getuid()) {
    std::cout << "Permission denied. Run as root." << std::endl;
    exit(1);
  }
  signal(SIGCHLD, &ft);
  pid = fork();
  if (pid < 0)
    return (EXIT_FAILURE);
  else if (pid == 0) {
    Server  *s = new Server();
    s->masterLoop();
    return (0);
  } else {
    return (0);
  }
}
