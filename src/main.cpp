/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoropeza <yoropeza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 09:24:06 by yoropeza          #+#    #+#             */
/*   Updated: 2024/11/23 12:13:07 by yoropeza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"

int main(int argc, char **argv) {
    
    if (argc == 3)
    {
        Server server; 
        // validate value of argv1 and argv2
        if (Server::checkPort(argv[1]) && Server::checkPass(argv[2]))
            server.run(Server::ft_stoi(argv[1]), argv[2]);
        else
        {
            std::cerr << "[error]: port or password is not valid" << std::endl;
            return (1);
        }        
    }
    else
	{
		std::cerr << "[usage]: ./irc_server [port] [password]" << std::endl;
		return (1);
	}
	return (0);
}
