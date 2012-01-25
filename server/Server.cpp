#include <boost/cstdlib.hpp>
#include "Server.hpp"
#include "Network.hpp"

namespace Server
{
    int Server::Run(int ac, char** av)
    {
        if (this->_Init(ac, av))
            return boost::exit_failure;
        this->_Deinit();
        return boost::exit_success;
    }

    Network& Server::GetNetwork()
    {
        return *this->_network;
    }

    bool Server::_Init(int ac, char** av)
    {
        this->_network = new Network();
        return true;
    }

    void Server::_Deinit()
    {
        delete this->_network;
    }
}
