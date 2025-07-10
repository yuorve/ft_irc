#include "../include/command.hpp"

void Command::handle_Mode(Client user, const std::vector<std::string>& params,
							std::vector<Channel> &channels){
	if (params.size() == 2)
	{
		auto it = std::find(channels.begin(), channels.end(), params[1]);
		if (it != channels.end())
		{
			Channel wanted = *it;
			Utils::sendIRCMessage(user.get_fd(), RPL_CHANNELMODEIS(user.get_username(), wanted.getName())); //modificar valores dependiendo de los permisos del canal
			std::string local = "localhost";
			for (int i = 0; i < wanted.oper.size(); i++)				
				Utils::sendIRCMessage(user.get_fd(), RPL_MODE(local, local, wanted.getName(), "+o", wanted.oper[i].get_username()));
			return ;
		}
		else
		{
			std::cerr << "Error: bad arguments" << std::endl;
			return ;
		}
		
	}
	// if (params.size() < 4)
	// {
	// 	std::cerr << "Error in /MODE" << std::endl;
	// 	return ;
	// }
	Channel *chWanted = Utils::searchChannel(channels, params[1]);
	if (Utils::searchUser(chWanted->oper, user) == false)
	{
		std::cout << "operator not found\n";
		Utils::sendIRCMessage(4, ERR_CHANOPRIVSNEEDED(user.get_nickname(), chWanted->getName()));
		return ;
	}

	//al añadir(+o) a los usuarios artificiales peta?
	if (params[2] == "+o")
	{
		Client wanted = Utils::nickSearch(chWanted->clients, params[3]);
		if (wanted.get_username() == "notfound" || Utils::searchUser(chWanted->oper, user) == false)
			return ;		//mensaje?
		if (Utils::searchUser(chWanted->oper, user) == false)
		Utils::sendIRCMessage(4, RPL_MODE(user.get_nickname(), user.get_username(), chWanted->getName(), params[2], wanted.get_username()));
		chWanted->oper.push_back(wanted);
		for (int i = 0; i < chWanted->oper.size(); i++)
			std::cout << chWanted->oper[i].get_nickname() << " ";
		chWanted->removeClient(wanted);
		handle_who(*chWanted);
	}
	if (params[2] == "-o")
	{
		Client wanted = Utils::nickSearch(chWanted->oper, params[3]);
		if (wanted.get_username() == "notfound" || Utils::searchUser(chWanted->oper, user) == false)
			return ;		//mensaje?
		Utils::sendIRCMessage(4, RPL_MODE(user.get_nickname(), user.get_username(), chWanted->getName(), params[2], wanted.get_username()));
		chWanted->removeOper(wanted);
		chWanted->addClient(wanted);
		handle_who(*chWanted);
	}
	if (params[2] == "+t")
		chWanted->topicEnabled = true;
	else if (params[2] == "-t")
		chWanted->topicEnabled = false;
	// NOT TESTED
	else if (params[2] == "+l")
		chWanted->maxClientsEnabled = true;
	else if (params[2] == "-l")
		chWanted->maxClientsEnabled = false;
	else if (params[2] == "+i")
		chWanted->invitationsEnabled = true;
	else if (params[2] == "-i")
		chWanted->invitationsEnabled = false;
	else if (params[2] == "+k")
		chWanted->haspswd = true;
	else if (params[2] == "-k")
		chWanted->haspswd = false;
}