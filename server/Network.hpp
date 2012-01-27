#ifndef __SERVER_NETWORK__
#define __SERVER_NETWORK__

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>

namespace Server
{
    class Server;

    class Network :
        private boost::noncopyable
    {
        public:
            Network(Server& server, unsigned short porr);
            void DeleteSocket(boost::asio::ip::tcp::socket* socket);
        private:
            void _AsyncAccept();
            void _HandleAccept(boost::asio::ip::tcp::socket* socket, const boost::system::error_code& error);
            Server& _server;
            boost::asio::ip::tcp::acceptor _acceptor;
    };
}

#endif
