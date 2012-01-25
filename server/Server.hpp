#ifndef __SERVER_SERVER__
#define __SERVER_SERVER__

namespace Server
{
    class Network;

    class Server
    {
        public:
            int Run(int ac, char** av);
            Network& GetNetwork();
        private:
            bool _Init(int ac, char** av);
            void _Deinit();
            Network* _network;
    };
}

#endif
