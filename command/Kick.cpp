/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 04:43:17 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/11 01:37:29 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"
bool Cmds::check_kick(std::string Parametres)
{
    if(Parameters.empty() || remove_(Parametres)[0] == ':')
    {
        this->replay->ERR_NEEDMOREPARAMS();
        return false;
    }
    return true;
}
void eraseClientFromChannel(std::map<std::string, Channels>::iterator &it, const std::string &nickName)
{
    std::vector<Client> &members = it->second.members;
    //std::vector<Client>::iterator it_to_remove = members.end();

    std::vector<Client> &operators = it->second.operators;
    //std::vector<Client>::iterator operatorIt = operators.end();

    for (std::vector<Client>::iterator it = members.begin(); it != members.end(); ++it)
    {
        if (it->get_nickname() == nickName)
        {
            std::cout << GREEN << nickName << " successfully kicked from channel" << RESET << std::endl;
            members.erase(it);
            break;
        }
    }
    for (std::vector<Client>::iterator it = operators.begin(); it != operators.end(); ++it)
    {
        if (it->get_nickname() == nickName)
        {
            operators.erase(it);
            std::cout << GREEN << nickName << " Operator successfully kicked from channel" << RESET << std::endl;
            break;
        }
    }
}

void Cmds::kick(std::string line, std::map<std::string, Channels> &channel, Client &c)
{
    std::string channelName;
    std::string nickName;
    std::vector<std::string> list;
    std::vector<std::string> split = split_string(line, ' ');
    channelName = split[0];
    nickName = split[1];
    if (nickName.find(',') != std::string::npos)
        list = split_string(nickName, ',');
    else
        list.push_back(nickName);
    if (channelName.empty() || nickName.empty())
    {
        this->replay->ERR_NEEDMOREPARAMS();
        return;
    }
    std::map<std::string, Channels>::iterator it = channel.find(channelName);

    if (hasChannel(channelName, channel))
    {
        if (isModerator(c, it->second.operators))
        {
            for (size_t i = 0; i < list.size(); i++)
            {
                if (isMember(list[i], it->second.members))
                {
                    std::vector<Client>::iterator iterator_member = it->second.members.begin();

                    for (; iterator_member != it->second.members.end(); iterator_member++)
                    {
                        std::string protocol = ":" + c.get_nickname() + "!" + c.get_username() + "@" + this->replay->getMachineHostName() + " KICK " + channelName + " " + list[i] + "\r\n";
                        send(iterator_member->get_fd(), protocol.c_str(), protocol.size(), 0);
                    }
                    std::cout << "Before:" << it->second.members.size() << std::endl;
                    eraseClientFromChannel(it, list[i]);
                   
                    std::cout << "after:" << it->second.members.size() << std::endl;
                }
                else
                {
                    //: irc.example.com 441 dan jerry #v3 :They aren't on that channel
                    std::string protocol = ":" + this->replay->getMachineHostName() + " 441 " + c.get_nickname() + " :They aren't on that channel\r\n";
                    send(c.get_fd(), protocol.c_str(), protocol.size(), 0);
                    return;
                }
            }
        }
        else
        {
            this->replay->ERR_CHANOPRIVSNEEDED(channelName);
            std::cout << RED << "Can't kick users from server without permissions" << RESET << std::endl;
        }
    }
    else
    {
        this->replay->ERR_NOSUCHCHANNEL();
    }
}
