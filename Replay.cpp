/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replay.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 01:36:07 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/11 03:10:20 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


//:localhosst number nickname: message
#include "Replay.hpp"

Replay::Replay()
{
    this->c = new Client();
}

Replay::~Replay()
{
    
}
void Replay::set_client(Client &c)
{
    this->c = &c;
}
Client &Replay::get_client()
{
    return *(this->c);
}
std::string Replay::getMachineHostName()
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        return "";
    }
    return hostname;
}

void Replay::ERR_NEEDMOREPARAMS()
{
    
    std::string message =  ":" + getMachineHostName() + " 461 " +  this->c->get_nickname() + " :Not enough parameters" + "\r\n";
    send(this->c->get_fd(),message.c_str(),message.length(),0);
}
void Replay::ERR_NONICKNAMEGIVEN ()
{
    std::string message =   ":" + getMachineHostName() + " 431 " + this->c->get_nickname() + " :" + "No nickname given" + "\r\n";
    send(this->c->get_fd(),message.c_str(),message.length(),0);
}

void Replay::ERR_ERRONEUSNICKNAME ()
{
    std::string message =   ":" + getMachineHostName() + " 432 " + this->c->get_nickname() + " :" + "Erroneus nickname" + "\r\n";
    send(this->c->get_fd(),message.c_str(),message.length(),0);
}

void Replay::ERR_NICKNAMEINUSE ()
{
    std::string message =   ":" + getMachineHostName() + " 433 " + this->c->get_nickname() + " :" + "Nickname is already in use" + "\r\n";
    send(this->c->get_fd(),message.c_str(),message.length(),0);
}

void Replay::ERR_ALREADYREGISTRED()
{
    std::string message =   ":" + getMachineHostName() + " 462 " + this->c->get_nickname() + " :" + "You may not reregister" + "\r\n";
   send(this->c->get_fd(),message.c_str(),message.length(),0);
}

void Replay::ERR_NICKCOLLISION ()
{

    std::string message =   ":" + getMachineHostName() + " 436 " + this->c->get_nickname() + " :" + "Nickname collision KILL from" + "\r\n";
    send(this->c->get_fd(),message.c_str(),message.length(),0);
}

void Replay::ERR_PASSWDMISMATCH()
{
    std::string message =   ":" + getMachineHostName() + " 464 " + this->c->get_nickname() + " :" + "Password incorrect" + "\r\n";
    send(this->c->get_fd(),message.c_str(),message.length(),0);
}

void Replay::ERR_CHANOPRIVSNEEDED(std::string channelName)
{
    (void)channelName;
    std::string message =  ":" + getMachineHostName() + " 482 " + this->c->get_nickname() + " :You're not channel operator\r\n";
    send(this->c->get_fd(), message.c_str(), message.length(), 0);
}

void Replay::ERR_NOTONCHANNEL(std::string channelName)
{
    std::string message =  ":" + getMachineHostName() + " 442 " + this->c->get_nickname() + " " + channelName  + " :You're not on that channel\r\n";
    send(this->c->get_fd(), message.c_str(), message.length(), 0);
}

void Replay::ERR_NOSUCHCHANNEL()
{
    std::string message =  ":" + getMachineHostName() + " 403 " + this->c->get_nickname()   + " :No such channel\r\n";
    send(this->c->get_fd(), message.c_str(), message.length(), 0);
}

void Replay::RPL_TOPICWHOTIME(Channels &channel)
{
    std::string message =  ":" + getMachineHostName() + " 332 " + channel.whoSetTopic + " " + channel._channelName  + " :TOPIC " + channel.timestamp_str+ "\r\n";
    send(this->c->get_fd(), message.c_str(), message.length(), 0);
}

void Replay::ERR_USERONCHANNEL()
{
    std::string message =   ":" + getMachineHostName() + " 443 " + this->c->get_nickname() + " :" + "is already on channel" + "\r\n";
    send(this->c->get_fd(),message.c_str(),message.length(),0);
}

void Replay::ERR_CANNOTSENDTOCHAN()
{
    std::string message =   ":" + getMachineHostName() + " 404 " + this->c->get_nickname() + " :Cannot send to channel\r\n";
    send(this->c->get_fd(),message.c_str(),message.length(),0);

}

