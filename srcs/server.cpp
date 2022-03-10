/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:25:49 by aborboll          #+#    #+#             */
/*   Updated: 2022/03/09 17:48:49 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

Server::Server(char *port, char *password)
{
	std::cout << "Server started at port " << port << " with password: " << password << std::endl;
}

void Server::run()
{
	std::cout << "Server is running" << std::endl;
}

Server::~Server(void)
{
	std::cout << "Server closed" << std::endl;
}
