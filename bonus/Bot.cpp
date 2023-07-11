/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 05:10:32 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/11 08:25:00 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "../Client.hpp"
#include <unistd.h>


std::string getMachineHostName_()
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        return "";
    }
    return hostname;
}

void Bot::print_table(Client &c)
{
	std::istringstream iss(c.t->tab);
	std::string line;

	while (std::getline(iss, line))
	{
		std::string message = ":" + getMachineHostName_() + " 001 " +c.get_nickname() + " :" + line + "\r\n";
		send(c.get_fd(), message.c_str(), message.length(), 0);
		//write(1, message.c_str(), message.length());
	}
}

Bot::Bot()
{
	this->tab = "1 | 2 | 3\n-----------\n4 | 5 | 6\n-----------\n7 | 8 | 9\n\n\n\n";
}


bool Bot::get_x(std::string Parametres,Client &c)
{
	this->x = Parametres[0];
	if(c.t->tab.find(this->x) == std::string::npos){
		return false;
	}
	this->index = c.t->tab.find(this->x);
	return true;
}

bool Bot::get_y(Client &c)
{
	time_t present_time;
    present_time = time(NULL);
	this->y = std::to_string((std::rand() +  present_time) % 10);
	if(c.t->tab.find(this->y) == std::string::npos)
		return false;
	this->index = c.t->tab.find(this->y);
	return true;
}

void Bot::Replace(int n, int flag,Client &c)
{
	if (flag == 0)
		c.t->tab[n] = 'O';
	else if (flag == 1)
		c.t->tab[n] = 'X';
}
void Bot::Print()
{
	// std::cout << s.t->tab;
	std::cout << std::endl;
}

void print_winner(char v,Client &c)
{
	std::string message;
	if(v == 'X')
		message =  ":" + getMachineHostName_() + " 001 " +c.get_nickname() + " :The match ended with the victory of X \r\n\n\n";
	else if(v == 'O')
		message =  ":" + getMachineHostName_() + " 001 " +c.get_nickname() + " :The match ended with the victory of O (bot) \r\n\n\n";
	send(c.get_fd(), message.c_str(), message.length(), 0);
	
}
int Bot::check_result(int flag,Client &c)
{
	for (int i = 0; i < 66 ; i += 22) {
        if (c.t->tab[i] == c.t->tab[i + 4] && c.t->tab[i] == c.t->tab[i + 8]) {
			print_winner(c.t->tab[i],c); 
			return 1;
        }
    }
	for (int i = 0; i < 12 ; i+=4) {
        if (c.t->tab[i] == c.t->tab[i + 22] && c.t->tab[i] == c.t->tab[i + 44]) {
            print_winner(c.t->tab[i],c); 
			return 1;  // Return the winner (X or O)
        }
    }
	if (c.t->tab[0] == c.t->tab[26] && c.t->tab[0] == c.t->tab[52]) {
			print_winner(c.t->tab[0],c);
            return 1; 
    }
    if (c.t->tab[8] == c.t->tab[26] && c.t->tab[8] ==c.t->tab[44]) {
			print_winner(c.t->tab[8],c);
            return 1;
    }
	if (flag == 0){
			std::string message = ":" + getMachineHostName_() + " 001 " +c.get_nickname() + " :The match ended with draw \r\n";
			send(c.get_fd(), message.c_str(), message.length(), 0);
			return (1);
	}
	return (0);
}


