/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaji-el <anaji-el@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 14:14:12 by anaji-el          #+#    #+#             */
/*   Updated: 2023/07/09 20:57:34 by anaji-el         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

bool    Cmds::isInvited(std::string nickname, std::vector<Client> _Invited){
    for (std::vector<Client>::iterator it = _Invited.begin(); it != _Invited.end(); ++it) {
        if (it->get_nickname() == nickname)
            return true;
    }
    return false;
}
// Check if the channel is invite-only and if the inviter is a channel operator
// Define a function to handle the INVITE command
void Cmds::Invite(std::string Parameters, std::map<std::string, Channels> &channels, Client &c, std::map<int,Client> serverClients) {
    std::vector<std::string> Param = split_string(Parameters,' ');
    std::string nickname = Param[0];
    std::string channelname = Param[1];
    
    if (Param.size() == 2)
    {
        if (channels.find(channelname) == channels.end())
        {
            this->replay->ERR_NOSUCHCHANNEL();
            return;
        }
        std::map<std::string, Channels>::iterator channel_iterator= channels.find(channelname);
        if (!isClient(nickname,serverClients))
        {
            this->replay->ERR_NOSUCHNICK();
            return;
        }
        if (!isMember(c, channel_iterator->second.members)){
            this->replay->ERR_NOTONCHANNEL(channelname);
            return;
        }
        if (isInvited(nickname,channel_iterator->second.members)){
            this->replay->ERR_USERONCHANNEL();
            return;
        }
        if (!isModerator(c,channel_iterator->second.operators)){
            this->replay->ERR_CHANOPRIVSNEEDED(channelname);
            return;
        }
        if (!isInvited(nickname,channel_iterator->second.members) && isModerator(c,channel_iterator->second.operators))
        {
            for (std::map<int,Client>::iterator it = serverClients.begin(); it != serverClients.end(); it++){
                if (nickname == it->second.get_nickname())
                    channel_iterator->second.invited_only.push_back(it->second.get_nickname());
            }
        }
    }
    else{
            this->replay->ERR_NEEDMOREPARAMS();
            return;
    }
}