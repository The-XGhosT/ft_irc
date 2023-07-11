/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 01:43:57 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/11 01:45:04 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"
#include <chrono>
#include <ctime>


bool Cmds::check_topic(std::string Parametres,Client &c, std::map<std::string,Channels> &channels)
{
    std::map<std::string,Channels> v = channels; 
    if (Parameters.empty() || remove_(Parameters)[0] == ':')
    {
        this->replay->ERR_NEEDMOREPARAMS();
        return false;
    }
    std::vector<std::string> par;
    par = split_string(Parametres, ':');
    if(!hasChannel(remove_(par[0]),channels)){
        this->replay->ERR_NOSUCHCHANNEL();
        return false;
    }
    std::map<std::string, Channels>::iterator channel_iterator = channels.find(remove_(par[0]));
    if(!isModerator(c,channel_iterator->second.operators) && channel_iterator->second.modes['t'] == false)
    {
         this->replay->ERR_CHANOPRIVSNEEDED(channel_iterator->second._channelName);
         return false;
    }
    if(!isMember(c,channel_iterator->second.members))
    {
        this->replay->ERR_NOTONCHANNEL(channel_iterator->second._channelName);
        return false;
    }
    return true;
    
}


void Cmds::topic(std::string Parametres,std::map<std::string,Channels> &channels,Client &c)
{
    (void)c;
    std::vector<std::string> par;
    std::map<std::string, Channels>::iterator channel_iterator = channels.find(remove_(Parametres));
	if (Parametres.find(":") != std::string::npos && Parametres.back() == ':')
    {
        channels[Parameters.substr(0,Parameters.find(":") - 1)]._topic = "";
        this->replay->TOPIC_SET(channels[Parameters.substr(0,Parameters.find(":") - 1)]);
        return;
    }
    else if(Parametres.find(":") != std::string::npos && Parametres.back() != ':')
        par = split_string(Parametres, ':');
    else if(Parametres.find(":") == std::string::npos && !Parametres.empty())
    {
        if(channel_iterator->second._topic == "")
        {
            this->replay->RPL_NOTOPIC(channel_iterator->second);
            return;
        }
        else
        {
            this->replay->RPL_TOPIC(channel_iterator->second);
            this->replay->RPL_TOPICWHOTIME(channel_iterator->second);
            return;
        }
    }
      std::time_t timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    // Convert the timestamp to a string
    std::stringstream ss;
    ss << timestamp;
    std::string timestamp_str = ss.str();
    channel_iterator->second.timestamp_str  = timestamp_str;
    channel_iterator = channels.find(remove_(par[0]));
    channel_iterator->second._topic = remove_(par[1]);
    channel_iterator->second.whoSetTopic = c.get_nickname();
    channel_iterator->second.timestamp_str = timeTopic();
    this->replay->TOPIC_SET(channel_iterator->second);
}

