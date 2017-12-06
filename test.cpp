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

#include "Server.hpp"

int	main()
{
  int  pid = 0;

  if (getuid()) {
    std::cout << "Permission denied. Run as root." << '\n';
    exit(1);
  }
  pid = fork();
  if (pid < 0)
    exit(EXIT_FAILURE);
  else if (pid == 0) {
    Server  *s = new Server();
    s->run();
    while (1);
  } else {
    exit(0);
  }
  std::cout << "return main" << '\n';
	return 0;
}
