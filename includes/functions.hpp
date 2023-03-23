/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:13:29 by aborboll          #+#    #+#             */
/*   Updated: 2023/03/23 16:34:36 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

std::string              itoa(int a);
std::vector<std::string> split(const std::string &str, const std::string &delimiters);
std::string              to_lower(std::string str);
bool                     is_number(const std::string &s);

#endif
