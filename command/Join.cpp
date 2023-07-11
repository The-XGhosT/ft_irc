/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 01:36:39 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/11 00:36:07 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"
#include <cctype>
#include <sstream>
#include <vector>
#include <algorithm>


bool Cmds::check_join(std::string Parameters)
{
    if(Parameters.empty())
    {
        this->replay->ERR_NEEDMOREPARAMS();
        return false;
    }
    if(Parameters.size() == 1 && Parameters[0] == '#')
    {
        this->replay->ERR_NEEDMOREPARAMS();
        return false;
    }
    std::vector<std::string> command = split_string(Parameters,' ');
    if(command.size() > 2)
    {
        return false;
    }
    return true;
}

void Cmds::extractJoinParams(std::string cmd, std::vector<std::string> &password, std::vector<std::string> &command)
{
    std::vector<std::string> all_in_vector;
    int count_space = std::count(cmd.begin(), cmd.end(), ' ');
    if (count_space > 0)
    {
        all_in_vector = split_string(cmd, ' ');
        int count_vergule = std::count(all_in_vector[0].begin(), all_in_vector[0].end(), ',');
        if (count_vergule > 0)
            command = split_string(all_in_vector[0], ',');
        else
            command.push_back(all_in_vector[0]);
        count_vergule = std::count(all_in_vector[1].begin(), all_in_vector[1].end(), ',');
        if (count_vergule > 0)
            password = split_string(all_in_vector[1], ',');
        else
            password.push_back(all_in_vector[1]);
    }
    else
    {
        int count_vergule = std::count(cmd.begin(), cmd.end(), ',');
        if (count_vergule > 0)
            command = split_string(cmd, ',');
        else
            command.push_back(cmd);
    }
}

void init_mode(std::map<char, bool> &modes)
{
    modes.insert(std::make_pair('i', false));
    modes.insert(std::make_pair('t', true));
    modes.insert(std::make_pair('k', false));
    modes.insert(std::make_pair('l', false));
    modes.insert(std::make_pair('o', false));
}

