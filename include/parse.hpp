#ifndef PARSE_HPP
#define PARSE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "server.hpp"
#include "channel.hpp"
#include "client.hpp"
#include "replies.hpp"

class Parse {
    public:
        Parse() {};
        ~Parse() {};
        static std::vector<std::string> split(const std::string& str);


        
    private:

};  

#endif