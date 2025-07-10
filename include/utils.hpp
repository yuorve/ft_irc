#pragma once

#include "server.hpp"

class Utils
{
	public:
		Utils();
		~Utils();
		static Client *fdSearch(std::vector<Client> &users, int client_fd);
		static Client nickSearch(std::vector<Client> users, std::string nick);
		static bool searchUser(std::vector<Client> users, Client wanted);
		static bool searchUserNick(std::vector<Client> users, std::string wanted);
		static Channel *searchChannel(std::vector<Channel> &channels, std::string chName);
		void message(const std::string& message, std::string& command, std::string& content);
		static void sendIRCMessage(int socket_fd, const std::string& message);
};