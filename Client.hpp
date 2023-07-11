/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 15:55:06 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/11 02:30:28 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

# include "Server.hpp"
# include "bonus/Bot.hpp"
class Bot;
class Client{
    public:
        Client(std::string nickname,std::string username,std::string realname,int fd,int count,bool IsRegistred,bool Ispassword);
        std::string nickname;
        std::string realname;
        std::string username;
        int fd;
        bool IsRegistred;
        bool Ispassword;
        int count;
        bool bot_checker;
        Bot *t;
    public:
        Client();
        void set_nickname(std::string nickname);
        void set_username(std::string username);
        void set_fd(int fd);
        void set_status(bool IsRegistred);
        void set_count(int count);
        void set_realname(std::string realname);
        void set_bot_checker(bool checker);
        std::string get_nickname();
        std::string get_username();
        std::string get_realname();
        bool get_bot_checker();
        int  get_fd();
        bool get_status();
        int get_count();
};

#endif