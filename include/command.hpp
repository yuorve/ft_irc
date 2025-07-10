#pragma once

#include "server.hpp"
#include "utils.hpp"

class Command {
	public:
		static void *join(Client user, const std::vector<std::string>& params, std::vector<Channel> &channels);
        static void nick(int client_fd, const std::string& params, std::vector<Client> &users);
        static void user(Client *client, const std::string& params);
		static void handle_Kick(Client client, const std::vector<std::string>& params, Channel &channel);
        static void handle_Invite(const std::vector<std::string>& params);
        static void handle_Topic(Client user, const std::vector<std::string>& params, std::vector<Channel> &channels);
        static void handle_Mode(Client user, const std::vector<std::string>& params, std::vector<Channel> &channels);
        static void handle_who(Channel &channel);
        static void handle_part(Client &client, Channel &channel);
        static void handle_privmsg(int client_fd, const std::string& parameters, std::vector<Client> &users, std::vector<Channel> &channels);
};