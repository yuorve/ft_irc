#include "../include/parse.hpp"

static void sendKickMessage(Client &wanted, const std::vector<std::string>& params, Channel &channel)
{
	std::string reason = "";
	for (int i = 3; i < params.size() - 1; i++)
		reason = reason + params[i] + " ";
	reason = reason + params[params.size() - 1];
	std::string kickMessage = RPL_KICK(wanted.get_username(), params[1], reason);

	std::string users = "";
	for (int i = 0; i < channel.oper.size(); i++)
		users = users + "@" + channel.oper[i].get_username() + " ";
	for (int i = 0; i < channel.clients.size(); i++)
		users = users + channel.clients[i].get_username() + " ";
	for (int i = 0; i < channel.clients.size(); i++)
		Utils::sendIRCMessage(channel.clients[i].get_fd(), kickMessage);
	for (int i = 0; i < channel.oper.size(); i++)
		Utils::sendIRCMessage(channel.oper[i].get_fd(), kickMessage);
}

void Command::handle_Kick(Client client, const std::vector<std::string>& params, Channel &channel){
	if (params.size() < 3){
		std::cerr << "Usage: /kick <user>" << std::endl;
		return ;
	}
	Client isoper = Utils::nickSearch(channel.oper, client.get_nickname());
	if (isoper.get_username() != "notfound")
	{
		if (&channel != NULL)
		{
			Client wanted = Utils::nickSearch(channel.clients, params[2]);
			if (wanted.get_username() != "notfound")
			{
				sendKickMessage(wanted, params, channel);
				channel.removeClient(wanted);
				return ;
			}
			else
				wanted = Utils::nickSearch(channel.oper, params[2]);
			if (wanted.get_username() != "notfound")
			{
				sendKickMessage(wanted, params, channel);
				channel.removeOper(wanted);
			}
			else
				std::cerr << "Error: user kicked not found\n";
			return ;
		}
		else
		{
			std::cerr << "Usage: /kick channel not found" << std::endl; //mensaje provisional
			return ;
		}
	}
	// mensaje de no tener permisos
}
