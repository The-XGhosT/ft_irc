/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 01:34:21 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/10 23:27:06 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

bool Cmds::check_nick(std::string Parameters, std::map<int, Client> &clients, Client &c)
{
    if(c.Ispassword == false)
    {
        this->replay->PASS_FIRST();
        return false;
    }
    if(c.IsRegistred == true)
    {
        this->replay->ERR_ALREADYREGISTRED();
        return false;
    }
    if (Parameters.empty())
    {
        this->replay->ERR_NONICKNAMEGIVEN();
        return false;
    }
    if (isdigit(Parameters[0]))
    {
        this->replay->ERR_ERRONEUSNICKNAME();
        return false;
    }
    const char *other = "[]{}\\|";
    for (int i = 1; i < (int)Parameters.size(); i++)
    {
        if (!isdigit(Parameters[i]) && !isalpha(Parameters[i]) && strchr(other, Parameters[i]) == NULL)
        {
            this->replay->ERR_ERRONEUSNICKNAME();
            return false;
        }
    }
    if (clients.size() > 0)
    {
        std::map<int, Client>::iterator to;
        to = clients.begin();
        for (; to != clients.end(); to++)
        {
            if (strcmp(to->second.nickname.c_str(), Parameters.c_str()) == 0)
            {
                if (c.IsRegistred == false)
                    this->replay->ERR_NICKNAMEINUSE();
                else
                    this->replay->ERR_NICKCOLLISION();
                return false;
            }
        }
    }
    return true;
}

void Cmds::nick(std::string Parameters, Client &c)
{
    c.nickname = Parameters;
    c.count++;
    if (c.count >= 3)
    {
        this->replay->WELCOM();
        c.IsRegistred = true;
    }
}