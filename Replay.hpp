/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replay.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 01:36:04 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/11 03:07:06 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLAY_HPP
#define REPLAY_HPP

#include "Server.hpp"
#include "Client.hpp"

class Client;
class Server;

class Replay{
  private:
   Client *c;
  public:
   Replay();
   ~Replay();
   void set_client(Client &c);
   Client &get_client();
   std::string getMachineHostName();
   void ERR_NEEDMOREPARAMS();
   void ERR_NONICKNAMEGIVEN ();
   void ERR_ERRONEUSNICKNAME ();
   void ERR_NICKNAMEINUSE ();
   void ERR_ALREADYREGISTRED();
   void ERR_NICKCOLLISION ();
   void ERR_PASSWDMISMATCH();
   void WELCOM();
   void COMMAND_NOT_FOUND(std::string command);
   void RPL_NOTOPIC(Channels &channel);
   void TOPIC_SET(Channels &channel);
   void RPL_TOPIC(Channels &channel);
   void ERR_CHANOPRIVSNEEDED(std::string channelName);
   void RPL_CHANNELMODEIS (std::string channel_name,Client &client,std::map<std::string, Channels> &channels);
   void RPL_CREATIONTIME (std::string channel_name,Client &client,std::map<std::string, Channels> &channels);
   void ERR_CANNOTSENDTOCHAN();
   void ERR_NOSUCHNICK();
   void PASS_ALREADY();
   void PASS_FIRST();
   void ERR_NOTONCHANNEL(std::string channelName);
   void NOT_REGISTER();
   void ERR_NOSUCHCHANNEL();
   void RPL_TOPICWHOTIME(Channels &channel);
  void  ERR_USERONCHANNEL();
  //join
  void RPL_ENDOFNAMES(std::string channel_name,Client &client);
  
};

#endif
