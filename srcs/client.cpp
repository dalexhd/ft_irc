/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:25:49 by aborboll          #+#    #+#             */
/*   Updated: 2022/04/09 20:09:43 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/client.hpp"

void Client::message(char const *message)
{
	if (send(_fd, message, strlen(message), 0) == -1)
		throw std::runtime_error("Error sending message");
}

std::string Client::read(void) const
{
	char buffer[MAX_BUFFER_SIZE];

	while (!std::strstr(buffer, "\r\n"))
	{
		memset(buffer, '\0', sizeof(buffer));
		if (recv(_fd, buffer, sizeof(buffer), 0) <= 0)
			break;
	}
	buffer[strlen(buffer) - 2] = '\0';
	return std::string(buffer);
}
