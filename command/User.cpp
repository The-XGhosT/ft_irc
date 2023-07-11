/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 01:33:11 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/11 00:35:09 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

bool Cmds::check_user(std::string Parameters, Client &c)
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
        this->replay->ERR_NEEDMOREPARAMS();
        return false;
    }
    return true;
}

void Cmds::user(std::string Parameters, Client &c)
{
    std::vector<std::string> tokens = split_string(Parameters, ' ');
    if (tokens[1] == "0" && tokens[2] == "*")
    {
        c.username = tokens[0];
        if (tokens[3] == ":")
            c.realname = tokens[4] + " " + tokens[5];
        else
            c.realname = tokens[3] + " " + tokens[4];
        c.count++;
        if (c.count >= 3)
        {
            this->replay->WELCOM();
            c.IsRegistred = true;
        }
    }
    else
    {
        std::string message = ": valid format is : USER  <username> 0 * <realname>\r\n";
        send(c.get_fd(),message.c_str(),message.size(),0);
    }
}