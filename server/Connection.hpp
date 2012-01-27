#ifndef __SERVER_CONNECTION__
#define __SERVER_CONNECTION__

#include <boost/asio.hpp>
#include <queue>

namespace Common
{
    class Packet;
}

namespace Server
{
    class Server;
    class Client;

    class Connection
    {
        public:
            Connection(Server& server, Client& client, boost::asio::ip::tcp::socket* socket);
            ~Connection();
            unsigned int GetSendQueueSize() const;
            bool SendPacket(Common::Packet const& p);
            std::string GetName() const;
        private:
            void _SendNextPacket();
            void _HandleWrite(boost::system::error_code const& error);
            void _ProcessPacket(Common::Packet& p);
            void _ReceivePacketSize();
            void _HandleReceivePacketSize(boost::system::error_code const& error, unsigned int);
            void _ReceivePacketContent(unsigned int size);
            void _HandleReceivePacketContent(boost::system::error_code const& error, unsigned int);
            Server& _server;
            Client& _client;
            boost::asio::ip::tcp::socket* _socket;
            std::vector<char> _buffer;
            std::queue< std::pair<char const*, unsigned int> > _sendQueue;
            bool _sendingData;
            unsigned int _sendQueueSize;
    };
}

#endif
