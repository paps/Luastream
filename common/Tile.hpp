#ifndef __COMMON_TILE__
#define __COMMON_TILE__

namespace Common
{
    namespace Tile
    {
        enum Tile
        {
            Empty = 0,
            DudeDown = 1,
            DudeUp = 2,
            DudeLeft = 3,
            DudeRight = 4,
        };
        enum
        {
            NbTileW = 72,
            NbTileH = 36,
        };
    }
}

#endif
