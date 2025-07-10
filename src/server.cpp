/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoropeza <yoropeza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 18:33:05 by yoropeza          #+#    #+#             */
/*   Updated: 2024/12/10 12:54:09 by yoropeza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"

Server::Server(Server const & src) {
	*this = src;
}

Server::Server() : server_socket(-1) { }

Server::~Server() { }

int	Server::ft_stoi(char const *str)
{
	int i;
	int sign;
	int n;

	i = 0;
	sign = 1;
	n = 0;
	while (((str[i] >= 9 && str[i] <= 13) || str[i] == 32) && str[i])
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while ((str[i] >= '0' && str[i] <= '9') && str[i])
	{
		n = n * 10 + (str[i] - '0');
		i++;
	}
	return (n * sign);
}

Server	& Server::operator=(Server const & rhs) {
	if (this != &rhs)
		*this = rhs;
	return *this;
}

int	Server::checkPort(const std::string port)
{
	for (size_t i = 0; i < port.length(); i++)
	{
		if (!std::isdigit(port[i]))
			return (0);
	}
	int num = ft_stoi(port.c_str());
	// Valid ports is 1024 or above
	if (num < 1024 || num > 65535)
		return (0);
	return (1);
}

int	Server::checkPass(const std::string pass)
{
    // Must has more of 8 characters
	if (pass.length() > 8)    
        return (1);
	return (0);
}

int Server::create_server_socket(int port) {
    sockaddr_in sa;
    int status;

    // Prepare the address and port for the server socket
    std::memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;                 // IPv4
    sa.sin_addr.s_addr = htonl(INADDR_ANY);  // any local machine address
    sa.sin_port = htons(port);        // Setting Port

    // Create listening socket
    server_socket = socket(sa.sin_family, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "[Server] Socket error: " << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "[Server] Created server socket fd: " << server_socket << std::endl;

    // Set SO_REUSEADDR option to prevent errors when address is used
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "[Server] setsockopt(SO_REUSEADDR) failed: " << strerror(errno) << std::endl;
        close(server_socket);
        return -1;
    }

	// set the socket option (O_NONBLOCK) for non-blocking socket
	if (fcntl(server_socket, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("failed to set option (O_NONBLOCK) on socket"));

    // Bind socket to address and port
    status = bind(server_socket, reinterpret_cast<struct sockaddr*>(&sa), sizeof(sa));
    if (status != 0) {
        std::cerr << "[Server] Bind error: " << strerror(errno) << std::endl;
        close(server_socket);
        return -1;
    }
    std::cout << "[Server] Bound socket to localhost port " << port << std::endl;

    return server_socket;
}

void Server::accept_new_connection() {
    char buffer[32];
    int client_fd = accept(server_socket, NULL, NULL);
    
    if (client_fd == -1) {
        std::cerr << "[Server] Accept error: " << strerror(errno) << std::endl;
        return;
    }
    add_to_poll_fds(client_fd);

    std::cout << "[Server] Accepted new connection on client socket " << client_fd << "." << std::endl;

    sprintf(buffer, "%d", client_fd);
    std::string msg_to_send = "Welcome. You are client fd [" + std::string(buffer) + "]\n";
    
    int status = send(client_fd, msg_to_send.c_str(), msg_to_send.length(), 0);
    if (status == -1) {
        std::cerr << "[Server] Send error to client " << client_fd << ": " << strerror(errno) << std::endl;
    }
}

void Server::handle_client_command(int client_fd, const std::vector<std::string>& tokens, bool& running) {
    std::cout << "[" << client_fd << "] Got command: |" << tokens[0] << "|" << std::endl;
    std::string command = tokens[0]; // Identify the command (assuming the first token is the command)
    std::string parameters = "";
    for (size_t i = 1; i < tokens.size(); ++i) {
        parameters += tokens[i];
        if (i != tokens.size() - 1)
            parameters += " ";
    }
    parameters.erase(parameters.find_last_not_of("\r") + 1); // Remove end of line characters
    std::cout << "[" << client_fd << "] Got message: |" << parameters << "|" << std::endl;    
    std::string response = "";
    if (command == "EXIT") {
        running = false;
        std::cout << "[" << client_fd << "] Ending Server ";
    }
    Client *cWanted = NULL;
    Channel *wChannel = NULL;
    //     for (char c : tokens[1]) {   //search for \r
    //     std::cout << "Carácter: " << c << " Código ASCII: " << (int)c << std::endl;
    // }
    if (command == "CAP") {
        response = "CAP * NAK :LS\r\n";
    }
    else if (command == "PASS") {
        if (parameters == server_password) {
            std::cout << "Valid password" << std::endl;
            // No Response is needed
        }
        else {
            std::cout << "Invalid password" << std::endl;
            // Error 464 : Invalid password
            Utils::sendIRCMessage(client_fd, ERR_PASSWDMISMATCH());
        }
    }
    else if (command == "NICK") {
        // No Response is needed
        Command::nick(client_fd, tokens[1], this->users);
    }
    else if (command == "USER") {
        // Response and code here, function needed
        cWanted = Utils::fdSearch(this->users, client_fd);
        Command::user(cWanted, tokens[1]);
        response = RPL_WELCOME(cWanted->get_nickname());
    }
    else if (command == "JOIN") {
        // Responses in join
        cWanted = Utils::fdSearch(this->users, client_fd);
        Command::join(*cWanted, tokens, this->channels);
    }
    else if (command == "MODE") {
        // Response and code here
        cWanted = Utils::fdSearch(this->users, client_fd);
        Command::handle_Mode(*cWanted, tokens, this->channels);
    }
    else if (command == "WHO") {
        // Response and code here
        Command::handle_who(*Utils::searchChannel(this->channels, tokens[1]));
    }
    else if (command == "PART") {
        // Response and code here
        cWanted = Utils::fdSearch(this->users, client_fd);
        wChannel = Utils::searchChannel(this->channels, tokens[1]);
        Command::handle_part(*cWanted, *wChannel);
        Command::handle_who(*wChannel);
    }
    else if (command == "KICK") {
        // Response and code here
        cWanted = Utils::fdSearch(this->users, client_fd);
        wChannel = Utils::searchChannel(this->channels, tokens[1]);
        Command::handle_Kick(*cWanted, tokens, *wChannel);
		Command::handle_who(*wChannel);
    }
    else if (command == "TOPIC") {
        // Response and code here
        cWanted = Utils::fdSearch(this->users, client_fd);
        Command::handle_Topic(*cWanted, tokens, this->channels);
    }
    else if (command == "INVITE") {
        // Response and code here
        // Obtener la información del invitador
        cWanted = Utils::fdSearch(this->users, client_fd);
        // Comprobar que el Canal existe
        wChannel = Utils::searchChannel(this->channels, tokens[2]);
        if (!wChannel)
        {
            // Error 403 : No existe el canal            
            Utils::sendIRCMessage(client_fd, ERR_NOSUCHCHANNEL(cWanted->get_nickname(), tokens[2]));
            return;
        }
        // Verificar que el invitador está en el canal
        if (!Utils::searchUserNick(wChannel->clients, cWanted->get_nickname()))
        {
            // Error 442: No estás en el canal
            Utils::sendIRCMessage(client_fd, ERR_NOTONCHANNEL(cWanted->get_nickname(), tokens[2]));
            return;
        }
        // Comprobar que el invitado existe
        if (!Utils::searchUserNick(this->users, tokens[1]))
        {
            // Error 401: No existe el usuario
            Utils::sendIRCMessage(client_fd, ERR_NOSUCHNICK(cWanted->get_nickname(), tokens[1], tokens[2]));
            return;
        }         
        // Verificar permisos si el canal es invite-only y el invitador un operador
        if (wChannel->invitationsEnabled && !Utils::searchUserNick(wChannel->oper, cWanted->get_nickname()))
        {
            // Error 482: No tienes permiso para invitar
            Utils::sendIRCMessage(client_fd, ERR_CHANOPRIVSNEEDED(cWanted->get_nickname(), tokens[2]));
            return;
        }
        // Obtener la información del Invitado
        Client cGuest = Utils::nickSearch(this->users, tokens[1]);
        // Añadir al usuario a la lista de invitados del canal
        wChannel->addGuest(cGuest);
        // Mensaje enviado al usuario que ha sido invitado
        Utils::sendIRCMessage(cGuest.get_fd(), RPL_INVITE(cWanted->get_nickname(), cWanted->get_username(), tokens[1], tokens[2]));
        // Respuesta enviada al usuario invitador
        Utils::sendIRCMessage(client_fd, RPL_INVITING(cWanted->get_nickname(), cWanted->get_username(), tokens[1], tokens[2]));
        //Command::handle_Invite(tokens);
    }
    else if (command == "PRIVMSG") {
        // Esta parte de aquí abajo solo es para mostrar la salida en el server, no tiene otra funcionalidad
        std::string message = "";
        for (size_t i = 2; i < tokens.size(); ++i) {
            message += tokens[i];
            if (i != tokens.size() - 1)
                message += " ";
        }
        message.erase(message.find_last_not_of("\r") + 1); // Remove end of line characters
        std::cout << "[" << client_fd << "] Send PRIVMSG to " << tokens[1] << " " << message << std::endl;
        // Response and code here
        Command::handle_privmsg(client_fd, parameters, this->users, this->channels);
    }
    else
    {
        //response = ":" + server_name + " 421 " + nick + " " + command + " :Command not found ";
        std::cout << "[" << client_fd << "] Command not found " << std::endl;
        return ;
    }
    Utils::sendIRCMessage(client_fd, response);
}

void Server::read_data_from_socket(int i, bool& running) {
    char sender[32];
    
    char buffer[BUFSIZ];
    memset(buffer, 0, BUFSIZ);
    int sender_fd = poll_fds[i].fd;
    int bytes_read = recv(sender_fd, buffer, BUFSIZ - 1, 0);
    if (bytes_read <= 0) {
        if (bytes_read == 0) {
            std::cout << "[" << sender_fd << "] Client socket closed connection." << std::endl;
        } else {
            std::cerr << "[Server] Recv error: " << strerror(errno) << std::endl;
        }
        close(sender_fd);
        del_from_poll_fds(i);
    } else {
        std::stringstream received_msg(buffer); // Convert the buffer to std::stringstream
        std::string line;
        while (std::getline(received_msg, line, '\n')) {
            std::stringstream ss_line(line); // Convert line to std::stringstream
            std::string token;
            std::vector<std::string> tokens;
            while (std::getline(ss_line, token, ' ')) {
                if (token[token.length() - 1] == '\r')
                    token.erase(token.length() - 1);
                tokens.push_back(token); // Create a vector with words
            }
            if (tokens.size() > 0)
                handle_client_command(sender_fd, tokens, running); // Function for command handler            
        }
        //std::cout << "[" << sender_fd << "] Got message: " << buffer << std::endl;
        sprintf(sender, "%d", sender_fd);
        std::string msg_to_send = "[" + std::string(sender) + "] says: " + buffer;
        for (size_t j = 1; j < poll_fds.size(); j++) {
            int dest_fd = poll_fds[j].fd;
            if (dest_fd != sender_fd) {
                int status = send(dest_fd, msg_to_send.c_str(), msg_to_send.length(), 0);
                if (status == -1) {
                    std::cerr << "[Server] Send error to client fd " << dest_fd << ": " << strerror(errno) << std::endl;
                }
            }
        }
    }
}

void Server::add_to_poll_fds(int new_fd) {
    pollfd new_pollfd;
    new_pollfd.fd = new_fd;
    new_pollfd.events = POLLIN;
    poll_fds.push_back(new_pollfd);
}

void Server::del_from_poll_fds(int i) {
    poll_fds.erase(poll_fds.begin() + i);
}

// Handler to terminal signals
void clean_exit(int signum) {
    std::cout << "\n[Servidor] Señal (" << signum << ") recibida. Cerrando el servidor...\n";
}

void Server::run(int port, std::string pass) {
    // config signal handle
    struct sigaction sa;
    sa.sa_handler = clean_exit;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
   
    bool running = true; // Variable to control
    std::cout << "---- SERVER ----\n\n";
    server_socket = create_server_socket(port);
    if (server_socket == -1) {
        return;
    }

    std::cout << "[Server] Setting Password to " << pass << std::endl;
    server_password = pass;
    std::cout << "[Server] Listening on port " << port << std::endl;
    if (listen(server_socket, SOMAXCONN) != 0) {
        std::cerr << "[Server] Listen error: " << strerror(errno) << std::endl;
        close(server_socket);
        return;
    }

    add_to_poll_fds(server_socket);
    std::cout << "[Server] Set up poll fd array\n"; 



    Channel canal1("#primero");
    Channel canal2("#segundo");
    Channel canal3("#ultimo");
    Client cliente1("Pedro");
    cliente1.set_nickname("NPedro");
    Client cliente2("Silvia");
    cliente2.set_nickname("NSilvia");
    Client cliente3("Joaquin");
    cliente3.set_nickname("NJoaquin");
    Client cliente11("Paca");
    Client cliente22("ROdriga");
    Client cliente33("Joaquina");
    canal1.haspswd = false;
    // canal1.set_maxClients(3);
    canal1.maxClientsEnabled = false;
    canal1.topic = "Primer topic";
    canal1.invitationsEnabled = false;
    canal1.topicdate = std::to_string(time(nullptr));
    canal2.haspswd = false;
    canal3.haspswd = true;
    canal3.key = "key";
    canal2.maxClientsEnabled = false;
    canal2.invitationsEnabled = false;
    canal1.addClient(cliente1);
    canal1.addClient(cliente2);
    canal1.addClient(cliente3);
    canal2.addClient(cliente11);
    canal2.addClient(cliente22);
    canal2.addClient(cliente33);
    this->channels.push_back(canal1);
    this->channels.push_back(canal2);
    this->channels.push_back(canal3);

while (running) {
        int status = poll(&poll_fds[0], poll_fds.size(), 2000);
        if (status == -1) {
            std::cerr << "[Server] Poll error: " << strerror(errno) << std::endl;
            break;
        } else if (status == 0) {
            std::cout << "[Server] Waiting...\n";
            continue;
        }

        for (size_t i = 0; i < poll_fds.size(); i++) {
            if ((poll_fds[i].revents & POLLIN) != 1) {
                continue;
            }
            std::cout << "[" << poll_fds[i].fd << "] Ready for I/O operation\n";
            if (poll_fds[i].fd == server_socket) {
                accept_new_connection();
            } else {
                read_data_from_socket(i, running);
            }
        }
    }

    // Clean up connections
    for (std::vector<pollfd>::iterator it = poll_fds.begin(); it != poll_fds.end(); ++it) {
        std::cout << "[Server] Closing connections.\n";
        close(it->fd);
    }
    close(server_socket);
    std::cout << "[Server] Closing server.\n";
}