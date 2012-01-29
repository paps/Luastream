#include <boost/cstdlib.hpp>
#include <boost/bind.hpp>
#include "Server.hpp"
#include "Network.hpp"
#include "Client.hpp"

namespace Server
{
    Server::Server() :
        _tickTimer(_ioService)
    {
    }

    int Server::Run(int ac, char** av)
    {
        if (!this->_Init(ac, av))
            return boost::exit_failure;
        this->_AsyncTick();
        this->_ioService.run();
        this->_Deinit();
        return boost::exit_success;
    }

    boost::asio::io_service& Server::GetIoService()
    {
        return this->_ioService;
    }

    Network& Server::GetNetwork()
    {
        return *this->_network;
    }

    void Server::AddClient(Client* client)
    {
        this->_clients.push_back(client);
    }

    void Server::_DeleteClients()
    {
        std::list<Client*>::iterator it = this->_clientsKillPool.begin();
        std::list<Client*>::iterator itEnd = this->_clientsKillPool.end();
        for (; it != itEnd; ++it)
            this->_DeleteClientNow(*it);
        this->_clientsKillPool.clear();
    }

    void Server::DeleteClientLater(Client* client)
    {
        this->_clientsKillPool.push_back(client);
    }

    bool Server::_DeleteClientNow(Client* client)
    {
        std::list<Client*>::iterator it = this->_clients.begin();
        std::list<Client*>::iterator itEnd = this->_clients.end();
        for (; it != itEnd; ++it)
            if (*it == client)
            {
                delete *it;
                return true;
            }
        return false;
    }

    std::list<Client*> const& Server::GetClients() const
    {
        return this->_clients;
    }

    void Server::_Tick(boost::system::error_code const& error)
    {
        if (error)
            return;
        this->_DeleteClients();
        std::list<Client*>::iterator it = this->_clients.begin();
        std::list<Client*>::iterator itEnd = this->_clients.end();
        for (; it != itEnd; ++it)
            (*it)->Tick();
        this->_AsyncTick();
    }

    void Server::_AsyncTick()
    {
        this->_tickTimer.expires_from_now(boost::posix_time::milliseconds(TickRate));
        this->_tickTimer.async_wait(boost::bind(&Server::_Tick, this, boost::asio::placeholders::error));
    }

    bool Server::_Init(int ac, char** av)
    {
        if (ac < 2)
        {
            std::cerr << "Usage: " << av[0] << " port" << std::endl;
            return false;
        }
        try
        {
            this->_network = new Network(*this, atoi(av[1]));
        }
        catch (std::exception& e)
        {
            std::cerr << "Could not listen on port " << atoi(av[1]) << ": " << e.what() << std::endl;
            return false;
        }
        return true;
    }

    void Server::_Deinit()
    {
        delete this->_network;
    }
}
