/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 12:36:41 by aborboll          #+#    #+#             */
/*   Updated: 2022/03/09 17:51:50 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/server.hpp"

int main(int argc, char **argv)
{
	if (!validate_args(argc, argv))
		return (1);
	Server server(argv[1], argv[2]);
	server.run();
	return (0);
}
