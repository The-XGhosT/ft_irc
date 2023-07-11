/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 15:55:08 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/11 08:22:36 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

    Client::Client(std::string nickname,std::string username,std::string realname,int fd,int count,bool IsRegistred,bool Ispassword)
    {
        set_nickname(nickname);
        set_username(username);
        set_realname(realname);
        set_fd(fd);
        set_count(count);
        set_status(IsRegistred);
        this->Ispassword = Ispassword;
        this->set_bot_checker(false);
        this->t = new  Bot();
    }
    
    Client::Client()
    {
        
    }
    
    void Client:: set_nickname(std::string nickname)
    {
        this->nickname = nickname;
    }
    void Client:: set_username(std::string username)
    {
        this->username = username;
    }
    void Client:: set_fd(int fd)
    {
        this->fd = fd;
    }
    void Client:: set_status(bool IsRegistred)
    {
        this->IsRegistred = IsRegistred;
    }
    void Client:: set_count(int count)
    {
        this->count = count;
    }
    
    void Client::set_realname(std::string realname)
    {
        this->realname = realname;
    }
    
    std::string Client:: get_nickname()
    {
        return this->nickname;
    }

    std::string Client:: get_username()
    {
        return this->username;
    }

    bool Client::get_status()
    {
        return this->IsRegistred;
    }
    
    int Client::get_count()
    {
       return this->count; 
    }
    
    int Client::get_fd()
    {
        return this->fd;
    }
    
    std::string Client::get_realname()
    {
        return this->realname;
    }
    
    bool Client::get_bot_checker()
    {
        return this->bot_checker ;
    }
    
    void Client::set_bot_checker(bool checker)
    {
        this->bot_checker = checker;
    }