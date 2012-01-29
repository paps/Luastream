#ifndef __SERVER_TILE__
#define __SERVER_TILE__

#include "../common/Color.hpp"

namespace Server
{
    class Tile
    {
        public:
            Tile();
            bool operator ==(Tile const& t) const;
            bool operator !=(Tile const& t) const;
            unsigned char GetType() const;
            unsigned char GetColors() const;
            Common::Color::Color GetFgColor() const;
            Common::Color::Color GetBgColor() const;
            void SetType(unsigned char type);
            void SetFgColor(Common::Color::Color c);
            void SetBgColor(Common::Color::Color c);
        private:
            unsigned char _type;
            unsigned char _colors;
    };
}

#endif
