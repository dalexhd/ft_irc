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
	std::vector<std::string>			fileContent;
} t_info;

void parseFile(std::string filename)
{
	std::ifstream infile(std::string("./tests/spec/" + filename).c_str());
	// Parse info and save content into info structure
	for (std::string line; std::getline(infile, line);)
	{
		std::cout << line << std::endl;
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
	std::cout << a[0] << "\n";
	client->send("JOIN CHANNEL1");

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
			info.fileContent.push_back("JOIN CHAN");


			parseFile("test1");
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
