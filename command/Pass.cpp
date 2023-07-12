/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqabbal <iqabbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 01:34:23 by iqabbal           #+#    #+#             */
/*   Updated: 2023/07/12 06:48:25 by iqabbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"


bool Cmds::check_pass(std::string Parameters)
{
    std::cout << "size is : " << Parameters.size() << std::endl;
    std::cout << "Param is : " << Parameters << std::endl;
    if (Parameters.empty())
    {
        this->replay->ERR_NEEDMOREPARAMS();
        return false;
    }
    return true;
}

void Cmds::pass(std::string Parameters, Client &c, std::string pass_server)
{
    if (c.IsRegistred == true)
    {
         this->replay->ERR_ALREADYREGISTRED();
         return;
    }
    if(c.Ispassword == true )
    {
        this->replay->PASS_ALREADY();
        return;
    }
    else if (strcmp(Parameters.c_str(), pass_server.c_str()) != 0)
    {
        this->replay->ERR_PASSWDMISMATCH();
        return;
    }
    else if (c.IsRegistred == false && strcmp(Parameters.c_str(), pass_server.c_str()) == 0)
    {
        c.Ispassword = true;
        c.count = 1;
    }
}
