/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 20:21:52 by aborboll          #+#    #+#             */
/*   Updated: 2022/05/02 15:07:10 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/functions.hpp"
#include <vector>

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

