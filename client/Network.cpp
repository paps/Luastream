#include <boost/bind.hpp>
#include "Network.hpp"
#include "Client.hpp"
#include "Screen.hpp"
#include "../common/Packet.hpp"
#include "../common/Tile.hpp"

namespace Client
{
    Network::Network(Client& client) :
        _socket(_ioService), _client(client), _sendingData(false), _sendQueueSize(0)
    {
    }

    Network::~Network()
    {
        while (!this->_sendQueue.empty())
        {
            delete this->_sendQueue.front().first;
            this->_sendQueue.pop();
        }
    }

    unsigned int Network::GetSendQueueSize() const
    {
        return this->_sendQueueSize;
    }

    bool Network::Connect(std::string const& host, std::string const& port)
    {
        try
        {
            boost::asio::ip::tcp::resolver resolver(this->_ioService);
            boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), host, port);
            boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
            this->_socket.connect(*iterator);
        }
        catch (std::exception& e)
        {
            this->_error = e.what();
            return false;
        }
        this->_SayHello();
        this->_ReceivePacketSize();
        return true;
    }

    bool Network::Tick()
    {
        if (this->_error.size())
            return false;
        try
        {
            this->_ioService.poll();
            this->_ioService.reset();
            std::cout << "tick!" << std::endl;
        }
        catch (std::exception& e)
        {
            this->_error = e.what();
            return false;
        }
        return true;
    }

    bool Network::Disconnect()
    {
        try
        {
            this->_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            this->_socket.close();
        }
        catch (std::exception& e)
        {
            this->_error = e.what();
            return false;
        }
        return true;
    }

    std::string const& Network::GetError() const
    {
        return this->_error;
    }

    bool Network::SendPacket(Common::Packet const& p)
    {
        char* copy = new char[p.GetCompleteSize()];
        memcpy(copy, p.GetCompleteData(), p.GetCompleteSize());
        this->_sendQueue.push(std::make_pair(copy, p.GetCompleteSize()));
        this->_sendQueueSize += p.GetCompleteSize();
        if (!this->_sendingData)
        {
            try
            {
                this->_sendingData = true;
                this->_SendNextPacket();
            }
            catch (std::exception& e)
            {
                this->_error = e.what();
                return false;
            }
        }
        return true;
    }

    void Network::_SayHello()
    {
        Common::Packet p;
        p.WriteInt(Common::Packet::ProtocolVersion);
        p.WriteByte(Common::Tile::NbTileW);
        p.WriteByte(Common::Tile::NbTileH);
        this->SendPacket(p);
    }

    void Network::_SendNextPacket()
    {
        std::vector<boost::asio::const_buffer> buffers;
        buffers.push_back(boost::asio::const_buffer(this->_sendQueue.front().first, this->_sendQueue.front().second));
        boost::asio::async_write(this->_socket, buffers, boost::bind(&Network::_HandleWrite, this, boost::asio::placeholders::error));
    }

    void Network::_HandleWrite(boost::system::error_code const& error)
    {
        this->_sendQueueSize -= this->_sendQueue.front().second;
        delete this->_sendQueue.front().first;
        this->_sendQueue.pop();
        if (error)
            this->_error = error.message();
        else if (this->_sendQueue.empty())
            this->_sendingData = false;
        else
            this->_SendNextPacket();
    }

    void Network::_ProcessPacket(Common::Packet& p)
    {
        try
        {
            Common::Packet::Type type = static_cast<Common::Packet::Type>(p.ReadByte());
            switch (type)
            {
                case Common::Packet::Tile:
                    this->_client.GetScreen().ProcessTilePacket(p);
                default:
                    throw std::runtime_error("unknown packet type");
            }
        }
        catch (std::exception& e)
        {
            this->_error = "Fatal exception on a received packet: \"" + std::string(e.what()) + "\".";
        }
    }

    void Network::_ReceivePacketSize()
    {
        this->_buffer.resize(2);
        boost::asio::async_read(this->_socket, boost::asio::buffer(this->_buffer), boost::bind(&Network::_HandleReceivePacketSize, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    void Network::_HandleReceivePacketSize(boost::system::error_code const& error, unsigned int)
    {
        if (error)
            this->_error = error.message();
        else
            this->_ReceivePacketContent(ntohs(*reinterpret_cast<uint16_t*>(&this->_buffer[0])));
    }

    void Network::_ReceivePacketContent(unsigned int size)
    {
        this->_buffer.resize(size);
        boost::asio::async_read(this->_socket, boost::asio::buffer(this->_buffer), boost::bind(&Network::_HandleReceivePacketContent, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    void Network::_HandleReceivePacketContent(boost::system::error_code const& error, unsigned int)
    {
        if (error)
            this->_error = error.message();
        else
        {
            Common::Packet p;
            p.WriteData(&this->_buffer[0], this->_buffer.size());
            this->_ProcessPacket(p);
            this->_ReceivePacketSize();
        }
    }
}
