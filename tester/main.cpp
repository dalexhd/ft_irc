#include "includes/Client.hpp"
#include <fstream>
#include <map>
#include <pthread.h>
#include <semaphore.h>
#include <sstream>
#include <string>



std::string host = "127.0.0.1"; // nc -c irc.irc-hispano.org 6667
std::string port = "6667";
sem_t           sem1;
int             count = 0;
pthread_mutex_t mutex;

typedef struct s_info
{
	int                                num_clients;
	std::vector<pthread_t>             clientThreads;
	std::map<std::string, Client*>		clients;
	std::vector<std::string>			fileContent; //name timestamp command
} t_info;

void parseFile(t_info *info,std::string filename)
{
	std::ifstream infile(std::string("./tests/spec/" + filename).c_str());
	// Parse info and save content into info structure
	for (std::string line; std::getline(infile, line);)
	{
		//vtr.push_back("cornflower");

		std::cout << line << std::endl;
		std::string delimiter = "-";

		size_t pos = 0;
		size_t i = 0;
		size_t ts = 0;

		std::string token;
		while ((pos = line.find(delimiter)) != std::string::npos) {
			token = line.substr(0, pos);
			switch(i){
				case 0:
					ts = stoi(token);
				case 1: //name

					if(info->clients.count(token) > 0)
					{
						std::cout << "Already exists " << token << std::endl;
					}
					else{
						std::cout << "Creating user " << token << std::endl;
						Client *client = new Client(host, port);
						client->login(token);
						info->clients.insert(std::make_pair( token, client ));
					}
			}

			//std::cout << pos << token << std::endl;
			line.erase(0, pos + delimiter.length());
			i++;
		}
		info->clients[token]->_commands.insert(std::make_pair( ts , line ));
	}


	// example
	//info->fileContent.insert(std::make_pair("nick1", "JOIN #canal1"));
	//info->fileContent.insert(std::make_pair("nick2", "JOIN #canal2"));
	//info->num_clients = 2;
}

static void *clientConversation(void *info)
{
	std::vector<std::string> *a = (std::vector<std::string> *)info;
	Client *client = new Client(host, port);
	client->login("FirstClient");
	std::cout << "Command" << a[0][0] << "\n";
	client->send(a[0][0]);
	std::cout << client->reads() << std::endl;

	for (int i = 0; i < 10; i++)
	{

	}
	return (0);
}

void createClients(t_info *info)
{
	int i;

	for (i = 0; i < info->num_clients; i++)
	{
		pthread_t a;
		info->clientThreads.push_back(a);
		pthread_create(&info->clientThreads[i], NULL, *clientConversation, &info->fileContent);
	}
	for (i = 0; i < info->num_clients; i++)
		pthread_join(info->clientThreads[i], NULL);
}

int main(int argc, char *argv[])
{


	// pthread_mutex_t	*r_fork;

	if (argc == 3)
	{
		host = argv[1];
		port = argv[2];
	}
	try
	{
		std::cout << "Send 1 to manual Client , other key to auto tester\n";
		std::string menu;
  		std::cin >> menu;
		if (menu == "1")
		{
			Client *client = new Client(host, port);
			client->login("FirstClient");
			client->requestingLoop();

		}
		else{

			std::cout << "Automated Test\n";
			t_info info;
			info.num_clients = 2;
			info.fileContent.push_back("JOIN #CHAN");


			parseFile(&info,"test1");
			createClients(&info);
			std::cout << count;

		}


	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
}
