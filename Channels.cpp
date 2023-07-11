/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 01:34:51 by iqabbal           #+#    #+#             */
/*   Updated: 2023/06/24 12:09:17 by amounach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"
#include "Client.hpp"
#include <sstream>
#include "Channels.hpp"

Channels::Channels()
{
}
Channels::~Channels()
{
}

Channels::Channels(std::string channelName, std::string topic, std::string password, Client client)
{
	this->_channelName = channelName;
	this->_topic = topic;
	this->_password = password;
	this->members.push_back(client);
}


void Channels::setChannelName(std::string channelName)
{
	this->_channelName = channelName;
}

void Channels::setTopic(std::string topic)
{
	this->_topic = topic;
}

void Channels::setPassword(std::string password)
{
	this->_password = password;
}

std::string Channels::getChannelName()
{
	return this->_channelName;
}

std::string Channels::getTopic()
{
	return this->_topic;
}

std::string Channels::getPassword()
{
	return this->_password;
}

// JOIN command

std::string getChannelName(std::string line)
{
	std::string channelName = line.substr(line.find(' ') + 1);
	return channelName;
}

std::string getNickName(std::map<int, Client> Users)
{
	std::string nickName;
	for (std::map<int, Client>::iterator it = Users.begin(); it != Users.end(); it++)
	{
		Client &client = it->second;
		nickName = client.get_nickname();
	}
	return nickName;
}

std::string getUserName(std::map<int, Client> Users)
{
	std::string UserName;
	for (std::map<int, Client>::iterator it = Users.begin(); it != Users.end(); it++)
	{
		Client &client = it->second;
		UserName = client.get_username();
	}
	return UserName;
}