void Replay::ERR_NOSUCHNICK()
{
    std::string message =   ":" + getMachineHostName() + " 401 " + this->c->get_nickname() + " :No such nick/channel" + "\r\n";
    send(this->c->get_fd(),message.c_str(),message.length(),0);
}

void Replay::WELCOM()
{
std::string message = ":" + this->getMachineHostName() + " 001 " + this->c->get_nickname()+" :Welcome to Our IRC Server!\r\n"; \
            send(this->c->get_fd(),message.c_str(),message.length(),0);
            message = ":" + getMachineHostName() + " 001 " + this->c->get_nickname() + " :Your hostname is " + getMachineHostName() + "\r\n";
            send(this->c->get_fd(),message.c_str(),message.length(),0);
            message = ":" + getMachineHostName() + " 001 " + this->c->get_nickname() +" :     ______        _             \r\n";
            send(this->c->get_fd(),message.c_str(),message.length(),0);
            message = ":" + getMachineHostName() + " 001 " + this->c->get_nickname() +" :    / ____/___    (_)___  __  __ \r\n";
            send(this->c->get_fd(),message.c_str(),message.length(),0);
            message = ":" + getMachineHostName() + " 001 " + this->c->get_nickname() +" :   / __/ / __ \\  / / __ \\/ / / / \r\n";
            send(this->c->get_fd(),message.c_str(),message.length(),0);
            message = ":" + getMachineHostName() + " 001 " + this->c->get_nickname() +" :  / /___/ / / / / / /_/ / /_/ /  \r\n";
            send(this->c->get_fd(),message.c_str(),message.length(),0);
            message = ":" + getMachineHostName() + " 001 " + this->c->get_nickname() +" : /_____/_/ /_/_/ /\\____/\\__, /   \r\n";
            send(this->c->get_fd(),message.c_str(),message.length(),0);
            message = ":" + getMachineHostName() + " 001 " + this->c->get_nickname() +" :            /___/      /____/    \r\n";
            send(this->c->get_fd(),message.c_str(),message.length(),0);
}

void Replay::COMMAND_NOT_FOUND(std::string command)
{
    std::string message =   ":" + getMachineHostName() + " 421 " + this->c->get_nickname() + " : "  + command + " Unknown command"  + "\r\n";
    send(this->c->get_fd(),message.c_str(),message.length(),0);
}



void Replay::RPL_NOTOPIC(Channels &channel)
{
    std::string message =   ":" + getMachineHostName() + " 331 " + this->c->get_nickname() + channel._channelName + " :"  + "No topic is set"  + "\r\n";
    send(this->c->get_fd(),message.c_str(),message.length(),0);

}

void Replay::RPL_TOPIC(Channels &channel)
{
    std::string message =   ":" + getMachineHostName()  + " 332 " + this->c->get_nickname() + " " + channel._channelName +  " :"  + channel._topic + "\r\n";
    send(this->c->get_fd(),message.c_str(),message.length(),0);
}

void Replay::TOPIC_SET(Channels &channel)
{
    std::string message = ":" + this->c->get_nickname() + "!" + this->c->get_username()+ "@" + getMachineHostName()  + " TOPIC " +channel._channelName + " :"  + channel._topic  + "\r\n";
    send(this->c->get_fd(),message.c_str(),message.length(),0);
}


void Replay:: RPL_ENDOFNAMES(std::string channel_name,Client &client)
{
    std::string message = ":" + getMachineHostName() + " 366 " + client.get_nickname() + " = " + channel_name + " :End of /NAMES list\r\n";
    send(this->c->get_fd(),message.c_str(),message.length(),0);
}




void Replay::PASS_ALREADY()
{
    std::string message =   ":" + getMachineHostName()  + " 004 " + this->c->get_nickname() + " :"  + "Already enter a password"  + "\r\n";
    send(this->c->get_fd(),message.c_str(),message.length(),0);
}
void Replay::PASS_FIRST()
{
    std::string message =   ":" + getMachineHostName()  + " 004 " + this->c->get_nickname() + " :"  + "Please Enter a password"  + "\r\n";
    send(this->c->get_fd(),message.c_str(),message.length(),0);
}

void Replay::NOT_REGISTER()
{
    std::string message =   ":" + getMachineHostName()  + " 004 " + this->c->get_nickname() + " :"  + "You are not register"  + "\r\n";
    send(this->c->get_fd(),message.c_str(),message.length(),0);
}