void Cmds::parseJoinCommand(std::vector<std::string> &channelNames, Client c)
{
    std::vector<std::string>::iterator it;

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

bool Cmds::inviteOnlyChannel(std::map<std::string, Channels> &channels, std::string channelName, Client c)
{
    std::map<std::string, Channels>::iterator it;

    for (size_t i = 0; i < channels[channelName].members.size(); i++)
    {
        if (channels[channelName].members[i].get_nickname() == c.get_nickname())
            return true;
    }
    std::string syntax = ":" + this->replay->getMachineHostName() + " 400 " + c.get_nickname() + " :Sorry, you cannot join this channel because it is invite-only.\
    Please ask someone who has access to the channel to invite you.\r\n";

    // std::string protocol = "Sorry, you cannot join this channel because it is invite-only. Please ask someone who has access to the channel to invite you.";
    send(c.get_fd(), syntax.c_str(), syntax.size(), 0);
    return false;
}

void Cmds::reachLimit(std::map<std::string, Channels>::iterator channelsIt, Client &client, std::string channelName)
{
    std::cout << "size is : " << channelsIt->second.members.size() << std::endl;
    if (channelsIt->second.limit <= (int)channelsIt->second.members.size())
    {
        std::cerr << RED << "This channel has reached its maximum capacity and cannot accommodate any more users at this time." << std::endl;
        std::string protocol = ":" + this->replay->getMachineHostName() + " 471 " + client.get_nickname() + channelName + " :Cannot join channel (+l)";
        send(client.get_fd(), protocol.c_str(), protocol.size(), 0);
    }
}

bool check_invite_only(std::string name, std::vector<std::string> &members_invite)
{
    for (int i = 0; i < (int)members_invite.size(); i++)
    {
        std::cout << "member is :" << members_invite[i] << std::endl;
        if (members_invite[i] == name)
            return true;
    }
    return false;
}

void Cmds::join(std::string Parametres, std::map<std::string, Channels> &channels,Client &client)
{
    std::vector<std::string> channels_name, channels_password;
    extractJoinParams(Parametres, channels_password, channels_name);
    
    for (std::vector<std::string>::iterator it = channels_name.begin(); it != channels_name.end(); it++)
    {
        std::string channel_name = *it;
        if (channel_name.empty() || channel_name[0] != '#' || channel_name.find(' ') != std::string::npos || it->size() <= 1)
        {
            std::cout << "Invalid channel name: " << channel_name << std::endl;
            return;
        }
    }
    std::cout << channels_name[0] << std::endl;
    Channels c;
    for (std::vector<std::string>::iterator it = channels_name.begin(); it != channels_name.end(); it++)
    {
        int index = std::distance(channels_name.begin(), it);
        std::map<std::string, Channels>::iterator channel_iterator = channels.find(channels_name[index]);
        if (hasChannel(channels_name[index], channels))
        {

            if (!isMember(client, channel_iterator->second.members))
            {
                if (check_mode('i', channel_iterator->second.modes))
                {
                    if (!check_invite_only(client.get_nickname(), channel_iterator->second.invited_only))
                    {
                        invite_only(client, channels_name[index]);
                        return;
                    }
                    else
                    {
                        channel_iterator->second.members.push_back(client);
                        already_channel(client, channels, channels_name[index]);
                        message_inform_clients(client, channels, channels_name[index]);
                        return ;
                    }
                }
                if (check_mode('k', channel_iterator->second.modes))
                {
                    if (channels_password.size() > 0)
                    {
                        if (channels_password[index].empty() || channel_iterator->second._password != channels_password[index])
                        {
                            bad_key(client, channels_name[index]);
                            return;
                        }
                        if (channel_iterator->second._password == channels_password[index])
                        {
                            channel_iterator->second.members.push_back(client);
                            joining_a_channel_has_akey(client, channels, channels_name[index]);
                            message_inform_clients(client, channels, channels_name[index]);
                            return;
                        }
                    }
                    else
                    {
                        bad_key(client, channels_name[index]);
                        return;
                    }
                }
                if (check_mode('l', channel_iterator->second.modes))
                {
                    reachLimit(channel_iterator, client, channels_name[index]);
                    return;
                }
                channel_iterator->second.members.push_back(client);
                already_channel(client, channels, channels_name[index]);
                message_inform_clients(client, channels, channels_name[index]);
            }
            else
            {
                err_user_on_channel(client, channels_name[index]);
            }
        }
        else
        {
            c._channelName = channels_name[index];
            init_mode(c.modes);
            if (!channels_password.empty() && channels_password[index].length() > 0)
            {
                c._password = channels_password[index];
                c.modes['k'] = true;
            }
            c.members.push_back(client);
            c.operators.push_back(client);
            c._topic = "siyasa";
            c.timestamp_str = timeTopic();
            c.whoSetTopic = client.get_nickname();
            channels.insert(std::make_pair(channels_name[index], c));
            channels[channels_name[index]] = c;
            new_channel(client, channels, c, channels_name[index]);
        }
    }
}

void Cmds::new_channel(Client &client, std::map<std::string, Channels> &channels, Channels &c, std::string channel_name)
{
    std::string protocol = ":" + client.get_nickname() + "!~" + client.get_username() + "@" + this->replay->getMachineHostName() + " JOIN " + channel_name + "\r\n" +
                           ":loclahost" + " MODE " + channel_name + " +nt\r\n" +
                           ":localhost" + " 353 " + client.get_nickname() + " = " + channel_name + " :@" + client.get_nickname() + "\r\n" +
                           ":localhost" + " 366 " + client.get_nickname() + " " + channel_name + " :End of /NAMES list\r\n";
    for (size_t it = 0; it < channels.at(c._channelName).members.size(); it++)
    {
        send(channels.at(c._channelName).members[it].get_fd(), protocol.c_str(), protocol.size(), 0);
    }
}

std::string Cmds::printChannelMemebers(std::string channelName, std::map<std::string, Channels> channel)
{
    std::string channelMemebers = "";
    std::string channelOperators = "";
    std::map<std::string, Channels>::iterator it = channel.find(channelName);
    if (it == channel.end())
        return "error";

    for (size_t i = 0; i < it->second.operators.size(); i++)
    {
        channelOperators += "@" + it->second.operators[i].get_nickname() + " ";
    }
    for (size_t i = 0; i < it->second.members.size(); i++)
    {
        if (!isModerator(it->second.members[i], it->second.operators))
            channelMemebers += it->second.members[i].get_nickname() + " ";
    }
    channelMemebers = channelOperators + channelMemebers;
    // channelMemebers = channelMemebers.substr(0, channelMemebers.rfind(' '));
    std::cout << channelMemebers << std::endl;
    return channelMemebers;
}

// void Cmds::RPL_NAMREPLY(std::string channel_name,std::map<std::string,Channels> channels,Client &client)
// {
//     std::string message = ":" + this->replay->getMachineHostName() + " 353 " + client.get_nickname() + " @ " + channel_name + " " + printChannelMemebers(channel_name, channels) +  client.get_nickname() + "\r\n";
//     send(client.get_fd(),message.c_str(),message.length(),0);
// }

// void Cmds::message_inform_clients(Client &client,std::map<std::string, Channels> &channels,std::string channel_name)
// {
//     std::map<std::string, Channels>::iterator channel_iterator = channels.find(channel_name);
//     std::string protocol = ":" + client.get_nickname() + "!~" + client.get_username() + "@" + this->replay->getMachineHostName() + " JOIN " + channel_name + "\r\n";

//     std::vector<Client>::iterator member_it = channel_iterator->second.members.begin();
//     for (; member_it != channel_iterator->second.members.end(); member_it++){
//         if(member_it->get_nickname() != client.get_nickname())
//             send(member_it->get_fd(), protocol.c_str(), protocol.size(), 0);
//         //RPL_NAMREPLY(channel_name,channels,*member_it);
//         this->replay->RPL_ENDOFNAMES(channel_name,client);
//     }
// }

void Cmds::message_inform_clients(Client &client, std::map<std::string, Channels> &channels, std::string channel_name)
{
    std::map<std::string, Channels>::iterator channel_iterator = channels.find(channel_name);
    std::string protocol = ":" + client.get_nickname() + "!~" + client.get_username() + "@" + this->replay->getMachineHostName() + " JOIN " + channel_name + "\r\n" +
                           ":localhost" + " 353 " + client.get_nickname() + " @ " + channel_name + " :" + printChannelMemebers(channel_name, channels) + "\r\n";
    std::vector<Client>::iterator member_it = channel_iterator->second.members.begin();
    for (; member_it != channel_iterator->second.members.end(); member_it++)
    {
        send(member_it->get_fd(), protocol.c_str(), protocol.size(), 0);
    }
}

void Cmds::already_channel(Client &client, std::map<std::string, Channels> &channels, std::string channel_name)
{

    std::map<std::string, Channels>::iterator channel_iterator = channels.find(channel_name);
    std::string protocol = ":" + client.get_nickname() + "!" + client.get_username() + "@" + this->replay->getMachineHostName() + " JOIN " + channel_name + "\r\n";
    send(client.get_fd(), protocol.c_str(), protocol.size(), 0);
    this->replay->RPL_TOPIC(channel_iterator->second);
    this->replay->RPL_TOPICWHOTIME(channel_iterator->second);
    // RPL_NAMREPLY(channel_name, channels, client);
    this->replay->RPL_ENDOFNAMES(channel_name, client);
}

void Cmds::bad_key(Client &client, std::string channel_name)
{
    std::string protocol = ":" + this->replay->getMachineHostName() + " 475 " + client.get_nickname() + " " + channel_name + " :Cannot join channel (+k) - bad key" + "\r\n";
    send(client.get_fd(), protocol.c_str(), protocol.size(), 0);
}

void Cmds::joining_a_channel_has_akey(Client &client, std::map<std::string, Channels> channels, std::string channel_name)
{
    (void)channels;
    (void)client;
    std::string protocol = ":" + client.get_nickname() + "!~" + client.get_username() + "@" + this->replay->getMachineHostName() + " JOIN " + channel_name + "\r\n";
    send(client.get_fd(), protocol.c_str(), protocol.size(), 0);
    // RPL_NAMREPLY(channel_name,channels,client);
    this->replay->RPL_ENDOFNAMES(channel_name, client);
}

void Cmds::invite_only(Client &client, std::string channel_name)
{
    std::string protocol = ":" + this->replay->getMachineHostName() + " 473 " + client.get_nickname() + " " + channel_name + " :Cannot join channel (+i)" + "\r\n";
    send(client.get_fd(), protocol.c_str(), protocol.size(), 0);
}

void Cmds::err_user_on_channel(Client &client, std::string channel_name)
{
    std::string protocol = ":" + this->replay->getMachineHostName() + " 443 " + client.get_nickname() + " " + channel_name + " :is already on channel" + "\r\n";
    send(client.get_fd(), protocol.c_str(), protocol.size(), 0);
}
