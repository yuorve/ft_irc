#include "../include/server.hpp"
#include "../include/parse.hpp"
#include "../include/utils.hpp"

void Utils::sendIRCMessage(int socket_fd, const std::string& message) {
	int status = send(socket_fd, message.c_str(), message.length(), 0);
	if (status == -1) {
		std::cerr << "Error al enviar mensaje: " << strerror(errno) << std::endl;
	}
}

bool Utils::searchUser(std::vector<Client> users, Client wanted)
{
	int i = 0;
	while (i < users.size())
	{
		if (users[i].get_nickname() == wanted.get_nickname())
				return true;
		i++;
	}
	std::cerr << "searchUser: user not found" << std::endl;
	return false;
}

bool Utils::searchUserNick(std::vector<Client> users, std::string wanted)
{
	int i = 0;
	while (i < users.size())
	{
		if (users[i].get_nickname() == wanted)
				return true;
		i++;
	}
	std::cerr << "searchUserNick: user " << wanted << " not found " << std::endl;	
	return false;
}

Channel *Utils::searchChannel(std::vector<Channel> &channels, std::string chName)
{
	auto it = std::find(channels.begin(), channels.end(), chName);
	if (it != channels.end())
	{
		Channel *wanted = &(*it);
		//std::cout << wanted->getName();
		return wanted;
	}
	std::cerr << "Error: Channel " << chName << " not found" << std::endl;
	return NULL;
}

Client *Utils::fdSearch(std::vector<Client> &users, int client_fd)
{
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].get_fd() == client_fd)
		{
			// std::cout << "fdsearch fd found " << client_fd << std::endl;
			return &users[i];
		}
	}
	std::cerr << "Error: user not found" << std::endl;
	return  NULL;
}

Client Utils::nickSearch(std::vector<Client> users, std::string nick)
{
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].get_nickname() == nick)
			return users[i];
	}
	std::cerr << "Error: nick " << nick << " not found " << std::endl;
	Client notfound("notfound");
	return notfound;
	//return NULL;
}
