// Create IRC client in cpp
#include "./Client.hpp"
#include <map>
#include <string>

#include <cstring>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>

class File
{
  public:
	std::string                   _name;
	std::map<std::string, Client> _clients;

  public:
	File(std::string filename)
	{
		this->_name = filename;
	}

	~File()
	{
	}

  private:
	std::vector<std::string> getFileContent()
	{
		std::vector<std::string> lines;
		std::string              line;
		std::ifstream            file(std::string(this->_name).c_str());

		for (std::string line; std::getline(file, line);)
		{
			lines.push_back(line);
		}
		return (lines);
	}

  public:
	void parseLine(std::string line)
	{
		std::vector<std::string> parts;
		std::string              delimiter = " ";
		size_t                   pos = 0;
		std::string              token;
		std::vector<std::string> params;

		while ((pos = line.find(delimiter)) != std::string::npos) //fill line
		{
			token = line.substr(0, pos);
			parts.push_back(token);
			line.erase(0, pos + delimiter.length());
		}
		parts.push_back(line);

		if(parts.size() < 4)
		{
			std::cout << "  Command line not valid: '" << line << "'\n";
		}
		else
		{
			Command lineCommand = Command(std::atoi(parts[0].c_str()), parts[4], parts);
			if (this->_clients.find(parts[1]) == this->_clients.end())
			{
				this->_clients.insert(std::pair<std::string, Client>(parts[1], Client(parts[1], parts[2], parts[3]))); // Add client to map
			}

			this->_clients.find(parts[1])->second._commands.insert(
				std::pair<size_t, Command>(std::atoi(parts[0].c_str()), lineCommand)); // ADD Command to already created client
		}
	}

	void parse()
	{
		std::vector<std::string> lines = this->getFileContent();
		for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
		{
			if ((*it).substr(0, 1) == "#" || (*it).substr(0, 2) == "//" )
			{
				continue;
			}
			parseLine(*it);
		}
	}

	void list_files(const std::string &path, std::vector<std::string> &files)
	{
		DIR *dir = opendir(path.c_str());
		if (!dir)
		{
			std::cerr << "Error: could not open directory: " << path << std::endl;
			return;
		}
		dirent *entry;
		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_type == DT_DIR)
			{
				if (entry->d_name[0] == '.')
				{
					continue;
				}
				std::string subdir_path = path + "/" + entry->d_name;
				list_files(subdir_path, files);
			}
			else
			{
				std::string file_path = path + "/" + entry->d_name;
				files.push_back(file_path);
				std::cout << "file " << "\n";
			}
		}
		closedir(dir);
	}
};
