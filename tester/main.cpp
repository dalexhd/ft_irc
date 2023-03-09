#include "includes/File.hpp"
#include "includes/Colors.hpp"


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

static void *clientConversation(void *client)
{
	Client *c = (Client *)client;
	for (std::map<size_t, Command>::iterator it2 = c->_commands.begin(); // COMMAND LOOP
			     it2 != c->_commands.end(); ++it2)
	{
		if (!c->_connected)
		{
			usleep(it2->second._ms * 1000);
			if(c->irc_connect())
				return 0;
		}
		usleep(it2->second._ms * 1000);
		c->send(it2->second.getCommand());
		std::cout << VERDE_T << c->_name << " - " << it2->first << " ms " << it2->second.getCommand()  << RESET << std::endl;
		//std::cout << "*full cmd" << it2->second.getCommand() << std::endl;
	}
	usleep(700);
	c->irc_disconnect();
	usleep(700);
	return 0;
}

void executeFileClients(File *file)
{
	for (std::map<std::string, Client>::iterator it = (*file)._clients.begin(); // CLIENT LOOP
		     it != (*file)._clients.end(); ++it)
	{
		pthread_create(&it->second._thread, NULL, *clientConversation, &it->second);
	}
	for (std::map<std::string, Client>::iterator it = (*file)._clients.begin();
		     it != (*file)._clients.end(); ++it)
	{
		pthread_join(it->second._thread, NULL);
	}
}

int main(void)
{
	std::cout << "Send 1 to Single Client , other key to Full Auto Tester\n";
	std::string menu;
	std::cin >> menu;
	if (menu == "1")
	{
		Client *client = new Client("Testbot", "Testbot", "Testbot");
		if(!client->irc_connect())
		{
			usleep(1000);
			client->login("FirstClient");
			usleep(1000);
			client->requestingLoop();
		}
	}
	else{

		std::cout << "Automated Test\n";
		std::vector<std::string> files;

	list_files("./tests/new", files);
	for (std::vector<std::string>::const_iterator it = files.begin(); // ITERATE FILES
	     it != files.end(); ++it)
	{
		std::cout << AZUL_T << "PATH: " << *it << RESET <<  std::endl;

		File file(*it);
		file.parse();
		std::cout  << std::endl;

		executeFileClients(&file);
		usleep(700);
	}


	}

	return (0);
}
