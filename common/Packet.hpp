#ifndef __COMMON_PACKET__
#define __COMMON_PACKET__

#include <string>
#include <boost/noncopyable.hpp>
#include <cstdint>

/*
 * Protocol:
 *
 * - Hello (C -> S)
 *   - uchar 0
 *   - int protocol version
 *   - uchar screen width
 *   - uchar screen height
 *
 * - Tile (S -> C)
 *   - uchar 1
 *   - repeated:
 *     - uchar x
 *     - uchar y
 *     - uchar type
 *     - uchar colors
 *
 * - Sync (S -> C)
 *   - uchar 2
 *   - repeated (y = i/NbTileW, x = i-y*NbTileW):
 *     - uchar type
 *     - uchar colors
 */

namespace Common
{
    class Packet :
        private boost::noncopyable
    {
        public:
            enum
            {
                ProtocolVersion = 1,
            };
            enum Type
            {
                Hello = 0,
                Tile = 1,
                Sync = 2,
            };
            Packet();
            ~Packet();
            unsigned int BytesLeft() const;
            void Clear();
            void WriteData(char* data, unsigned int size);
            void WriteByte(uint8_t val);
            void WriteShort(uint16_t val);
            void WriteInt(uint32_t val);
            void WriteFloat(float val);
            void WriteDouble(double val);
            void WriteString(std::string const& val);
            uint8_t ReadByte();
            uint16_t ReadShort();
            uint32_t ReadInt();
            float ReadFloat();
            double ReadDouble();
            std::string ReadString();
            unsigned int GetSize() const;
            unsigned int GetCompleteSize() const;
            char const* GetData() const;
            char const* GetCompleteData() const;
        private:
            enum
            {
                InitialSize = 4096,
                SizeStep = 4096,
            };
            void _WriteSize();
            void _WriteByte(char byte);
            void _Resize(unsigned int target);
            char* _data;
            unsigned int _size;
            unsigned int _allocSize;
            unsigned int _offset;
    };
}

#endif
