#include "includes/Client.hpp"


void read ()
{

}

int main(int argc, char *argv[])
{
	std::string host = "127.0.0.1"; //nc -c irc.irc-hispano.org 6667
	std::string port = "6667";

	if (argc == 3)
	{
		host = argv[1];
		port = argv[2];
	}
	try
	{
		Client client = Client(host, port);
		std::cout << "Connected to " << client._host << ":" << client._port << std::endl;
		client.login("Testbot");
		client.requestingLoop();
		return (0);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
}
