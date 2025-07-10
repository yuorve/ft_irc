#include "../include/command.hpp"


void Command::handle_Topic(Client user, const std::vector<std::string>& params, std::vector<Channel> &channels){
	// handle topic command
	if (params.size() < 2)
	{
		std::cerr << "Usage: /topic <message>" << std::endl;
		return ;
	}
	Channel *chWanted = Utils::searchChannel(channels, params[1]);
	if (chWanted->topicEnabled == true && Utils::searchUser(chWanted->oper, user) == false)
	{
		Utils::sendIRCMessage(user.get_fd(),ERR_CHANOPRIVSNEEDED(user.get_nickname(), chWanted->getName()));
		return ;
	}
	if (chWanted != NULL)
	{

		std::string newTopic = "";
		if (params.size() > 2)
		{
			newTopic = params[2].substr(1, params[2].length());
			for (int i = 3; i< params.size(); i++)
			newTopic += " " + params[i];
		}
		chWanted->topic = newTopic;
		// chWanted->topicdate = std::to_string(time(nullptr));
		newTopic =  RPL_TOPIC(user.get_nickname(), chWanted->name, chWanted->topic);
		for (int k = 0; k < chWanted->clients.size(); k++)
			Utils::sendIRCMessage(chWanted->clients[k].get_fd(), newTopic);
		for (int k = 0; k < chWanted->oper.size(); k++)
			Utils::sendIRCMessage(chWanted->oper[k].get_fd(), newTopic);
		return ;
	}
	else
	{
		std::cerr << "Usage: /topic channel not found" << std::endl; //mensaje provisional
		return ;
	}
}
