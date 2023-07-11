/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Prvmsg.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 02:44:35 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/10 01:29:08 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

bool Cmds::check_prvmsg(std::string Parametres)
{
    (void)Parametres;
    return true;
}

void Cmds::prvmsg(std::string Parametres)
{
    (void)Parametres;
}

bool isChannel(std::string str)
{
    if (str.find("#") != std::string::npos)
        return true;
    return false;
}

bool hasClient(std::string clientName, std::map<int, Client> &clients)
{
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->second.get_nickname() == clientName)
            return true;
    }
    return false;
}

std::string appendMessage(std::vector<std::string> &line)
{
    std::string message;

    for (size_t i = 1; i < line.size(); i++)
    {
        message += line[i] + " ";
    }
    return message;
}

bool Cmds::checkChannel(std::vector<std::string> str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (isChannel(str[i]))
            return true;
    }
    return false;
}
std::string remove_two(std::string command)
{
    std::string newcommand;
    for(int i = 0; i < (int)command.size(); i++)
    {
        if(command[i] != ':')
            newcommand += command[i];
    }
    std::cout << "this is the new : " << newcommand << std::endl;
    return newcommand;
}

void Cmds::privateMessage(std::string command, std::map<std::string, Channels> &channels, std::map<int, Client> clients, Client &client)
{
    std::cout << "command : " << command << std::endl;
    command = remove_two(command);
    std::vector<std::string> line = split_string(command, ' ');
    std::string target = line[0];
    std::string message;
    std::vector<std::string> _channels;
    std::vector<std::string> _clients;
    message = appendMessage(line);
    std::cout << RED << message << std::endl;
    if (command.empty() || target.empty() || message.empty())
        this->replay->ERR_NEEDMOREPARAMS();
    if (target.find(',') != std::string::npos)
    {
        _channels = split_string(target, ',');
        if (!checkChannel(_channels))
            _clients = split_string(target, ',');
    }
    else
        _channels.push_back(target);
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (isChannel(_channels[i])) // check if channel name contain #
        {
            std::map<std::string, Channels>::iterator it = channels.find(_channels[i]);
            if (hasChannel(_channels[i], channels)) /// check if channel existe in the map
            {
                std::vector<Client>::iterator clientsIterator = it->second.members.begin();
                for (; clientsIterator != it->second.members.end(); clientsIterator++) // broadcast
                {
                    if (clientsIterator->get_fd() != client.get_fd())
                    {
                        std::string protocol = ":" + client.get_nickname() + "!" + client.get_username() + "@" + this->replay->getMachineHostName() + " PRIVMSG " + _channels[i] + " :" + message + "\r\n";
                        send(clientsIterator->get_fd(), protocol.c_str(), protocol.size(), 0);
                    }
                }
            }
            else
            {
                this->replay->ERR_CANNOTSENDTOCHAN();
            }
        }
    }
    for (size_t i = 0; i < _channels.size(); i++)
    {
        for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
        {
            if (it->second.get_nickname() == _channels[i])
            {
                std::cout << _channels[i] << std::endl;
                std::cout << "messge : " << message << std::endl;
                std::string protocol = ":" + client.get_nickname() + "!~" + client.get_username() + "@" + this->replay->getMachineHostName() + " PRIVMSG " + _channels[i] + " :" + message + "\r\n";
                send(it->second.get_fd(), protocol.c_str(), protocol.size(), 0);
            }
        }
    }
}