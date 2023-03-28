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
		usleep(700);

		if (!c->_connected)
		{
			usleep(it2->second._ms);
			if (c->irc_connect() > 0)
				return 0;
			c->login();
		}
		usleep(it2->second._ms);
		c->send(it2->second.getCommand());
		std::cout << VERDE_T << c->_name << " - " << it2->first << " ms "
		          << it2->second.getCommand() << RESET << std::endl;
		/*std::ofstream myfile;
		myfile.open("tests/res/privmsg/privmsg_1", std::ios_base::app);
		myfile << c->reads();
		myfile.close();*/
		std::cout << c->reads() << std::endl;
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
    quit automaticos al final de cata file test
    Añadir test de todos los comando principales
*/

#define MENU 6
int main(void)
{
	if (MENU == 1 || MENU == 2)
	{
		std::cout << "Single Client\n";
		std::string server;
		if (MENU == 1)
			server = "irc.irc-hispano.org";
		else
			server = "127.0.0.1";

		Client *client = new Client("Testbot", "Testbot", "Testbot", server);
		client->irc_connect();
		usleep(500);
		client->login();
		// std::cout <<  client->reads() << std::endl;
		usleep(500);
		client->requestingLoop();
	}
	else
	{
		std::cout << "Automated Test\n";
		std::vector<std::string> files;

		list_files("./tests/spec", files);
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
	}
	Client *client = new Client("Testbot", "Testbot", "Testbot");
	client->irc_connect();
	usleep(1000);
	client->login();
	usleep(1000);
	client->send("close");
	return (0);
}
