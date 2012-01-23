#include <boost/bind.hpp>
#include "Network.hpp"
#include "Client.hpp"
#include "../common/Packet.hpp"

namespace Client
{
    Network::Network(Client& client) :
        _socket(_ioService), _client(client)
    {
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
        this->_ReceivePacketSize();
        return true;
    }

    bool Network::Tick()
    {
        if (!this->_error.size())
            return false;
        try
        {
            this->_ioService.poll();
            this->_ioService.reset();
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
        if (!this->_error.size())
            return false;
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

    void Network::_ProcessPacket(Common::Packet& p)
    {
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
