#include "FrameBuffer.hpp"
#include "Client.hpp"
#include "../common/Tile.hpp"
#include "../common/Packet.hpp"
#include "Connection.hpp"

namespace Server
{
    FrameBuffer::FrameBuffer(Client& client) :
        _client(client)
    {
        this->_oldFrame.resize(Common::Tile::NbTileW * Common::Tile::NbTileH);
        this->_newFrame.resize(Common::Tile::NbTileW * Common::Tile::NbTileH);
    }

    Tile& FrameBuffer::Get(unsigned int x, unsigned int y)
    {
        if (x < Common::Tile::NbTileW && y < Common::Tile::NbTileH)
            return this->_newFrame[this->_PosToIdx(x, y)];
        return this->_dummyTile;
    }

    void FrameBuffer::Sync()
    {
        this->_oldFrame = this->_newFrame;
        Common::Packet p;
        p.WriteByte(Common::Packet::Sync);
        std::vector<Tile>::const_iterator it = this->_oldFrame.begin();
        std::vector<Tile>::const_iterator itEnd = this->_oldFrame.end();
        for (; it != itEnd; ++it)
        {
            p.WriteByte(it->GetType());
            p.WriteByte(it->GetColors());
        }
        this->_client.GetConnection().SendPacket(p);
    }

    void FrameBuffer::Flip()
    {
        Common::Packet p;
        p.WriteByte(Common::Packet::Tile);
        for (unsigned int y = 0; y < Common::Tile::NbTileH; ++y)
            for (unsigned int x = 0; x < Common::Tile::NbTileW; ++x)
                if (this->_oldFrame[this->_PosToIdx(x, y)] != this->_newFrame[this->_PosToIdx(x, y)])
                {
                    Tile const& t = this->_newFrame[this->_PosToIdx(x, y)];
                    p.WriteByte(x);
                    p.WriteByte(y);
                    p.WriteByte(t.GetType());
                    p.WriteByte(t.GetColors());
                }
        if (p.GetSize() > 1)
            this->_client.GetConnection().SendPacket(p);
        this->_oldFrame = this->_newFrame;
    }

    unsigned int FrameBuffer::_PosToIdx(unsigned int x, unsigned int y) const
    {
        return y * Common::Tile::NbTileW + x;
    }
}
