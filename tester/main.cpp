#include "includes/Colors.hpp"
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

static void *clientConversation(void *client)
{
	Client *c = (Client *) client;
	for (std::map<size_t, Command>::iterator it2 = c->_commands.begin(); // COMMAND LOOP
	     it2 != c->_commands.end(); ++it2)
	{
		if (!c->_connected)
		{
			std::cout << c->_name << " is not connected, trying to connect\n";
			usleep(it2->second._ms * 1000);
			if (c->irc_connect() > 0)
				return 0;
			c->login();
		}
		usleep(it2->second._ms * 1000);
		c->send(it2->second.getCommand());
		std::cout << VERDE_T << c->_name << " - " << it2->first << " ms "
		          << it2->second.getCommand() << RESET << std::endl;
		usleep(700);

		std::ofstream myfile;
		myfile.open("tests/res/privmsg/privmsg_1", std::ios_base::app);
		myfile << c->reads();
		std::cout << "Server response" << c->reads() << std::endl;
		myfile.close();
		usleep(700);
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

/*
    Buscar IRC consistente, descargar ejecutable para ir probando outouts
     y guardarlos en la respuestas de los test, buscar segfaults,
    crear Diff de nuestro output con el archivo de output de un IRC bien hecho
*/
int main(void)
{
	std::cout << "Send 1 to Single Client , other key to Full Auto Tester\n";
	/* 	std::string menu;
	    std::cin >> menu;
	    if (menu == "1" ||  menu == "2")
	    {
			std::string server;
			if(menu == "1")
				server = "irc.irc-hispano.org";
			else
				server = "127.0.0.1";

	        Client *client = new Client("Testbot", "Testbot", "Testbot",
	   "irc.irc-hispano.org"); client->irc_connect(); usleep(1000);
	        client->login();
	        usleep(1000);
	        client->requestingLoop();
	    }
	    else
	    { */
	std::cout << "Automated Test\n";
	std::vector<std::string> files;

	list_files("./tests/new", files);
	for (std::vector<std::string>::const_iterator it = files.begin(); // ITERATE FILES
	     it != files.end(); ++it)
	{
		std::cout << AZUL_T << "PATH: " << *it << RESET << std::endl;

		File file(*it);
		file.parse();
		std::cout << std::endl;

		executeFileClients(&file);
		usleep(700);
	}

	return (0);
}
