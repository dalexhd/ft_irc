/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 20:21:52 by aborboll          #+#    #+#             */
/*   Updated: 2022/05/18 18:32:20 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/functions.hpp"

std::string itoa(int a)
{
	std::string ss = ""; // create empty string
	while (a)
	{
		int x = a % 10;
		a /= 10;
		char i = '0';
		i = i + x;
		ss = i + ss; // append new character at the front of the string!
	}
	return ss;
}

std::vector<std::string> split(const std::string &str, const std::string &delimiters)
{
	std::vector<std::string> tokens;

	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	std::string::size_type pos = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		lastPos = str.find_first_not_of(delimiters, pos);
		pos = str.find_first_of(delimiters, lastPos);
	}
	return tokens;
}
