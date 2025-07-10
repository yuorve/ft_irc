/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoropeza <yoropeza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 18:28:58 by yoropeza          #+#    #+#             */
/*   Updated: 2024/11/23 12:18:15 by yoropeza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstring>
#include <cerrno>
#include <netdb.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <sstream>
#include <fcntl.h>

#include "parse.hpp"
#include "channel.hpp"
#include "command.hpp"

#define PORT 8484  // our server's port

class Client;
class Channel;

class Server {
private:
    int server_socket;
    std::string server_password;
    std::vector<pollfd> poll_fds; // Array of socket file descriptors
    std::vector<Channel> channels;  //añadido
    std::vector<Client> users;  //añadido
    
	Server(const Server &other);		
	Server &operator=(const Server& other);	
    int create_server_socket(int port);
    void accept_new_connection();
    void read_data_from_socket(int i, bool& running);
    void add_to_poll_fds(int new_fd);
    void del_from_poll_fds(int i);
    // void handle_client_command(int client_fd, const std::vector<std::string>& command);  //command ahora es un vector
    void handle_client_command(int client_fd, const std::vector<std::string>& tokens, bool& running);
    
public:
    Server(void);
    ~Server(void);
    static int	checkPort(const std::string port);
    static int	checkPass(const std::string pass);
    static int	ft_stoi(const char *str);
    void run(int port, std::string pass);
};

#endif