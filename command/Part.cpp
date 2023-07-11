/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaji-el <anaji-el@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 14:14:29 by anaji-el          #+#    #+#             */
/*   Updated: 2023/07/09 01:03:21 by anaji-el         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

void Cmds::part(std::string Parameters, std::map<std::string, Channels> &channels, Client &c) {
    std::string channelname = Parameters;
    if (channelname.empty())
    {
        this->replay->ERR_NEEDMOREPARAMS();
        return;
    }
    std::map<std::string, Channels>::iterator it = channels.find(channelname);
    
    if (!hasChannel(channelname, channels)) {
        this->replay->ERR_NOSUCHCHANNEL();
        return;
    }
    else {
        if (!isMember(c, it->second.members)) {
            this->replay->ERR_NOTONCHANNEL(channelname);
            return;
        }
        else {
            std::vector<Client>::iterator it_members =it->second.members.begin();
            std::vector<Client>::iterator it_operators =it->second.operators.begin();
            for (it_members =it->second.members.begin(); it_members != it->second.members.end(); it_members++)
            {
                std::string protocol = ":" + c.get_nickname() + "!" + c.get_username() + "@" + this->replay->getMachineHostName() + " PART " + channelname + " "  + "\r\n";
                send(it_members->get_fd(), protocol.c_str(), protocol.size(), 0);
            }
            for (it_members =it->second.members.begin(); it_members != it->second.members.end(); ++it_members) {
                if (it_members->get_nickname() == c.get_nickname()) {
                   it->second.members.erase(it_members);
                    std::cout << GREEN << c.get_nickname() << " successfully you're out of channel." << RESET << std::endl;
                    break;
                }
            }
            for (it_operators =it->second.operators.begin(); it_operators != it->second.operators.end(); ++it_operators) {
                if (it_operators->get_nickname() == c.get_nickname()) {
                   it->second.operators.erase(it_operators);
                    break;
                }
            }
        }
    }
}