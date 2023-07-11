/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmds.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 02:03:40 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/11 04:56:38 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDS_HPP
#define CMDS_HPP

#define PASS 0
#define NICK 1
#define USER 2
#define JOIN 3
#define MODE 4
#define TOPIC 5
#define KICK 6
#define PRIVMSG 7
#define INFO 8
#define INVITE 9
#define PART 10
#define PONG 11
#define BOT 12

#include <iostream>
#include <vector>
#include <sstream>
#include "Server.hpp"
#include "Replay.hpp"

class Replay;

class Cmds{
    private:
        std::string Command;
        std::string Parameters;
        Replay *replay;
    public:
        //constructors
        Cmds();
        ~Cmds();

        //setters
        void set_replay(Replay &replay);
        void set_command(std::string Command);
        void set_parametres(std::string Parameters);
        

        //getters
        Replay &get_replay();
        std::string get_command();
        std::string get_parametre();

        //all_command
        std::vector<std::string> init_my_command();
        bool check_command(std::string command , std::string Parameters,Client &c,std::map<int,Client> &clients,std::string pass_server,Server &s);

        // //check_every_command
        bool check_pass(std::string Parameters);
        bool check_nick(std::string Parameters,std::map<int,Client> &clients,Client &c);
        bool check_user(std::string Parameters,Client &c);
        bool check_join(std::string Parameters);
        bool check_mode(std::string Parameters,std::map<std::string,Channels> &channels, Client &client);
        bool check_topic(std::string Parametres,Client &c, std::map<std::string,Channels> &channels);
        bool check_kick(std::string Parametres);
        bool check_privmsg();
        bool check_info(std::string Parametres);
        bool check_invit();
        bool check_prvmsg(std::string Parametres);
        void prvmsg(std::string Parametres);

        //join method
        void new_channel(Client &client,std::map<std::string, Channels> &channels, Channels &c,std::string channel_name);
        void already_channel(Client &client,std::map<std::string, Channels> &channels,std::string channel_name);
        void message_inform_clients(Client &client,std::map<std::string, Channels> &channels,std::string channel_name);
        void bad_key(Client &client,std::string channel_name);
        void joining_a_channel_has_akey(Client &client,std::map<std::string,Channels> channels ,std::string channel_name);
        void reachLimit(std::map<std::string, Channels>::iterator channelsIt, Client &client, std::string channelName);
        void invite_only(Client &client,std::string channel_name);
        void err_user_on_channel(Client &client,std::string channel_name);


        // //commands
        void pass(std::string Parameters,Client &c,std::string pass_server);
        void nick(std::string Parameters, Client &c);
        void user(std::string Parameters, Client &c);
        void join(std::string Parametres, std::map<std::string, Channels> &channels, Client &client);
        void mode(std::string channel, std::map<std::string, Channels> &channels,Client &client);
        void topic(std::string Parametres,std::map<std::string,Channels> &channels,Client &c);
        void kick(std::string line, std::map<std::string, Channels> &channel, Client &c);
        void privateMessage(std::string command, std::map<std::string, Channels> &channels, std::map<int, Client> clients, Client &client);
        void infoc(std::string channel_name,std::map<std::string, Channels> channels,Client &c);
        void Invite(std::string Parameters, std::map<std::string, Channels> &channels, Client &c, std::map<int,Client> serverClients);
        bool isInvited(std::string nickname, std::vector<Client> _Invited);
        void part(std::string Parameters, std::map<std::string, Channels> &channels, Client &c);
        void bot(std::string Parametres,Client &c);
        
        //amounach
        std::map<std::string, Channels> serverChannels;
        void joinExistingChannel(int fd, std::string command, std::vector<std::string> listChannels, Server &s);
        void createChannel(int fd, std::string command, std::vector<std::string> listChannels, Server &s);

        //utils
        void RPL_NAMREPLY(std::string channel_name,std::map<std::string,Channels> channels,Client &client);
        bool add_target_user(std::string nickname, std::vector<Client> &operators, std::vector<Client> &members,std::string channelName);
        bool remove_target_user(std::string nickname, std::vector<Client> &operators, std::vector<Client> &members,std::string channelName);
        void parseJoinCommand(std::vector<std::string> &channelNames, Client c);
        void extractJoinParams(std::string cmd, std::vector<std::string> &password, std::vector<std::string> &command);
        std::string printChannelMemebers(std::string channelName, std::map<std::string, Channels> channel);
        std::string remove_(std::string line);
        bool check_mode(char m,std::map<char,bool>modes);
        bool hasChannel(std::string channelName, std::map<std::string, Channels> &channels);
        bool checkChannel(std::vector<std::string> str);
        bool isModerator(Client &c, std::vector<Client> &operators_);
        bool isMember(Client &c, std::vector<Client> &members);
        bool isMember(std::string nickName, std::vector<Client> &members);
        std::vector<std::string> split_string(const std::string &str, char delimiter);
        bool inviteOnlyChannel(std::map<std::string, Channels> &channels, std::string channelName, Client c);
        bool isClient(std::string nickname, std::map<int,Client> &clients);
        std::string timeTopic();
};

#endif
