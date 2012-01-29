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
    class FrameBuffer;

    class Client
    {
        public:
            Client(Server& server, boost::asio::ip::tcp::socket* socket);
            ~Client();
            void Tick();
            void Hello(Common::Packet& p);
            Connection& GetConnection();
            bool IsAccepted() const;
            FrameBuffer& GetFrameBuffer();
        private:
            Server& _server;
            bool _accepted;
            Connection* _connection;
            FrameBuffer* _frameBuffer;
    };
}

#endif
