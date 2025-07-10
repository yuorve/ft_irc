#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>

class Client{
    private:
        int fd;
        std::string username;
        std::string nickname;
        bool registered;

    public:
        Client();
        Client(std::string name);       //añadido para pruebas
        // Client(const Client &src);  //añadido
        ~Client();
        int get_fd();
        std::string get_username();
        std::string get_nickname();
        std::string get_prefix();
        void set_fd(int fd);
        void set_username(std::string username);
        void set_nickname(std::string nickname);
        void set_registered(bool registered);
        bool is_registered();
        bool operator==(const Client& other) const {
        return fd == other.fd;
    }
       
};

std::vector<Client *>::iterator find_client_nickname(std::vector<Client *> &clients, const std::string &nickname);
std::vector<Client *>::iterator find_client_fd(std::vector<Client *> &clients, int fd);  //fallo de escritura
#endif