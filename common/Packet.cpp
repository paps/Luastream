#include <stdexcept>
#include <cstring>
#include <iomanip>
#include <iostream>
#ifdef _WIN32
#include <Winsock2.h>
#else
#include <arpa/inet.h>
#endif
#include "Packet.hpp"

namespace Common
{
    Packet::Packet() :
        _size(0), _allocSize(InitialSize), _offset(2)
    {
        this->_data = new char[InitialSize];
        this->_WriteSize();
    }

    Packet::~Packet()
    {
        delete [] this->_data;
    }

    unsigned int Packet::BytesLeft() const
    {
        return this->_size - this->_offset + 2;
    }

    void Packet::WriteString(std::string const& val)
    {
        this->WriteShort(val.size());
        for (unsigned int i = 0; i != val.size(); ++i)
            this->_WriteByte(val[i]);
    }

    void Packet::WriteByte(uint8_t val)
    {
        this->_WriteByte(static_cast<char>(val));
    }

    void Packet::WriteShort(uint16_t val)
    {
        val = htons(val);
        for (unsigned int i = 0; i < 2; ++i)
            this->_WriteByte(*(reinterpret_cast<char*>(&val) + i));
    }

    void Packet::WriteInt(uint32_t val)
    {
        val = htonl(val);
        for (unsigned int i = 0; i < 4; ++i)
            this->_WriteByte(*(reinterpret_cast<char*>(&val) + i));
    }

    void Packet::WriteFloat(float val)
    {
        for (unsigned int i = 0; i < 4; ++i)
            this->_WriteByte(*(reinterpret_cast<char*>(&val) + i));
    }

    void Packet::WriteDouble(double val)
    {
        for (unsigned int i = 0; i < 8; i++)
            this->_WriteByte(*(reinterpret_cast<char*>(&val) + i));
    }

    uint8_t Packet::ReadByte()
    {
        if (this->_offset < this->_size + 2)
            return this->_data[this->_offset++];
        throw std::runtime_error("packet too small to read");
    }

    uint16_t Packet::ReadShort()
    {
        if (this->_offset + 1 < this->_size + 2)
        {
            uint16_t const* ret = reinterpret_cast<uint16_t const*>(&this->_data[this->_offset]);
            this->_offset += 2;
            return ntohs(*ret);
        }
        throw std::runtime_error("packet too small to read");
    }

    uint32_t Packet::ReadInt()
    {
        if (this->_offset + 3 < this->_size + 2)
        {
            uint32_t const* ret = reinterpret_cast<uint32_t const*>(&this->_data[this->_offset]);
            this->_offset += 4;
            return ntohl(*ret);
        }
        throw std::runtime_error("packet too small to read");
    }

    float Packet::ReadFloat()
    {
        if (this->_offset + 3 < this->_size + 2)
        {
            float const* ret = reinterpret_cast<float const*>(&this->_data[this->_offset]);
            this->_offset += 4;
            return *ret;
        }
        throw std::runtime_error("packet too small to read");
    }

    double Packet::ReadDouble()
    {
        if (this->_offset + 7 < this->_size + 2)
        {
            double const* ret = reinterpret_cast<double const*>(&this->_data[this->_offset]);
            this->_offset += 8;
            return *ret;
        }
        throw std::runtime_error("packet too small to read");
    }

    std::string Packet::ReadString()
    {
        unsigned int size = this->ReadShort();
        std::string ret;
        for (unsigned int i = 0; i < size; ++i)
            ret += this->ReadByte();
        return ret;
    }

    void Packet::Clear()
    {
        this->_size = 0;
        this->_offset = 2;
        this->_WriteSize();
    }

    void Packet::WriteData(char* data, unsigned int size)
    {
        if (this->_allocSize < size + 2)
            this->_Resize(size + 2);
        ::memcpy(this->_data + 2, data, size);
        this->_size = size;
        this->_offset = 2;
        this->_WriteSize();
    }

    void Packet::_Resize(unsigned int target)
    {
        char* tmp = this->_data;
        this->_data = new char[target];
        ::memcpy(this->_data, tmp, this->_allocSize);
        this->_allocSize = target;
        delete [] tmp;
    }

    void Packet::_WriteByte(char byte)
    {
        if (this->_offset >= _allocSize)
            this->_Resize(_allocSize + SizeStep);
        this->_data[this->_offset] = byte;
        ++this->_offset;
        if (this->_offset - 2 > this->_size)
            this->_size = this->_offset - 2;
        this->_WriteSize();
    }

    void Packet::_WriteSize()
    {
        uint16_t size = htons(static_cast<uint16_t>(this->_size));
        this->_data[0] = *(reinterpret_cast<char*>(&size));
        this->_data[1] = *(reinterpret_cast<char*>(&size) + 1);
    }

    char const* Packet::GetData() const
    {
        return this->_data + 2;
    }

    char const* Packet::GetCompleteData() const
    {
        return this->_data;
    }

    unsigned int Packet::GetSize() const
    {
        return this->_size;
    }

    unsigned int Packet::GetCompleteSize() const
    {
        return this->_size + 2;
    }
}
