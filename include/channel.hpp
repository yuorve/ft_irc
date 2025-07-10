#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <algorithm>
#include "client.hpp"

class Channel {
public:
    Channel();
    Channel(const std::string& name);
    ~Channel();
    void addGuest(Client& client);
    void removeGuest(Client& client);
    void addClient(Client& client);
    void removeClient(Client& client);
    void removeOper(Client& client);
    void set_maxClients(size_t maxClients);
    void setName(const std::string& topic);
    std::string getName() const;
    void setTopic(const std::string& topic);
    std::string getTopic() const;   
    void setMode(const std::string& mode);
    std::string getMode() const;
    void broadcast(const std::string& message, int sender);
    bool isFull() const;
    bool exist() const;
    size_t getMaxClients() const;
    std::string name;
    std::string topic;
    std::string topicdate;
    std::string key;    
    std::vector<Client> clients;
    std::vector<Client> oper;
    std::vector<Client> guests; // Lista de invitados al canal
    bool operator==(const Channel& src) const;
    bool operator!=(const Channel& src) const;
   // Channel(const Channel& other); //movido
    Channel& operator=(const Channel& other);
    bool haspswd;
    bool maxClientsEnabled;
    bool topicEnabled;
    bool invitationsEnabled;

private:
    std::string mode;
    size_t maxClients;
};

#endif
