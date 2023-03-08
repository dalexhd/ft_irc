#include "includes/File.hpp"

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
		}
	}
	closedir(dir);
}

int main(void)
{
	std::vector<std::string> files;

	list_files("./tests/new", files);
	for (std::vector<std::string>::const_iterator it = files.begin();
	     it != files.end(); ++it)
	{
		std::cout << "PATH: " << *it << std::endl;

		File file(*it);
		file.parse();
		std::cout  << std::endl;

		for (std::map<std::string, Client>::iterator it = file._clients.begin(); // CLIENT LOOP
		     it != file._clients.end(); ++it)
		{
			std::cout << it->second << std::endl;
			for (std::map<size_t, Command>::iterator it2 = it->second._commands.begin(); // COMMAND LOOP
			     it2 != it->second._commands.end(); ++it2)
			{
				if (!it->second._connected)
				{
					usleep(it2->second._ms * 1000);
					if(it->second.irc_connect() > 0)
						return (1);
				}
				usleep(it2->second._ms * 1000);
				it->second.send(it2->second.getCommand());
				//std::cout << "*full cmd" << it2->second.getCommand() << std::endl;
			}
			std::cout << std::endl << std::endl;
		}
		for (std::map<std::string, Client>::iterator it = file._clients.begin();
		     it != file._clients.end(); ++it)
		{
			usleep(5000);
			it->second.irc_disconnect();
		}

		usleep(700);
	}
	return (0);
}
