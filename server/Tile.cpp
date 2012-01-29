#include "Tile.hpp"

namespace Server
{
    Tile::Tile() :
        _type(0), _colors(0)
    {
    }

    bool Tile::operator ==(Tile const& t) const
    {
        return this->_type == t.GetType() && this->_colors == t.GetColors();
    }

    bool Tile::operator !=(Tile const& t) const
    {
        return !(*this == t);
    }

    unsigned char Tile::GetType() const
    {
        return this->_type;
    }

    unsigned char Tile::GetColors() const
    {
        return this->_colors;
    }

    Common::Color::Color Tile::GetFgColor() const
    {
        return Common::Color::ReadFgColor(this->_colors);
    }

    Common::Color::Color Tile::GetBgColor() const
    {
        return Common::Color::ReadBgColor(this->_colors);
    }

    void Tile::SetType(unsigned char type)
    {
        this->_type = type;
    }

    void Tile::SetFgColor(Common::Color::Color c)
    {
        Common::Color::WriteFgColor(this->_colors, c);
    }

    void Tile::SetBgColor(Common::Color::Color c)
    {
        Common::Color::WriteBgColor(this->_colors, c);
    }
}
