/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:25:49 by aborboll          #+#    #+#             */
/*   Updated: 2022/04/06 18:49:06 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/client.hpp"

void Client::message(std::string &message)
{
	if (send(_fd, message.c_str(), message.size(), 0) == -1)
		throw std::runtime_error("Error sending message" + std::string(strerror(errno)));
}

void Client::disconnect(void)
{
	close(_fd);
	_fd = -1;
}
