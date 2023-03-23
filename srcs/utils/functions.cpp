/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 20:21:52 by aborboll          #+#    #+#             */
/*   Updated: 2023/03/23 16:33:20 by aborboll         ###   ########.fr       */
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

std::string to_lower(std::string str)
{
	std::string tmp = "";
	for (size_t i = 0; i < str.length(); i++)
		tmp += tolower(str[i]);
	return tmp;
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

bool is_number(const std::string &s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it))
		++it;
	return !s.empty() && it == s.end();
}
