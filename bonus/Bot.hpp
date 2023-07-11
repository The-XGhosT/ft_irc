/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 05:17:06 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/11 02:17:35 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include "../Client.hpp"
class Client;
class Bot{
    public:
        std::string tab;
        std::string x;
        std::string y;
        int index;
        //0-4-8-22-26-30-44-48-52
    public:
        //getter
        Bot();
        bool get_x(std::string Parametres,Client &c);
        bool get_y(Client &c);
        void Replace(int n, int flag,Client &c);
        void Print();
        int  check_result(int flag,Client &c);
        bool bot_run(std::string Parametres,Client &c);
        void print_table(Client &c);
};

#endif
