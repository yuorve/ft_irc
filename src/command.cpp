#include "../include/parse.hpp"

#include <iostream>

// void Parse::message(const std::string& message, std::string& command, std::string& content) {
//     std::vector<std::string> tokens = parse(message, ' ');
//     command = tokens[0];
//     tokens.erase(tokens.begin());
//     content = join(tokens, ' ');
// }

std::vector<std::string> Parse::split(const std::string& str){
	std::vector<std::string> tokens;
	std::vector<std::string> commands = {"JOIN", "KICK", "INVITE", "MODE", "NICK"};
	int i = 0;
	if (str.find("CAP LS") == 0 && str.length() != 10)
		i = 12;
	int aux;
	int k;
	while (str[i] && i < str.length())
	{
		k = 0;
		aux = i;
		if (str[i] != ' ' && str[i] != '\0' && i < str.length())
		{
			while (str[aux] != ' ' && str[aux] != '\0' && aux < str.length())
			{
				k++;
				aux++;
			}
			tokens.push_back(str.substr(i, k));
			if (tokens.size() == 1 && tokens[0] == "TOPIC")
			{
				tokens.push_back(str.substr(6, str.length()));
				return tokens;
			}
			if (tokens.size() == 1
				&& std::find(commands.begin(), commands.end(), tokens[0]) == commands.end())
			{
				tokens[0] = str;
				return tokens;
			}
			i = k + i;
		}
		else
			i++;
	}
	i = 0;
	// while (i < tokens.size())   //comprueba que parsee todas las palabras correctamente en caso de ser un comando
	// {
	// 	std::cout << "token " << i << " : " << tokens[i] << std::endl;
	// 	i++;
	// }
	return tokens;
}



void Command::nick(int client_fd, const std::string& params, std::vector<Client> &users)
{
	Client newUser;
	newUser.set_fd(client_fd);
	newUser.set_nickname(params);
	users.push_back(newUser);
}

void Command::user(Client *client, const std::string& params)
{
	client->set_username(params);
}

void Command::handle_Invite(const std::vector<std::string>& params){
	// handle invite command
	// Pasos a seguir
	// Comprobar que el Canal existe

	// Verificar que el invitador está en el canal
	// Verificar que el usuario invitado existe
	// Verificar permisos si el canal es invite-only
	// Añadir al usuario a la lista de invitados del canal
	// Hay que crear un vector o algo con los usuarios invitados al canal
	// Hay que modificar el comando JOIN ya que luego el usuario tiene que hacer JOIN y habrá que eliminarlo de la lista de invitados
}

void Command::handle_who(Channel &channel)
{
	std::string users = "";
	for (int i = 0; i < channel.oper.size(); i++)
		users = users + "@" + channel.oper[i].get_username() + " ";
	for (int i = 0; i < channel.clients.size(); i++)
		users = users + channel.clients[i].get_username() + " ";
	for (int i = 0; i < channel.clients.size(); i++)
	{
		Utils::sendIRCMessage(channel.clients[i].get_fd(), RPL_NAMREPLY(channel.clients[i].get_nickname(), channel.name, users));
		Utils::sendIRCMessage(channel.clients[i].get_fd(), RPL_ENDOFNAMES(channel.clients[i].get_nickname(), channel.name));
	}
	for (int i = 0; i < channel.oper.size(); i++)
	{
		Utils::sendIRCMessage(channel.oper[i].get_fd(), RPL_NAMREPLY(channel.oper[i].get_nickname(), channel.name, users));
		Utils::sendIRCMessage(channel.oper[i].get_fd(), RPL_ENDOFNAMES(channel.oper[i].get_nickname(), channel.name));
	}
}

void Command::handle_part(Client &client, Channel &channel)
{
	Client wanted = Utils::nickSearch(channel.clients, client.get_nickname());
	if (wanted.get_username() != "notfound")
	{
		channel.removeClient(wanted);
		return ;
	}
	else
		wanted = Utils::nickSearch(channel.oper, client.get_nickname());
	if (wanted.get_username() != "notfound")
		channel.removeOper(wanted);
	else
		std::cerr << "Error: user parting not found\n";
	return ;

}

void Command::handle_privmsg(int client_fd, const std::string& parameters, std::vector<Client> &users, std::vector<Channel> &channels) {
    Client *client = NULL;
	client = Utils::fdSearch(users, client_fd);
	// Separar los parámetros
    size_t pos = parameters.find(' ');
    if (pos == std::string::npos) {
        // Error: faltan parámetros				
		Utils::sendIRCMessage(client->get_fd(), ERR_PRIVMSG(client->get_nickname()));
        return;
    }

    std::string recipients_str = parameters.substr(0, pos);
    std::string message = parameters.substr(pos + 1);

    // Eliminar ':' inicial del mensaje si existe
    if (!message.empty() && message[0] == ':') {
        message.erase(0, 1);
    }

    if (recipients_str.empty() || message.empty()) {
        // Error: faltan parámetros
		Utils::sendIRCMessage(client->get_fd(), ERR_TOOMANYRECIP(client->get_nickname()));
        return;
    }

    // Separar los destinatarios por comas
    std::vector<std::string> recipients;
    std::stringstream ss(recipients_str);
    std::string recipient;
    while (std::getline(ss, recipient, ',')) {
        recipients.push_back(recipient);
    }

    // Límite de destinatarios
    const int TARGMAX_PRIVMSG = 4; // Por ejemplo, límite de 4 destinatarios, se puede definir dentro de la clase server por ahora lo dejo aquí
    if (recipients.size() > TARGMAX_PRIVMSG) {
		Utils::sendIRCMessage(client->get_fd(), ERR_TOOMANYRECIP(client->get_nickname()));
        return;
    }

    // Procesar cada destinatario
    for (size_t i = 0; i < recipients.size(); ++i) {
        std::string receiver = recipients[i];

		Channel *wChannel = Utils::searchChannel(channels, receiver);
		
        // Verificar si el receptor es un usuario
        if (Utils::searchUserNick(users, receiver)) {
			Client cWanted = Utils::nickSearch(users, receiver);
            std::string msg_to_send = ":" + client->get_prefix() + " PRIVMSG " + receiver + " :" + message + "\r\n";
            send(cWanted.get_fd(), msg_to_send.c_str(), msg_to_send.length(), 0);
        }
        // Verificar si el receptor es un canal
        else if (wChannel != NULL) {
            // Aquí hay que verificar permisos, modos, creo que no todos pueden enviar un broadcasting o eso mejor hacerlo en la función broadcast
			// ...			
            std::string msg_to_send = ":" + client->get_prefix() + " PRIVMSG " + receiver + " :" + message + "\r\n";
			// Envia el mensaje a todos los usuarios en el canal excepto al emisor
            wChannel->broadcast(msg_to_send, client->get_fd());
        }
        else {
            // Error: No existe el nick o canal
			Utils::sendIRCMessage(client->get_fd(), ERR_NOSUCHNICK(client->get_nickname(), " ", receiver));
        }
    }
}

