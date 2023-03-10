# include "Server.hpp"
#include <utility>
#include <cstdlib>//for atoi and stuff...
# include <algorithm>//for std::replace
# include <string>
# include "utils.hpp"

void    Server::parsePrivmsg(std::vector<std::string> &vec, int fd)
{
    std::string message = "";
    if (vec.size() == 0)
    {
        message = ":" + __users[fd].getNickname() + " 461 " + "PRIVMSG " + "Not enough parameters\n";
        sendMessage(fd, message);
        return ;
    }
    if (vec.size() < 2)
    {
        message = ":" + __users[fd].getNickname() + " 412" + " PRIVMSG " + "No text to send\n";
        sendMessage(fd, message);
        return ;
    }
    std::string msg = "";
    if (vec[1][0] != ':')
        msg += ":";
    for (size_t i = 1; i < vec.size(); ++i)
    {
        msg += vec[i];
        if (i != vec.size() - 1)
            msg += " ";
    }
    std::string targets = vec[0];
    std::vector<std::string> targetsVec;
    while (targets.find(',') != std::string::npos)
    {
        targetsVec.push_back(targets.substr(0, targets.find(',')));
        targets.erase(0, targets.find(',') + 1);
    }
    targetsVec.push_back(targets);
    for (size_t i = 0; i < targetsVec.size(); ++i)
    {
        if (targetsVec[i][0] == CHANNEL_PREFIX) {
            std::map<std::string, Channel>::iterator it = __channels.find(targetsVec[i]);// search for channel in the map
            if ( it == __channels.end() ) {
                message = ":" + __users[fd].getNickname() + " 403 " + "PRIVMSG " + "No such channel\n";
                sendMessage(fd, message);
                return;
            }
            Channel channel = it->second;
            if (isInChannel(channel, fd) == false) {
                message = ":" + __users[fd].getNickname() + " 404 " + "PRIVMSG " + "Cannot send to channel\n";
                sendMessage(fd, message);
                return;
            }
            std::map<int, Client>::const_iterator it2 = channel.getChannelClients().begin();
            for (; it2 != channel.getChannelClients().end(); ++it2) {
                if (it2->first == fd)
                    continue;
                message = ":" + __users[fd].getNickname() + " PRIVMSG " + targetsVec[i] + " " + msg + "\n";
                sendMessage(it2->first, message);
            }
        } else {
            int userId = GetUserId(targetsVec[i]);
            if (userId == -1){
                message = ":" + __users[fd].getNickname() + " 401 " + "PRIVMSG " + "No such nick\n";
                sendMessage(fd, message);
            } else if (userId == fd){
                message = ":" + __users[fd].getNickname() + " 400 " + "PRIVMSG " + "Cannot send to yourself\n";
                std::cout << "Cannot send to yourself" << std::endl;
                sendMessage(fd, message);
            } else {
                message = ":" + __users[fd].getNickname() + " PRIVMSG " + targetsVec[i] + " " + msg + "\n";
                sendMessage(userId, message);
            }
        }
    }
}
