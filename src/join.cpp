#include "../include/command.hpp"

static void sendJoinMessages(Client user, Channel channel) {
	Utils::sendIRCMessage(user.get_fd(), RPL_JOIN(user.get_nickname(), user.get_username(), channel.name));
	// Parse::sendIRCMessage(user.get_fd(), RPL_WELCOME(user.get_nickname(), channel.name));
	Utils::sendIRCMessage(user.get_fd(), RPL_TOPIC(user.get_nickname(), channel.name, channel.topic));
	// sendIRCMessage(socket_fd, RPL_TOPICWHOTIME(username, channel.name, channel.topicdate)); //fecha que no es y persona que no es
	//seguramente haya errores de confundir user y nick
}

static bool join_comp(Client user, const std::vector<std::string>& params, Channel channel)
{
// Existencia del Canal:
// Verificar que el nombre del canal sea válido según las reglas de IRC
// (generalmente comienza con # o & y cumple ciertas restricciones de longitud y caracteres).
// Si el canal no existe aún, el servidor puede crearlo en ese momento y
	if (params[1][0] != '#' || params[1].length() > 50)
	{
		Utils::sendIRCMessage(user.get_fd(), ERR_NOSUCHCHANNEL(user.get_nickname(), params[1]));
		return  true;
	}
// Límite de Usuarios (+l):
// Verificar si el canal tiene un límite máximo de usuarios configurado (modo +l). Si el canal está lleno,
// el servidor debería rechazar la solicitud de JOIN y notificar al cliente con un mensaje de error,
// como ERR_CHANNELISFULL.
	if (channel.isFull() == true && channel.maxClientsEnabled == true)
	{
		Utils::sendIRCMessage(user.get_fd(), ERR_CHANNELISFULL(user.get_nickname(), params[1]));
		return true;
	}
// Canal Restringido a Invitados (+i):
// Si el canal está en modo +i (solo por invitación), verificar si el usuario está en la lista de invitados.
// Si no lo está, el servidor debería rechazar la solicitud con el mensaje ERR_INVITEONLYCHAN.
	if (channel.invitationsEnabled == true) // y no tienes invitacion
	{
		Utils::sendIRCMessage(user.get_fd(), ERR_INVITEONLYCHAN(user.get_nickname(), params[1]));
		return true;
	}
// Contraseña del Canal (+k):
// Si el canal tiene una contraseña establecida (modo +k), el servidor debería verificar si el cliente ha
// proporcionado la contraseña correcta. Si la contraseña es incorrecta o no se proporciona, el servidor
// debería responder con ERR_BADCHANNELKEY.
	if (channel.haspswd == true && params[2] != channel.key)
	{
		Utils::sendIRCMessage(user.get_fd(), ERR_BADCHANNELKEY(user.get_nickname(), params[1]));
		return true;
	}
// Modo de Solo Lectura (+m):		//innecesario?
// Aunque el modo +m (modo moderado) no impide que un usuario entre al canal,
// el servidor debería hacer un seguimiento para que, si el cliente se une a un canal moderado,
// se le impida enviar mensajes si no tiene voz (+v) u operador (+o).
	// if (channel.onlyReadEnabled == true)
	// {

			// return true;
	// }
// Usuarios Duplicados:
// Verificar que el usuario no esté ya en el canal para evitar duplicados.
	if (Utils::searchUser(channel.clients, user) == true)
	{
		Utils::sendIRCMessage(user.get_fd(), ERR_USERONCHANNEL(user.get_nickname(), params[1]));
		return true;
	}
		
// Si todas las comprobaciones pasan, el servidor añade al usuario al canal y envía una notificación de JOIN a todos los miembros, junto con cualquier información relevante como los modos de canal y usuarios con permisos especiales.
	return false;
}

void *Command::join(Client user, const std::vector<std::string>& params, std::vector<Channel> &channels){
	auto it = std::find(channels.begin(), channels.end(), params[1]);
	if (it != channels.end())
	{
		Channel *wanted = &(*it);
		if (join_comp(user, params, *wanted) == true)
			return NULL;
		std::cout << "Usage: /join channel found." << std::endl;
		wanted->addClient(user);
		sendJoinMessages(user, *wanted);
		return NULL;
	}
	else
	{
		std::cerr << "Usage: /join channel not found. Creating..." << std::endl; //mensaje provisional
		Channel *newChan = new Channel;
		newChan->setName(params[1]); 
		newChan->topic = "[No topic]";
		// newChan->topicdate = std::to_string(time(nullptr));
		// newChan->addClient(user);
		newChan->oper.push_back(user);
		channels.push_back(*newChan);
		sendJoinMessages(user, *newChan);
		return NULL;
	}
}
