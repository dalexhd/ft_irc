#include "includes/Client.hpp"
#include <fstream>
#include <map>
#include <pthread.h>
#include <semaphore.h>
#include <sstream>
#include <string>

sem_t           sem1;
int             count = 0;
pthread_mutex_t mutex;
typedef struct s_info
{
	int                                num_clients;
	std::vector<pthread_t>             clientThreads;
	std::map<std::string, std::string> fileContent;
} t_info;

void parseFile(t_info *info, std::string filename)
{
	std::ifstream infile(std::string("./tests/spec/" + filename).c_str());
	// Parse info and save content into info structure
	for (std::string line; std::getline(infile, line);)
	{
		std::cout << line << std::endl;
	}

	// example
	info->fileContent.insert(std::make_pair("nick1", "JOIN #canal1"));
	info->fileContent.insert(std::make_pair("nick2", "JOIN #canal2"));
	info->num_clients = 2;
}

static void *clientConversation(void *)
{
	for (int i = 0; i < 5000; i++)
	{
		pthread_mutex_lock(&mutex);
		count++;
		pthread_mutex_unlock(&mutex);
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
		pthread_create(&info->clientThreads[i], NULL, *clientConversation, NULL);
	}
	for (i = 0; i < info->num_clients; i++)
		pthread_join(info->clientThreads[i], NULL);
}

int main(int argc, char *argv[])
{
	std::string host = "127.0.0.1"; // nc -c irc.irc-hispano.org 6667
	std::string port = "6667";

	// pthread_mutex_t	*r_fork;

	if (argc == 3)
	{
		host = argv[1];
		port = argv[2];
	}
	try
	{
		t_info info;
		info.num_clients = 2;
		// sem_init(&sem1, 0, 1);

		parseFile(&info, "test1");
		createClients(&info);
		std::cout << count;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
}
