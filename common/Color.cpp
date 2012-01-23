#include "Color.hpp"

namespace Common
{
    namespace Color
    {
        Color ReadBgColor(unsigned char colors)
        {
            return static_cast<Color>((colors >> 4) & 0x0f);
        }

        Color ReadFgColor(unsigned char colors)
        {
            return static_cast<Color>(colors & 0x0f);
        }

        void WriteBgColor(unsigned char& colors, Color c)
        {
            colors &= ~(0x0f << 4);
            colors |= c << 4;
        }

        void WriteFgColor(unsigned char& colors, Color c)
        {
            colors &= ~0x0f;
            colors |= c;
        }
    }
}
