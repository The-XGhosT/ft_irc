/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaji-el <anaji-el@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:35:55 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/03 16:19:14 by anaji-el         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int argc, char **argv)
{
    (void)argv;
    if (argc != 3)
    {
        std::cout  << GREEN  << "\n./ircserv " << RESET << "<port>" << RED << " <password>\n"  << RESET<< std::endl;
        return 1;
    }
    Server s;
    s.Create_server(argv[1],argv[2]);
} 