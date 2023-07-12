/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmds.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 02:03:45 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/12 06:47:35 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmds.hpp"
#include <cctype>
#include <sstream>
#include <vector>
#include <algorithm>

Cmds::Cmds()
{
    this->replay = new Replay();
}
Cmds::~Cmds()
{
}

void Cmds::set_replay(Replay &replay)
{
    this->replay = &replay;
}
Replay &Cmds::get_replay()
{
    return *(this->replay);
}

void Cmds::set_command(std::string Command)
{
    if (!Command.empty())
        this->Command = Command;
}
void Cmds::set_parametres(std::string Parameters)
{
    if (!Command.empty())
        this->Parameters = Parameters;
}

std::string Cmds::get_command()
{
    return this->Command;
}
std::string Cmds::get_parametre()
{
    return this->Parameters;
}

std::vector<std::string> Cmds::init_my_command()
{
    std::vector<std::string> all_commands;
    all_commands.push_back("PASS");
    all_commands.push_back("NICK");
    all_commands.push_back("USER");
    all_commands.push_back("JOIN");
    all_commands.push_back("MODE");
    all_commands.push_back("TOPIC");
    all_commands.push_back("KICK");
    all_commands.push_back("PRIVMSG");
    all_commands.push_back("INFO");
    all_commands.push_back("INVITE");
    all_commands.push_back("PART");
    all_commands.push_back("PONG");
    all_commands.push_back("BOT");
    return all_commands;
}

bool bot_checker(Client &c,std::string hostname)
{
    if(c.get_bot_checker() == true)
    {
        std::string message = ":" + hostname + " 001 " +c.get_nickname() + " : please exit from bot \"Bot exit\" or finish  the game with bot 😊\r\n" ;
        send(c.get_fd(),message.c_str(),message.size(),0);
        return false;
    }
    return true;
}

bool user_register(Client &c,std::string hostname)
{
    if(c.IsRegistred == false)
    {
        std::string message = ":" + hostname + " 001 " +c.get_nickname() + " : you are not register yet 😎\r\n" ;
        send(c.get_fd(),message.c_str(),message.size(),0);
        return false;
    }
    return true;
}


bool Cmds::check_command(std::string command, std::string Parameters, Client &c, std::map<int, Client> &clients, std::string pass_server, Server &s)
{
    std::vector<std::string>::iterator it;
    std::vector<std::string> all_commands;
    int command_to_execute;
    std::cout << "in check command " << Parameters << std::endl;
    this->replay->set_client(c);
    command_to_execute = -1;
    all_commands = init_my_command();
    for (int i = 0; i < (int)all_commands.size(); i++)
    {
        if (all_commands[i] == command)
        {
            command_to_execute = i;
            break;
        }
    }
    if(command_to_execute == -1 && c.get_bot_checker() == false)
    {
        this->replay->COMMAND_NOT_FOUND(command);
        return false;
    }
    switch (command_to_execute)
    {
        case PASS:
                if (!check_pass(Parameters))
                    return false;
                pass(Parameters, c, pass_server);
                break;
        case NICK:
                if (!check_nick(Parameters, clients, c) || c.count == 0 || c.IsRegistred == true)
                    return false;
                nick(Parameters, c);
                break;
        case USER:
                if (!check_user(Parameters, c) || c.count == 0 || c.IsRegistred == true)
                    return false;
                user(Parameters, c);
                break;
        case JOIN:
                if(user_register(c,this->replay->getMachineHostName()) && bot_checker(c,this->replay->getMachineHostName()) && check_join(Parameters))
                    join(Parameters, s.channels,c);
                break;
        case MODE:
                if(!user_register(c,this->replay->getMachineHostName()) || !bot_checker(c,this->replay->getMachineHostName()) || !check_mode(Parameters,s.channels,c))
                    return false;
                mode(Parameters, s.channels, c);
                break;
        case TOPIC:
                if (!user_register(c,this->replay->getMachineHostName()) || !bot_checker(c,this->replay->getMachineHostName()) || !check_topic(Parameters, c, s.channels))
                    return false;
                topic(Parameters, s.channels, c);
                break;
        case KICK:
                if(user_register(c,this->replay->getMachineHostName()) && bot_checker(c,this->replay->getMachineHostName()) && check_kick(Parameters))
                    kick(Parameters, s.channels, c);
                break;
        case PRIVMSG:
                if(user_register(c,this->replay->getMachineHostName()) && bot_checker(c,this->replay->getMachineHostName()))
                    privateMessage(Parameters, s.channels, s.clients, c);
                break;
        case INFO:
                if (!user_register(c,this->replay->getMachineHostName()) || !bot_checker(c,this->replay->getMachineHostName()) || !check_info(Parameters))
                    return false;
                infoc(remove_(Parameters), s.channels,c);
                break;
        case INVITE:
                if(user_register(c,this->replay->getMachineHostName()) && bot_checker(c,this->replay->getMachineHostName()))
                    Invite(Parameters, s.channels, c, s.clients);
        case PART:
                if(user_register(c,this->replay->getMachineHostName()) && bot_checker(c,this->replay->getMachineHostName()))
                    part(Parameters, s.channels, c);
                break;
        case PONG:
                break;
        case BOT :
                if(user_register(c,this->replay->getMachineHostName()))
                    bot(remove_(Parameters),c);
                break;
    }
    return true;
}



bool not_valid(std::string &Parameters)
{
    std::cout << "this is : " << Parameters; 
    if(Parameters.size() > 1)
        return false;
    if(!isdigit(Parameters[0]))
        return false;
    return true;
}


std::string getMachineHostName()
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        // Error occurred while retrieving the hostname
        return "";
    }
    return hostname;
};

void parseJoinCommand(std::vector<std::string> &channelNames, Client c)
{
    std::vector<std::string>::iterator it;

    // if (channelNames.size() <= 0)
    // {
    //     std::cerr << "Usage: JOIN <channelName> <password>" << std::endl;
    //     send_message_to_server(c, "Usage: JOIN <channelName> <password>");
    // }
    for (std::vector<std::string>::iterator it = channelNames.begin(); it != channelNames.end(); it++)
    {
        std::string channelName = *it;
        if (channelName.empty() || channelName[0] != '#' || channelName.find(' ') != std::string::npos || it->size() <= 1)
        {
            std::cout << "Invalid channel name: " << channelName << std::endl;
            return;
            break;
        }
    }
    (void)c;
}

std::string printOperatorMemebers(std::map<std::string, Channels> channel)
{
    std::string operaterMembers = ":";
    std::map<std::string, Channels>::iterator it;

    for (std::map<std::string, Channels>::iterator it = channel.begin(); it != channel.end(); it++)
    {
        for (size_t i = 0; i < it->second.operators.size(); i++)
        {
            operaterMembers = "@" + it->second.members[i].get_nickname() + " ";
        }
    }
    return operaterMembers;
}