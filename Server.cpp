/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 08:56:07 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/11 02:04:11 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include <iostream>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include "Cmds.hpp"

void socket_(int &socket_fd)
{
    socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if(socket_fd == -1)
    {
           std::cerr << RED << "Error to create a socket" << RESET << std::endl;
           exit(1);
    }
    std::cout << GREEN << "Socket is created" << RESET<< std::endl;
}

void bind_(int &socket_fd,int port)
{
    struct sockaddr_in	addr_server;
	addr_server.sin_family = AF_INET;
	// addr_server.sin_addr.s_addr = inet_addr(SERVER_IP);
	addr_server.sin_addr.s_addr = INADDR_ANY;
	addr_server.sin_port = htons(port);
    if(bind(socket_fd,(struct sockaddr *)&addr_server,sizeof(addr_server)) == -1)
    {
        close(socket_fd);
         std::cerr << RED << "Error to bind a socket" << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << GREEN << "Socket is bined to an adress "<< RESET << std::endl;
}

void listen_(int &socket_fd,int port)
{
    (void)port;
    if (listen(socket_fd, 5) == -1)
	{
         std::cerr << RED << "Error to listen a socket client" << RESET << std::endl;
         exit(1);
    }
    std::cout << GREEN << "Server listening on port " << port  << RESET << std::endl;
}

void FD_SET_(fd_set &readFds,int &socket_fd,std::vector<int> &fds,int &maxSd)
{
        FD_ZERO(&readFds);
        FD_SET(socket_fd, &readFds);
        maxSd = socket_fd;
        std::vector<int>::iterator it;
        it = fds.begin();
        for (; it != fds.end(); it++) {
            if (*it > 0) {
                FD_SET(*it, &readFds);
            }
            if (*it > maxSd) {
                maxSd = *it;
            }
        }
}

void select_(fd_set &readFds,int &maxSd)
{
    int status = select(maxSd + 1, &readFds, nullptr, nullptr, nullptr);
        if (status < 0) {
            perror("Select error");
            exit(EXIT_FAILURE);
        }
}

void FD_ISSET_ACCEPT(fd_set &readFds,int &socket_fd,std::vector<int> &fds,std::map<int,Client> &clients)
{
    struct sockaddr_in	serverAddress;
    socklen_t len = sizeof(serverAddress);
    
    if (FD_ISSET(socket_fd, &readFds))
    {
        int newSocket;
        if ((newSocket = accept(socket_fd, (struct sockaddr *)&serverAddress, &len)) < 0) {
            std::cerr << RED << "Accept error" << RESET << std::endl;
            exit(EXIT_FAILURE);
        }
        Client c("XXXX","","",0,0,false,false);
        clients[newSocket] = c;
        fds.push_back(newSocket);
    }
}


bool delete_client(std::map<int,Client> &clients, std::vector<int> &fds,int fd)
{
    std::map<int,Client>::iterator iter = clients.find(2);
    if (iter != clients.end()) {
        clients.erase(iter);
    }
    
    std::vector<int>::iterator it = std::find(fds.begin(), fds.end(), fd);
    if (it != fds.end()) {
        // Deleting the element
        fds.erase(it);
    }
    return true;
}

void Server::connection_with_sever(std::vector<int>&fds,std::map<int,Client> &clients,fd_set &readFds)
{
        for ( std::vector<int>::iterator it = fds.begin(); it != fds.end(); it++) 
        {
            int sd = *it;
            if (FD_ISSET(sd, &readFds)){
                    char buffer[1024];
                    std::string message = "";
                    do{
                        memset(buffer, 0, sizeof(buffer));
                        int valread = recv(sd,buffer,1024,0);
                        if (valread == 0) {
                            delete_client(clients,fds,sd);
                            std::cout <<  RED << "Client disconnected" << RESET << std::endl;
                            close(sd);
                            return;
                        }
                        message += std::string(buffer, valread);
                        std::cout << message << std::endl;

                    }while(message.find("\n") == std::string::npos);
                    if(message.length() > 0)
                    {
                        std::map<int, Client>::iterator it = clients.find(sd);
                        authentification(message,it->second,sd,clients);
                    }
            }
        }
}

Server ::Server()
{
}
void Server:: Create_server(std::string port,std::string password)
{
    int socket_fd;
    set_port(port), set_password(password);
    char buffer[BUFFER_SIZE] = {0};
    (void)buffer;
    const char* message = "Hello, World!";
    (void)message;
    socket_(socket_fd);
    int option = 1;

    // Set SO_REUSEADDR option
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1) {
        std::cerr << "Failed to set SO_REUSEADDR option: " << strerror(errno) << std::endl;
        close(socket_fd);
        exit(1);
    }
    
    bind_(socket_fd,get_port());
    listen_(socket_fd,get_port());
    
    fd_set readFds;
    std::vector<int> fds;
    while (true) {
        int maxSd = 0;
        FD_SET_(readFds,socket_fd,fds,maxSd);
        select_(readFds,maxSd);
        FD_ISSET_ACCEPT(readFds,socket_fd,fds,this->clients);
        connection_with_sever(fds,this->clients,readFds);
        
    } 
}

