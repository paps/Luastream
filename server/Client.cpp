#include "Client.hpp"
#include "Server.hpp"
#include "Connection.hpp"
#include "../common/Packet.hpp"

namespace Server
{
    Client::Client(Server& server, boost::asio::ip::tcp::socket* socket) :
        _server(server)
    {
        this->_connection = new Connection(this->_server, *this, socket);
    }

    Client::~Client()
    {
        delete this->_connection;
    }

    void Client::Hello(Common::Packet& p)
    {
        int protocolVersion = p.ReadInt();
        unsigned char screenWidth = p.ReadByte();
        unsigned char screenHeight = p.ReadByte();
    }
}
