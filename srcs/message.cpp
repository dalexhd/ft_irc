/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:25:49 by aborboll          #+#    #+#             */
/*   Updated: 2022/06/03 14:17:25 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Message.hpp"

static std::map<size_t, std::string> tokenize(std::string const &str, const char delim)
{
	size_t                        start;
	size_t                        end = 0;
	size_t                        occurrences = 0;
	std::map<size_t, std::string> ret;

	while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = str.find(delim, start);
		if (str[start] == ':')
		{
			end = str.length();
			start += 1;
		}
		ret.insert(make_pair(occurrences, str.substr(start, end - start)));
		occurrences++;
	}
	return (ret);
}

/**
 * @brief Construct a new Message:: Message object
 *
 * Here we will only construct "<command> <parameters> <crlf>" properties from
 * the message. We will ignore tags and source from the message.
 *
 * These message parts, and parameters themselves, are separated by one or more ASCII SPACE characters `(' ', 0x20)`.
 *
 * @param buffer The string buffer
 */
Message::Message(std::string &buffer)
{
	size_t      _start_pos;
	size_t      _end_pos;
	std::string tmp;

	_buffer = buffer;

	// Parse cmd
	_start_pos = buffer.find_first_not_of(' '); // We skip trailing spaces.
	if (_start_pos == std::string::npos)        // In case is an empty message
	{
		_cmd = "";
		_params = tokenize("", ' ');
		return;
	}
	_end_pos = buffer.find_first_of(" \r\n", _start_pos);
	_cmd = _buffer.substr(_start_pos, _end_pos - _start_pos);
	// Convert cmd to lowercase
	_cmd = to_lower(_cmd);

	// Parse params
	_start_pos = buffer.find_first_of(" ", _start_pos); // We find the index character after the cmd.
	_end_pos = buffer.find_first_of("\r\n", _start_pos); // We find the index of leading "\r\n" chars.
	tmp = _buffer.substr(_start_pos + 1, _end_pos - _start_pos);
	_params = tokenize(tmp, ' ');
}

std::string Message::getCmd(void) const
{
	return (_cmd);
}

std::map<size_t, std::string> Message::getParams(void) const
{
	return (_params);
}
