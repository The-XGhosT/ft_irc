/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 08:56:00 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/11 02:26:26 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <arpa/inet.h>
# include <fcntl.h>
#include <cstring>
# include <poll.h>
# include <vector>
# include "Client.hpp"
#include <unistd.h>
#include <vector>
#include <map>
#include <algorithm>
#include "Channels.hpp"
#include "Cmds.hpp"
#include "bonus/Bot.hpp"

#define BUFFER_SIZE 1024

const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string BIG_TEXT = "\033[2m";
const std::string BOLD_TEXT = "\033[1m";

class Client;
class Cmds;
class Server{
    private:
       
        unsigned int            port;
        std::string             password;
        int                     socket_server;
        std::vector<int>        clients_fd;
    public:
        //amounach
 //constructor and destructor
        std::map<int,Client>    clients;
         std::map<std::string, Channels> channels;
        std::string message;
        Server();
        ~Server();

        //set functions
        
        void set_port(std::string port);
        void set_password(std::string password);
        void set_fd_server(int fd);
        void set_map(int key, Client Value);
        void set_clients_fd(int fd);
        //get functions
        int get_fd_server();
        unsigned int get_port();
        std::string get_password();
        std::map<int,Client> get_map();
        std::vector<int> get_clients_fd();
        // std::map<int,Client> get_map();
        //methods

        void Create_server(std::string port,std::string password);
        bool authentification(std::string message,Client &c,int sd,std::map<int,Client> &clients);
        void connection_with_sever(std::vector<int>&fds,std::map<int,Client> &clients,fd_set &readFds);
};

#endif
