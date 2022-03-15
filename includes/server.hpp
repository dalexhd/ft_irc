/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:13:29 by aborboll          #+#    #+#             */
/*   Updated: 2022/03/09 17:46:44 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <iostream>
#include <string>

// Validation
bool validate_args(int argc, char **argv);

class Server
{
  public:
	Server(char *port, char *password);
	~Server();
	void run();
};

#endif
