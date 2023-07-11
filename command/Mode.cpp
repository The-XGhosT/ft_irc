/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 03:38:52 by anaji-el          #+#    #+#             */
/*   Updated: 2023/07/10 23:46:36 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

std::string get_modes(std::map<char,bool>  modes)
{
    std::string _modes = "+";
    std::map<char,bool>::iterator it = modes.begin();
    for(; it != modes.end(); it++)
    {
        if(it->second == true)
            _modes += it->first;
    }
    return _modes;
}

void Replay::RPL_CHANNELMODEIS (std::string channel_name,Client &client,std::map<std::string, Channels> &channels)
{
    std::string message = ":"+ getMachineHostName() + " 324 " + client.get_nickname() + " " + channel_name + " " + get_modes(channels[channel_name].modes) + "\r\n";
    send(client.get_fd(),message.c_str(),message.length(),0);

}
//sendReply( + channelobj->getChannelCreationTime() + "\r\n"); //RPL_CREATIONTIME (329)

void Replay::RPL_CREATIONTIME (std::string channel_name,Client &client,std::map<std::string, Channels> &channels)
{
    std::string message = ":"+ getMachineHostName() + " 329 " + client.get_nickname() + " " + channel_name + " " + channels[channel_name].timestamp_str + "\r\n";
    send(client.get_fd(),message.c_str(),message.length(),0);
}

bool Cmds::check_mode(std::string Parameters,std::map<std::string,Channels> &channels, Client &client)
{
    std::vector<std::string> command;
    std::vector<std::string> arguments;
    if(Parameters.empty())
    {
        this->replay->ERR_NEEDMOREPARAMS();
        return false;
    }
    command = split_string(Parameters, ' ');
    if(!hasChannel(command[0],channels))
    {
        this->replay->ERR_NOSUCHCHANNEL();
        return false;
    }
    std::map<std::string, Channels>::iterator channel_iterator = channels.find(command[0]);
    if(command.size() == 1 && hasChannel(command[0],channels) && isMember(client,channel_iterator->second.members))
    {
        this->replay->RPL_CHANNELMODEIS(command[0],client,channels);
        this->replay->RPL_CREATIONTIME(command[0],client,channels);
        return false;
    }
    if(!isModerator(client,channel_iterator->second.operators))
    {
        this->replay->ERR_CHANOPRIVSNEEDED(command[0]);
        return false;
    }
    
    //mazal had error : RPL_CHANNELMODEIS && RPL_CREATIONTIME
    
    return true;
}

Client  &get_client(std::string nickname,std::vector<Client> &members)
{
    std::vector<Client>::iterator it = members.begin();
    for(;it != members.end(); it++)
    {
        if(nickname == it->get_nickname())
            break;
    }
    return *it;
}

bool Cmds::add_target_user(std::string nickname, std::vector<Client> &operators, std::vector<Client> &members,std::string channelName)
{
    std::vector<Client>::iterator it = members.begin();
    for (; it != members.end(); it++)
    {
        int index = std::distance(members.begin(), it);
        if (members[index].get_nickname() == nickname)
        {
            if (isModerator(get_client(nickname,members),operators)){   
               std::cerr << "you're already operator in this channel" << std::endl;
            }
        else
            operators.push_back(members[index]);
            return true;
        }
    }
    this->replay->ERR_NOTONCHANNEL(channelName);
    return false;
}



bool Cmds::remove_target_user(std::string nickname, std::vector<Client> &operators, std::vector<Client> &members,std::string channelName)
{
    std::vector<Client>::iterator it = members.begin();
    for (; it != members.end(); it++)
    {
        int index = std::distance(members.begin(), it);
        if (members[index].get_nickname() == nickname)
        {
            operators.erase(operators.begin() + index);
            return true;
        }
    }
    this->replay->ERR_NOTONCHANNEL(channelName);
    return false;
}
std::vector<std::string> init_arguments(std::vector<std::string> command)
{
    std::vector<std::string> arguments;
    
    for(size_t i = 2; i < command.size() ; i++)
    {
        arguments.push_back(command[i]);
    }
    return arguments;
}
bool check_not_digit(std::string command)
{
    for(size_t i = 0; i < command.length(); i++)
    {
        if(!isdigit(command[i]))
            return false;
    }
    return true;
}
bool just_space_(std::string Parameters)
{
    for(size_t i = 0; i < Parameters.size() ; i++)
    {
        if(Parameters[i] != ' ' && Parameters[i] != '\n')
            return false;
    }
    return true;
}

bool check_vector(std::vector<std::string> vec,int index)
{
    if(vec.empty())
    {
        return false;
    }
    if(vec[index].empty() || just_space_(vec[index]))
    {
        return false;
    };
    return true;
}
void Cmds::mode(std::string channel, std::map<std::string, Channels> &channels, Client &client)
{
   (void)client;
    int track_parametre = 0;
    std::vector<std::string> command;
    std::vector<std::string> arguments;
    command = split_string(channel, ' ');
    if(command.size() > 2)
        arguments = init_arguments(command);
    bool flag;
    if (command[1][0] == '-')
        flag = false;
    else if (command[1][0] == '+')
        flag = true;
    std::map<std::string, Channels>::iterator channel_iterator = channels.find(command[0]);
    for (int i = 1; i < (int)command[1].size(); i++)
    {
        std::map<char, bool>::iterator mode = channel_iterator->second.modes.find(command[1][i]);
        if (mode->first == 'o')
        {
            if(!check_vector(arguments,track_parametre))
            {
                std::cerr << "more param";
                return;
            }
            if (flag == true)
            {
                add_target_user(arguments[track_parametre++], channel_iterator->second.operators, channel_iterator->second.members,channel_iterator->second._channelName);
                message_inform_clients(client, channels,channel_iterator->second._channelName);
                // RPL_NAMREPLY(command[0],channels,client);
                // message_inform_clients(client,channels,command[0]);
            }
            else if(flag == false)
            {
                remove_target_user(arguments[track_parametre++], channel_iterator->second.operators, channel_iterator->second.members,channel_iterator->second._channelName);
                message_inform_clients(client, channels,channel_iterator->second._channelName);
                // RPL_NAMREPLY(command[0],channels,client);
                // message_inform_clients(client,channels,command[0]);
            }       
        }
        if (mode->first == 'i')
        {
            if (flag == true)
                channel_iterator->second.modes['i'] = true;
            else
                channel_iterator->second.modes['i'] = false;
        }
        if (mode->first == 'k')
        {
            if (flag == true)
            {
                channel_iterator->second.modes['k'] = true;
                if(!check_vector(arguments,track_parametre))
                {
                    std::cerr << "more param";
                    return;
                }
                else
                {
                    
                    channel_iterator->second._password = arguments[track_parametre++];
                }
            }
            else
                mode->second = false;
        }
        if (mode->first == 't')
        {
            if (flag == true)
                channel_iterator->second.modes['t'] = true;
            else
               channel_iterator->second.modes['t'] = false;
        }
        if (mode->first == 'l')
        {
            if (flag == true)
            {
                channel_iterator->second.modes['l'] = true;
                if(!check_vector(arguments,track_parametre))
                {
                    std::cerr << "more param";
                    return;
                }
                else
                {
                    if(check_not_digit(arguments[track_parametre]))
                        channel_iterator->second.limit = (int)atof(arguments[track_parametre++].c_str());
                    else
                        std::cerr << "not digit" << std::endl;
                }
            }
                
            else
                channel_iterator->second.modes['l'] = false;
        }
    }
}


//messages when setting modes