std::string remove(std::string line)
{
    std::string ws = " \n\r\t\f\v";
    size_t start = line.find_first_not_of(ws);
    size_t end = line.find_last_not_of(ws) + 1;
    size_t len = end - start;
    if(end != 0)
        line = line.substr(start,len);
    return line;
}

std::vector<std::string> splitString(const std::string& input, const std::string& delimiter) {
    std::vector<std::string> tokens;

    std::string::size_type start = 0;
    std::string::size_type end = input.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(input.substr(start, end - start));
        start = end + delimiter.length();
        end = input.find(delimiter, start);
    }

    tokens.push_back(input.substr(start));

    return tokens;
}


bool check_nickname(std::string nickname,std::map<int,Client>&clients)
{
    std::map<int,Client>::iterator to;
    to = clients.begin();
    for(;to != clients.end();to++)
    {
        if(strcmp(to->second.nickname.c_str(),nickname.c_str()) == 0)
            return false;
    }
    return true;
}

bool check_username(std::string username,std::map<int,Client>&clients)
{
    std::map<int,Client>::iterator to;
    to = clients.begin();
    for(;to != clients.end();to++)
    {
        if(strcmp(to->second.username.c_str(),username.c_str()) == 0)
            return false;
    }
    return true;
}


bool Server::authentification(std::string message,Client &c,int sd,std::map<int,Client> &clients)
{
    Cmds cmd;
    Replay replay;
    std::string command;
    
    cmd.set_replay(replay);
    c.set_fd(sd);
    if(message.find(' ') !=  std::string::npos)
    {
        command = message.substr(0,message.find(' '));
        std::transform(command.begin(), command.end(), command.begin(), ::toupper);
        cmd.set_command(remove(command));
    }
    else
    {
        command = message.substr(0);
        std::transform(command.begin(), command.end(), command.begin(), ::toupper);
        cmd.set_command(remove(command));
    }
    if(message.find(' ') !=  std::string::npos)
        cmd.set_parametres(remove(message.substr(message.find(' '))));
    else
        cmd.set_parametres(remove(message.substr(0)));
    cmd.check_command(cmd.get_command(),cmd.get_parametre(),c,clients,get_password(),*this);
    return true;
}

Server:: ~Server()
{
    
}

void Server:: set_port(std::string port)
{
    this->port = atof(port.c_str());
}

void Server:: set_password(std::string password)
{
    this->password = password;
}

unsigned int Server:: get_port()
{
    return this->port;
}

std::string Server:: get_password()
{
    return this->password;
}

void Server::set_fd_server(int fd)
{
    this->socket_server = fd;
}
int Server::get_fd_server()
{
    return this->socket_server;
}



void Server::set_map(int key, Client Value)
{
    this->clients.insert(std::make_pair(key,Value));
}

std::map<int,Client>Server:: get_map()
{
    return this->clients;
}

void Server:: set_clients_fd(int fd)
{
    this->clients_fd.push_back(fd);
}
std::vector<int> Server::get_clients_fd()
{
    return this->clients_fd;
}

