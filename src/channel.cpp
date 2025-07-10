#include "../include/channel.hpp"
#include "../include/utils.hpp"

Channel::Channel() : name("default") {}     //no estaba, agregado
Channel::Channel(const std::string& name) : name(name) {}   //en el hpp no estaba

void Channel::addGuest(Client& client) {
    guests.push_back(client);
}

void Channel::addClient(Client& client) {
    clients.push_back(client);
}

void Channel::removeClient(Client& client) {
	auto it = std::find(clients.begin(), clients.end(), client);
    if (it != clients.end())
        clients.erase(it);
    // for (auto it = clients.begin(); it != clients.end(); ++it) {
    //     if (*it == client) {
    //         clients.erase(it);
    //         break;
    //     }
    // }
}

void Channel::removeOper(Client& target) {
	auto it = std::find(oper.begin(), oper.end(), target);
    if (it != oper.end())
        oper.erase(it);
    // for (auto it = clients.begin(); it != clients.end(); ++it) {
    //     if (*it == client) {
    //         clients.erase(it);
    //         break;
    //     }
    // }
}

void Channel::removeGuest(Client& target) {
	auto it = std::find(guests.begin(), guests.end(), target);
    if (it != guests.end())
        guests.erase(it);
}

void Channel::broadcast(const std::string& message, int sender) {
    for (auto client : clients) {
        if (client.get_fd() != sender)
            // send message to client
            Utils::sendIRCMessage(client.get_fd(), message);
    }
}

void Channel::setName(const std::string& name) {
    this->name = name;
}

std::string Channel::getName() const {
    return name;
}

void Channel::setTopic(const std::string& topic) {
    this->topic = topic;
}

std::string Channel::getTopic() const {
    return topic;
}

void Channel::setMode(const std::string& mode) {
    this->mode = mode;
}

std::string Channel::getMode() const {
    return mode;
}

void Channel::set_maxClients(size_t maxClients) {
    this->maxClients = maxClients;
}

bool Channel::isFull() const {
    return clients.size() >= maxClients;
}

bool Channel::exist() const {
    return clients.size() >= maxClients;
}

size_t Channel::getMaxClients() const {
    return maxClients;
}

Channel::~Channel() {
    for (auto client : clients) {
        // send message to client
    }
}


bool Channel::operator==(const Channel& src) const {    //añadido
        return (name == src.name);}

bool Channel::operator!=(const Channel& src) const {    //añadido
        return (name != src.name);}

Channel& Channel::operator=(const Channel& other)       //añadido
{
    name = other.name;
    topic= other.topic;
    topicdate = other.topicdate;
    // for (int i = 0; i < other.clients.size(); i++)
    // {
    //     clients[i].set_fd(other.clients[i]->get_fd());
    //     clients[i].set_username(other.clients[i]->get_username());
    //     clients[i].set_nickname(other.clients[i]->get_nickname());
    //     clients[i].set_registered(other.clients[i]->is_registered());
    // }
    for (size_t i = 0; i < other.clients.size(); ++i) {
    clients.push_back(other.clients[i]); // Copiar cada cliente directamente
    }
    haspswd = other.haspswd;
    return *this;
}
