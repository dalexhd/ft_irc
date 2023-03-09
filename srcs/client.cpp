/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:25:49 by aborboll          #+#    #+#             */
/*   Updated: 2022/09/21 16:37:33 by aborboll         ###   ########.fr       */
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

void Client::read(void)
{
	char    buffer[MAX_BUFFER_SIZE];
	ssize_t res;

	memset(buffer, '\0', sizeof(buffer));
	res = recv(_fd, buffer, sizeof(buffer), 0);
	if (res == -1)
	{
		std::cout << "Error received -1" << std::endl;
		return;
	}
	else if (res == 0)
	{
		std::cout << "No data received 0" << std::endl;
		this->_status = DISCONNECTED;
		return;
	}
	if (strlen(buffer) > 2)
		buffer[strlen(buffer) - 2] = '\0';
	std::string tmp(buffer);
	_messagesSent.push_back(Message(tmp));
	_message = &(_messagesSent.back());
}
