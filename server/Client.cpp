#include "Client.hpp"
#include "Server.hpp"
#include "Connection.hpp"
#include "FrameBuffer.hpp"
#include "../common/Packet.hpp"
#include "../common/Tile.hpp"

namespace Server
{
    Client::Client(Server& server, boost::asio::ip::tcp::socket* socket) :
        _server(server), _accepted(false)
    {
        this->_frameBuffer = new FrameBuffer(*this);
        this->_connection = new Connection(this->_server, *this, socket);
        std::cout << "New client " << this->_connection->GetName() << std::endl;
    }

    Client::~Client()
    {
        std::cout << "Client " << this->_connection->GetName() << " left" << std::endl;
        delete this->_connection;
        delete this->_frameBuffer;
    }

    void Client::Tick()
    {
        this->_frameBuffer->Flip();
    }

    FrameBuffer& Client::GetFrameBuffer()
    {
        return *this->_frameBuffer;
    }

    Connection& Client::GetConnection()
    {
        return *this->_connection;
    }

    bool Client::IsAccepted() const
    {
        return this->_accepted;
    }

    void Client::Hello(Common::Packet& p)
    {
        if (this->_accepted)
        {
            this->_server.DeleteClientLater(this);
            std::cerr << "Multiple Hello for client " << this->_connection->GetName() << std::endl;
            return;
        }
        int protocolVersion = p.ReadInt();
        unsigned char screenWidth = p.ReadByte();
        unsigned char screenHeight = p.ReadByte();
        if (protocolVersion != Common::Packet::ProtocolVersion || screenWidth != Common::Tile::NbTileW || screenHeight != Common::Tile::NbTileH)
        {
            this->_server.DeleteClientLater(this);
            std::cerr << "Client " << this->_connection->GetName() << " not accepted: "
                << "protocol version " << protocolVersion << ", screen width " << screenWidth << ", screen height " << screenHeight << " "
                << "(expected " << Common::Packet::ProtocolVersion << ", " << Common::Tile::NbTileW << ", " << Common::Tile::NbTileH << ")"
                << std::endl;
            return;
        }
        this->_accepted = true;
        this->_frameBuffer->Sync();
    }
}
