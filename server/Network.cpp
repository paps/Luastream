#include <boost/bind.hpp>
#include "Network.hpp"
#include "Server.hpp"
#include "Client.hpp"

namespace Server
{
    Network::Network(Server& server, unsigned short port) :
        _server(server),
        _acceptor(server.GetIoService(), boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {
        this->_AsyncAccept();
    }

    void Network::_AsyncAccept()
    {
        boost::asio::ip::tcp::socket* socket = new boost::asio::ip::tcp::socket(this->_server.GetIoService());
        this->_acceptor.async_accept(*socket, boost::bind(&Network::_HandleAccept, this, socket, boost::asio::placeholders::error));
    }

    void Network::_HandleAccept(boost::asio::ip::tcp::socket* socket, const boost::system::error_code& error)
    {
        if (error)
        {
            std::cerr << "New client " << socket->remote_endpoint().address().to_string() << " not accepted: " << error.message() << std::endl;
            this->DeleteSocket(socket);
        }
        else
            this->_server.AddClient(new Client(this->_server, socket));
        this->_AsyncAccept();
    }

    void Network::DeleteSocket(boost::asio::ip::tcp::socket* socket)
    {
        socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        socket->close();
        delete socket;
    }
}
