#ifndef __SERVER_CLIENT__
#define __SERVER_CLIENT__

#include <boost/asio.hpp>

namespace Common
{
    class Packet;
}

namespace Server
{
    class Server;
    class Connection;

    class Client
    {
        public:
            Client(Server& server, boost::asio::ip::tcp::socket* socket);
            ~Client();
            void Hello(Common::Packet& p);
        private:
            Server& _server;
            Connection* _connection;
    };
}

#endif
