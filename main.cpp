/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 12:36:41 by aborboll          #+#    #+#             */
/*   Updated: 2022/04/09 18:11:34 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/server.hpp"

int main(int argc, char **argv)
{
	Server *                 server;
	std::vector<std::string> args;

	for (int i(0); i < argc; ++i)
		args.push_back(argv[i]); // We store args inside a vector with std::string

	if (!validate_args(argc, argv))
		return (1);
	try
	{
		server = new Server("127.0.0.1", args[1], args[2]);
		delete server;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}
