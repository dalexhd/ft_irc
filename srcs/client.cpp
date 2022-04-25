/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:25:49 by aborboll          #+#    #+#             */
/*   Updated: 2022/04/22 18:52:13 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/client.hpp"

void Client::message(char const *message)
{
	if (send(_fd, message, strlen(message), 0) == -1)
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
	std::string tmp = std::string(buffer);
	_messagesSent.push_back(Message(tmp));
	return (&(_messagesSent.back()));
}
