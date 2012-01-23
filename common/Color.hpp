#ifndef __COMMON_COLOR__
#define __COMMON_COLOR__

namespace Common
{
    namespace Color
    {
        enum Color
        {
            Black = 0,
            DarkGrey = 1,
            LightGrey = 2,
            DarkBlue = 3,
            LightBlue = 4,
            DarkGreen = 5,
            LightGreen = 6,
            DarkTeal = 7,
            LightTeal = 8,
            DarkRed = 9,
            LightRed = 10,
            DarkPink = 11,
            LightPink = 12,
            DarkYellow = 13,
            LightYellow = 14,
            White = 15,
        };
        enum
        {
            NbColors = 16,
        };
        Color ReadBgColor(unsigned char colors);
        Color ReadFgColor(unsigned char colors);
        void WriteBgColor(unsigned char& colors, Color c);
        void WriteFgColor(unsigned char& colors, Color c);
    }
}

#endif
