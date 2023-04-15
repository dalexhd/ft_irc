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
			usleep(it2->second._ms * 10);
			if (c->irc_connect() > 0)
				return 0;
			c->login();
		}
		usleep(it2->second._ms + 700);
		c->send(it2->second.getCommand());
		std::cout << VERDE_T << c->_name << " - " << it2->first << " ms "
		          << it2->second.getCommand() << RESET << std::endl;

		std::string tmpread(c->reads());
		// WRITE TO A FILE
		std::ofstream myfile;
		/*myfile.open("tests/res" + c->_filename.substr(12, 99),
		std::ios_base::app);
		myfile << tmpread;
		myfile.close();*/
		usleep(700);
		std::cout << tmpread << std::endl;
	}

	usleep(700);
	c->send("QUIT");
	c->irc_disconnect();
	usleep(700);
	return 0;
}

void executeFileClients(File *file)
{
	for (std::map<std::string, Client>::iterator it = (*file)._clients.begin(); // CLIENT LOOP
	     it != (*file)._clients.end(); ++it)
	{
		it->second._filename = file->_name;
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

int main(void)
{
	std::string option(getenv("TYPE"));

	std::cout << "        " << ROJO_F << "Tester Options" << RESET << std::endl;
	std::cout << VERDE_T << "- - - - - - - - - - - - - - - - -" << RESET << std::endl;
	std::cout << "1. Connect to Local Server" << std::endl;
	std::cout << "2. Connect to Chat Hispano Server" << std::endl;
	std::cout << "3. Automated Test" << std::endl << std::endl << ">> ";

	// std::cin >> option; // comment for coverage
	if (option == "1" || option == "2")
	{
		std::string server = "127.0.0.1";
		if (option == "2")
			server = "irc.irc-hispano.org";
		std::cout << "Single Client to " + server + "\n";
		Client *client = new Client("Testbot", "Testbot", "Testbot", server);
		if (client->irc_connect())
			return 1;
		usleep(500);
		client->login();
		usleep(500);
		client->requestingLoop();
	}
	else
	{
		std::cout << "Automated Test\n";
		std::vector<std::string> files;

		list_files("./tests/spec", files);
		createResTest();
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
