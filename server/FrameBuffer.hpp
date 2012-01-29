#ifndef __SERVER_FRAMEBUFFER__
#define __SERVER_FRAMEBUFFER__

#include <boost/noncopyable.hpp>
#include <vector>
#include "Tile.hpp"

namespace Server
{
    class Client;

    class FrameBuffer :
        private boost::noncopyable
    {
        public:
            FrameBuffer(Client& client);
            Tile& Get(unsigned int x, unsigned int y);
            void Sync();
            void Flip();
        private:
            unsigned int _PosToIdx(unsigned int x, unsigned int y) const;
            Client& _client;
            std::vector<Tile> _oldFrame;
            std::vector<Tile> _newFrame;
            Tile _dummyTile;
    };
}

#endif
