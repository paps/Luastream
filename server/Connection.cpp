#include <boost/bind.hpp>
#include "Connection.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Network.hpp"
#include "../common/Packet.hpp"

namespace Server
{
    Connection::Connection(Server& server, Client& client, boost::asio::ip::tcp::socket* socket) :
        _server(server), _client(client), _socket(socket), _sendingData(false), _sendQueueSize(0)
    {
        this->_ReceivePacketSize();
    }

    Connection::~Connection()
    {
        this->_server.GetNetwork().DeleteSocket(this->_socket);
    }

    std::string Connection::GetName() const
    {
        return this->_socket->remote_endpoint().address().to_string();
    }

    unsigned int Connection::GetSendQueueSize() const
    {
        return this->_sendQueueSize;
    }

    bool Connection::SendPacket(Common::Packet const& p)
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
                std::cerr << "Client " << this->GetName() << " failed on packet sending: " << e.what() << std::endl;
                this->_server.DeleteClientLater(&this->_client);
                return false;
            }
        }
        return true;
    }

    void Connection::_SendNextPacket()
    {
        std::vector<boost::asio::const_buffer> buffers;
        buffers.push_back(boost::asio::const_buffer(this->_sendQueue.front().first, this->_sendQueue.front().second));
        boost::asio::async_write(*this->_socket, buffers, boost::bind(&Connection::_HandleWrite, this, boost::asio::placeholders::error));
    }

    void Connection::_HandleWrite(boost::system::error_code const& error)
    {
        this->_sendQueueSize -= this->_sendQueue.front().second;
        delete this->_sendQueue.front().first;
        this->_sendQueue.pop();
        if (error)
        {
            std::cerr << "Client " << this->GetName() << " failed on packet sending: " << error.message() << std::endl;
            this->_server.DeleteClientLater(&this->_client);
        }
        else if (this->_sendQueue.empty())
            this->_sendingData = false;
        else
            this->_SendNextPacket();
    }

    void Connection::_ProcessPacket(Common::Packet& p)
    {
        try
        {
            Common::Packet::Type type = static_cast<Common::Packet::Type>(p.ReadByte());
            switch (type)
            {
                case Common::Packet::Hello:
                    this->_client.Hello(p);
                default:
                    throw std::runtime_error("unknown packet type");
            }
        }
        catch (std::exception& e)
        {
            std::cerr << "Client " << this->GetName() << " failed on packet reception: " << e.what() << std::endl;
            this->_server.DeleteClientLater(&this->_client);
        }
    }

    void Connection::_ReceivePacketSize()
    {
        this->_buffer.resize(2);
        boost::asio::async_read(*this->_socket, boost::asio::buffer(this->_buffer), boost::bind(&Connection::_HandleReceivePacketSize, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    void Connection::_HandleReceivePacketSize(boost::system::error_code const& error, unsigned int)
    {
        if (error)
        {
            std::cerr << "Client " << this->GetName() << " failed on packet reception: " << error.message() << std::endl;
            this->_server.DeleteClientLater(&this->_client);
        }
        else
            this->_ReceivePacketContent(ntohs(*reinterpret_cast<uint16_t*>(&this->_buffer[0])));
    }

    void Connection::_ReceivePacketContent(unsigned int size)
    {
        this->_buffer.resize(size);
        boost::asio::async_read(*this->_socket, boost::asio::buffer(this->_buffer), boost::bind(&Connection::_HandleReceivePacketContent, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    void Connection::_HandleReceivePacketContent(boost::system::error_code const& error, unsigned int)
    {
        if (error)
        {
            std::cerr << "Client " << this->GetName() << " failed on packet reception: " << error.message() << std::endl;
            this->_server.DeleteClientLater(&this->_client);
        }
        else
        {
            Common::Packet p;
            p.WriteData(&this->_buffer[0], this->_buffer.size());
            this->_ProcessPacket(p);
            this->_ReceivePacketSize();
        }
    }
}
