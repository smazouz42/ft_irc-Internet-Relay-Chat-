/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smazouz <smazouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 01:29:00 by osallak           #+#    #+#             */
/*   Updated: 2023/02/18 19:39:20 by smazouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// # include "utils.hpp"
# include "Server.hpp"


int main(int ac, char **av)
{
    Server server;

    if (ac != 3)
    {
        std::cout << "Usage: ./irc [port] [password]" << std::endl;
        return (1);
    }

    server.setPassword(av[2]);//check if password is valid
    server.setPort(av[1]);//check if port is valid
    server.run();
}
