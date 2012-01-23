#ifndef __CLIENT_NETWORK__
#define __CLIENT_NETWORK__

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

namespace Common
{
    class Packet;
}

namespace Client
{
    class Client;

    class Network :
        private boost::noncopyable
    {
        public:
            Network(Client& client);
            bool Connect(std::string const& host, std::string const& port);
            bool Tick();
            bool Disconnect();
            std::string const& GetError() const;
        private:
            void _ProcessPacket(Common::Packet& p);
            void _ReceivePacketSize();
            void _HandleReceivePacketSize(boost::system::error_code const& error, unsigned int);
            void _ReceivePacketContent(unsigned int size);
            void _HandleReceivePacketContent(boost::system::error_code const& error, unsigned int);
            boost::asio::io_service _ioService;
            boost::asio::ip::tcp::socket _socket;
            std::string _error;
            Client& _client;
            std::vector<char> _buffer;
    };
}

#endif
