/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaji-el <anaji-el@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 01:34:45 by iqabbal           #+#    #+#             */
/*   Updated: 2023/06/25 12:58:06 by anaji-el         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include "Server.hpp"
#include "Client.hpp"
#include <iostream>
#include <map>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdint>
#include <netdb.h>
#include <vector>
#include <cstring>
#include <cerrno>
#include <string>
#include <vector>
#include <string>
#include <map>

class Server;
class Client;

class Channels
{
public:
		std::string _channelName;
		std::string _topic;
		std::string _password;
		std::vector<Client>operators;
		std::vector<Client>members;
		std::map<char,bool> modes;
		std::vector<std::string> password;
		std::vector<std::string> invited_only;
		std::string whoSetTopic;
		std::string timestamp_str;
		bool HasPassword;
		Channels();
		Channels(std::string channelName, std::string topic,std::string password, Client client);
		~Channels();
		//setters
		void setChannelName(std::string channelName);
		void setTopic(std::string topic);
		void setPassword(std::string password);
		//getters
		std::string getChannelName();
		std::string getTopic();
		std::string getPassword();

		// std::vector<std::string> _channelOperators;
		std::map<char, bool> _channelMods;
		int limit;
		std::string getNickName(std::map<int, Client> Users);
		std::string getUserName(std::map<int, Client> Users);
		void extractJoinParams(std::string command);
		void createChannel(int fd, std::string command, std::vector<std::string> listChannels, Server &s);
		std::string getChannelName(std::string line);
		//std::string getMachineHostName();
};
void parseJoinCommand(std::string command, std::vector<std::string> &channelsName);
#endif

