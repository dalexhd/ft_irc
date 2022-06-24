/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:25:49 by aborboll          #+#    #+#             */
/*   Updated: 2022/06/03 15:30:30 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

void Client::message(char const *message)
{
	std::cout << C_BLUE ">> Sending message to client: " << _nick << ": " << C_X << message;
	if (send(_fd, message, strlen(message), 0) == -1)
		throw std::runtime_error("Error sending message");
}

void Client::message(std::string const message)
{
	std::cout << C_BLUE ">> Sending message to client: " << _nick << ": " << C_X << message;
	if (send(_fd, message.c_str(), message.length(), 0) == -1)
		throw std::runtime_error("Error sending message");
}

Message *Client::read(void)
{
	char buffer[MAX_BUFFER_SIZE];

	while (!std::strstr(buffer, "\r\n"))
	{
		memset(buffer, '\0', sizeof(buffer));
		if (recv(_fd, buffer, sizeof(buffer), 0) <= 0)
			break;
	}
	buffer[strlen(buffer) - 2] = '\0';
	std::string tmp(buffer);
	_messagesSent.push_back(Message(tmp));
	return (&(_messagesSent.back()));
}
