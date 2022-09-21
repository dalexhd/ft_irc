#include "includes/Client.hpp"

int main(int argc, char *argv[])
{
	std::string host = "127.0.0.1";
	std::string port = "6667";

	if (argc == 3)
	{
		host = argv[1];
		port = argv[2];
	}
	try
	{
		std::string response;

		Client client = Client(host, port);
		std::cout << "Connected to " << client._host << ":" << client._port << std::endl;
		client.send("NICK TestBot");
		usleep(1000);
		client.send("USER TestBot 0 * :TestBot");
		usleep(1000);
		client.send("quit");
		usleep(1000);
		response = client.read();
		std::cout << response << std::endl;
		return (0);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
}