void welcome_bot(Client &c)
{
   
    std::string message = ":" + getMachineHostName_() + " 001 " +c.get_nickname() +" :\\ \\        / /   ____| |    / ____/ __ \\|  \\/  |\r\n";
    send(c.get_fd(),message.c_str(),message.length(),0);
    message = ":" + getMachineHostName_() + " 001 " +c.get_nickname() +" : \\ \\  /\\  / / | |__  | |   | |   | |  | | \\  / |\r\n";
    send(c.get_fd(),message.c_str(),message.length(),0);
    message = ":" + getMachineHostName_() + " 001 " +c.get_nickname() +" :  \\ \\/  \\/ /  |  __| | |   | |   | |  | | |\\/| |\r\n";
    send(c.get_fd(),message.c_str(),message.length(),0);
    message = ":" + getMachineHostName_() + " 001 " +c.get_nickname() +" :   \\  /\\  /   | |____| |___| |___| |__| | |  | |\r\n";
    send(c.get_fd(),message.c_str(),message.length(),0);
    message = ":" + getMachineHostName_() + " 001 " +c.get_nickname() +" :    \\/  \\/    |______|______\\_____\\____/|_|  |_|\r\n";
    send(c.get_fd(),message.c_str(),message.length(),0);
    message = ":" + getMachineHostName_() + " 001 " +c.get_nickname() +" :                𝚝𝚑𝚒𝚜 𝚒𝚜 𝚊 𝚜𝚒𝚖𝚙𝚕𝚎 𝚋𝚘𝚝 ❞🆇/🅾❞\r\n";
    send(c.get_fd(),message.c_str(),message.length(),0);
    c.set_bot_checker(true);
}


bool Bot::bot_run(std::string Parametres,Client &c)
{
	std::cout << "Param is ------> :  "<< Parametres << std::endl;
	if(c.get_bot_checker() == false && Parametres == "run")
	{
        welcome_bot(c);
		return true;
	}
	else if(Parametres == "exit")
	{
		std::string message = ":" + getMachineHostName_() + " 001 " +c.get_nickname() +" :  ______     __ \r\n";
		send(c.get_fd(),message.c_str(),message.length(),0);
		message = ":" + getMachineHostName_() + " 001 " +c.get_nickname() +" : |  _ \\ \\   / /\r\n";
		send(c.get_fd(),message.c_str(),message.length(),0);
		message = ":" + getMachineHostName_() + " 001 " +c.get_nickname() +" : | |_) \\ \\_/ / \r\n";
		send(c.get_fd(),message.c_str(),message.length(),0);
		message = ":" + getMachineHostName_() + " 001 " +c.get_nickname() +" : |  _ < \\   /  \r\n";
		send(c.get_fd(),message.c_str(),message.length(),0);
		message = ":" + getMachineHostName_() + " 001 " +c.get_nickname() +" : | |_) | | |\r\n";
		send(c.get_fd(),message.c_str(),message.length(),0);
		message = ":" + getMachineHostName_() + " 001 " +c.get_nickname() +" : |____/  |_| \r\n";
		send(c.get_fd(),message.c_str(),message.length(),0);
		return false;
	}
	else if(get_x(Parametres,c))
	{
		Replace(this->index,1,c);
		print_table(c);
		if(check_result(1,c) == 1)
		{
			print_table(c);
			return false;
		}
		while(!get_y(c))
		{	
		}
		Replace(this->index,0,c);
		print_table(c);
		if(check_result(1,c) == 1)
		{
			print_table(c);
			return false;
		}
	}
	else 
	{
		std::string message = ":" + getMachineHostName_() + " 001 " +c.get_nickname() + " : enter a valid number \r\n";
		send(c.get_fd(), message.c_str(), message.length(), 0);
		return true;
	}
	return true;
}

void Cmds::bot(std::string Parametres,Client &c)
{
    std::cout << "Parametres bot is : " << Parameters << std::endl;
	if(Parameters.empty())
	{
		this->replay->ERR_NEEDMOREPARAMS();
		return;
	}
    if (c.get_bot_checker() == false && Parameters != "run")
    {
        std::string message =  ":" + getMachineHostName_() + " 001 " +c.get_nickname() + " : Enter \"BOT run\" to run this bot\r\n";
		send(c.get_fd(), message.c_str(), message.length(), 0);
        return;
    }
    if(strcmp(remove_(Parametres).c_str(), "exit") != 0  && remove_(Parametres).size() > 1 && !isdigit(Parametres[0]) && strcmp(remove_(Parametres).c_str(), "run") != 0)
    {
        std::string message =   ":" + getMachineHostName_() + " 001 " +c.get_nickname() + " : Enter exit command to exit this bot or number to play\r\n";
	    send(c.get_fd(), message.c_str(), message.length(), 0);
	    
    }
    else
    {
        if(!(c.t->bot_run(remove_(Parameters),c)))
            c.set_bot_checker(false);
    }
}
