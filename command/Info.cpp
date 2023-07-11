/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Info.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 01:38:22 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/11 07:44:44 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"



std::string all_operators(std::vector<Client> operators)
{
    (void)operators;
    std::string channelOperators = "";
   
    for(std::vector<Client>::iterator it = operators.begin(); it != operators.end(); it++){
        channelOperators += it->get_nickname() + " ";
    }
    return channelOperators;
}
std::string all_members(std::vector<Client> members)
{
    (void)members;
    std::string channelMemebers = "";
    std::vector<Client>::iterator it = members.begin();
    for(; it != members.end(); it++){
        channelMemebers += it->get_nickname() + " ";
    }
    return channelMemebers;
}

std::string  all_modes(std::map<char,bool> modes){
    std::string modes_message = "";
    std::map<char,bool>::iterator it = modes.begin();
    for(; it != modes.end(); it++)
    {
        std::ostringstream oss;
        oss << it->first;;
        if(it->second == true)
            modes_message +=  "{\"" + oss.str() + "  : True \"}  , ";
        else
            modes_message +=  "{\"" + oss.str() + "  : False \"} , ";
    }
    modes_message[modes_message.length() - 2] = ' ';
    return modes_message;
}
std::string password_(bool haspassword, std::string pass)
{
    std::string password_message;
    if(haspassword == true)
    {
        password_message += "True  =" + pass;
    }
    else
    {
        password_message += "False  = empty";
    }
    return password_message;
}

std::string  Limit_(bool haslimit, int limit)
{
    std::string limit_message;
    if(haslimit == true)
    {
        limit_message +=  "True  : " + std::to_string(limit);
    }
    else
    {
        limit_message +=  "False : channel don't have a limit";
    }
    return limit_message;
}

bool Cmds::check_info(std::string Parametres)
{
    if(Parametres.empty())
    {
       this->replay->ERR_NEEDMOREPARAMS();
       return false;
    }
    return true;
}
#include <algorithm>

std::string in_box(std::string message)
{
    std::string new_message = "";
    std::string tmp;
    int max_line;
    std::istringstream iss(message);
    int number_of_line = 0;
    while (std::getline(iss,tmp)) {
       if(max_line < (int)tmp.length())
            max_line = (int)tmp.length();
        number_of_line++;
    }
    std::istringstream issa(message);
    int i = 0;
    while(std::getline(issa,tmp))
    {
        if(i > 1 && i < (number_of_line - 1))
        {
            while((int)tmp.length() <= max_line)
            {
                if((int)tmp.length() < max_line)
                    tmp += " ";
                if((int)tmp.length() == max_line)
                   tmp += '|';
                if((int)tmp.length() == max_line && tmp[max_line] != '|') 
                  tmp[max_line] = '|';
            }
           
        }
        else if(i == 1 || i == (number_of_line - 1))
        {
            while((int)tmp.length() <= max_line)
            {
                if((int)tmp.length() <= max_line)
                    tmp += "-";
                if((int)tmp.length() == max_line)
                    tmp += '|';
            }
        }
        i++;
        new_message += tmp + "\n";
    }
    return new_message;
}
void Cmds::infoc(std::string channel_name,std::map<std::string, Channels> channels,Client &c)
{
    std::map<std::string, Channels>::iterator channel_iterator = channels.find(channel_name);
    if(channel_iterator != channels.end())
    {
        std::string message = " \n|-------------------info channel--------------------\n|  Name       : "                 \
        + channel_iterator->second._channelName + "\n|  opearators :"                                                      \
        + all_operators(channel_iterator->second.operators)                                                                \
        + "\n|  members    : "  + all_members(channel_iterator->second.members)                                            \
        + "\n|  modes      : "   + all_modes(channel_iterator->second.modes)                                               \
        + "\n|  Password   : "  + password_(channel_iterator->second.modes['k'],channel_iterator->second._password)        \
        + "\n|  limit      : "     + Limit_(channel_iterator->second.modes['l'], channel_iterator->second.limit)           \
        + "\n|  Topic      : "     + channel_iterator->second._topic                                                       \
        + "\n|--------------------------------------------------";
        std::string protocol = ":"+ this->replay->getMachineHostName() + " 001 " + c.get_nickname() + ":";
        message = in_box(message) + "\r\n";
        std::string tmp;
        std::istringstream iss(message);
        while(std::getline(iss,tmp))
        {
            protocol +=tmp + "\r\n";
            send(c.get_fd(),protocol.c_str(),protocol.size(),0);
        }
    }
}