#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include<string>

class CommandsHandling{
public:
    CommandsHandling();

    //taking command from client and return RESP response
    std::string ProcessCommand(std::string Commandline);

};

#endif