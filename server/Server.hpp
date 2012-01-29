#ifndef __SERVER_SERVER__
#define __SERVER_SERVER__

#include <list>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>

namespace Server
{
    class Network;
    class Client;

    class Server :
        private boost::noncopyable
    {
        public:
            Server();
            int Run(int ac, char** av);
            boost::asio::io_service& GetIoService();
            Network& GetNetwork();
            void AddClient(Client* client);
            void DeleteClientLater(Client* client);
            std::list<Client*> const& GetClients() const;
        private:
            enum
            {
                TickRate = 10000,
            };
            void _Tick(boost::system::error_code const& error);
            void _AsyncTick();
            bool _Init(int ac, char** av);
            void _Deinit();
            bool _DeleteClientNow(Client*);
            void _DeleteClients();
            boost::asio::io_service _ioService;
            Network* _network;
            std::list<Client*> _clients;
            std::list<Client*> _clientsKillPool;
            boost::asio::deadline_timer _tickTimer;
    };
}

#endif
