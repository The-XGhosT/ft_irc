/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 01:46:22 by iqabbal           #+#    #+#             */
/*   Updated: 2023/06/26 09:14:39 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

std::string Cmds::remove_(std::string line)
{
    std::string ws = " \n\r\t\f\v";
    size_t start = line.find_first_not_of(ws);
    size_t end = line.find_last_not_of(ws) + 1;
    size_t len = end - start;
    if(end != 0)
        line = line.substr(start,len);
    return line;
}

bool Cmds::check_mode(char m,std::map<char,bool>modes)
{
    std::map<char,bool>::iterator it = modes.find(m);
    if(it->second == true)
        return true;
    return false;

}

bool Cmds::hasChannel(std::string channelName, std::map<std::string, Channels> &channels)
{
    std::map<std::string, Channels>::iterator it = channels.begin();
    for (; it != channels.end(); ++it)
    {
        if (it->first == channelName)
            return true;
    }
    return false;
}

bool Cmds::isModerator(Client &c, std::vector<Client> &operators_)
{
    std::vector<Client>::iterator it = operators_.begin();
    for (; it != operators_.end(); it++)
    {
        if (c.get_nickname() == it->get_nickname())
            return true;
    }
    return false;
}

bool Cmds::isMember(Client &c, std::vector<Client> &members)
{
    std::vector<Client>::iterator it = members.begin();
    for (; it != members.end(); it++)
    {
        if (c.get_nickname() == it->get_nickname())
            return true;
    }
    return false;
}

bool Cmds::isMember(std::string nickName, std::vector<Client> &members)
{
    std::vector<Client>::iterator it = members.begin();
    for (; it != members.end(); it++)
    {
        if (nickName == it->get_nickname())
            return true;
    }
    return false;
}


std::vector<std::string> Cmds::split_string(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::istringstream string_stream(str);
    std::string token;

    while (std::getline(string_stream, token, delimiter))
    {
        if (!token.empty())
            tokens.push_back(token);
    }
    return (tokens);
}

std::string Cmds::timeTopic()
{
    std::time_t now = std::time(NULL);
	std::stringstream ss;

	ss << now;
	return ss.str();
}

bool Cmds::isClient(std::string nickname, std::map<int,Client> &clients)
{
    std::map<int,Client>::iterator it = clients.begin();
    for (; it !=clients.end(); it++)
    {
        if (nickname  == it->second.get_nickname())
            return true;
    }
    return false;
}




/*
JOIN <channel>{,<channel>} [<key>{,<key>}]
KICK <channel> <nick>
TOPIC <channel> [<topic>]




*/