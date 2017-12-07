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

int	main()
{
  int  pid = 0;

  pid = fork();
  if (pid < 0)
    exit(EXIT_FAILURE);
  else if (pid == 0) {
    Server  *s = new Server();
    try {
      s->openConnection();
      s->masterLoop();
    } catch (...) {
      std::cout << "failure" << '\n';
      exit(EXIT_FAILURE);
    }
  } else {
    std::cout << "launcher exit" << '\n';
    // exit(0);
  }
  std::cout << "return main" << '\n';
	return 0;
}
