#include "Screen.hpp"
#include "Client.hpp"
#include "common/Tile.hpp"

namespace Client
{
    Screen::Screen(Client& client) :
        _client(client)
    {
        this->_PopulateColors();
        this->_PopulateBgShapes();
        this->_rect = new sf::IntRect;
        this->_sprite = new sf::Sprite(client.GetTexture());
        this->_sprite->SetScale(this->_client.GetZoom(), this->_client.GetZoom());
    }

    Screen::~Screen()
    {
        delete this->_sprite;
        delete this->_rect;
    }

    void Screen::DrawText(unsigned int line, std::string const& text, Common::Color::Color fg, Common::Color::Color bg)
    {
        unsigned int x = 0;
        std::string::const_iterator it = text.begin();
        std::string::const_iterator itEnd = text.end();
        for (; it != itEnd; ++it)
        {
            this->DrawTile(x, line, *it, fg, bg);
            ++x;
        }
    }

    void Screen::DrawTile(unsigned int x, unsigned int y, unsigned char type, Common::Color::Color fg, Common::Color::Color bg)
    {
        if (x >= Common::Tile::NbTileW || y >= Common::Tile::NbTileH)
            return;
        if (static_cast<unsigned int>(bg) >= Common::Color::NbColors || static_cast<unsigned int>(fg) >= Common::Color::NbColors)
            return;
        this->_rect->Top = (type / Client::TilesPerLine) * this->_client.GetTileH();
        this->_rect->Bottom = this->_rect->Top + this->_client.GetTileH();
        this->_rect->Left = (type - (type / Client::TilesPerLine) * Client::TilesPerLine) * this->_client.GetTileW();
        this->_rect->Right = this->_rect->Left + this->_client.GetTileW();
        this->_sprite->SetSubRect(*this->_rect);
        this->_sprite->SetX(x * this->_client.GetTileW() * this->_client.GetZoom());
        this->_sprite->SetY(y * this->_client.GetTileH() * this->_client.GetZoom());
        this->_bgShapes[bg].SetPosition(this->_sprite->GetPosition().x + 1, this->_sprite->GetPosition().y + 1);
        this->_client.Draw(this->_bgShapes[bg]);
        if (type != 0 && type != ' ')
        {
            this->_sprite->SetColor(this->_colors[fg]);
            this->_client.Draw(*this->_sprite);
        }
    }

    void Screen::_PopulateColors()
    {
        this->_colors.resize(Common::Color::NbColors);
        //            Color name                              R    G    B
        this->_colors[Common::Color::Black] =       sf::Color(0,   0,   0  );
        this->_colors[Common::Color::DarkGrey] =    sf::Color(104, 104, 104);
        this->_colors[Common::Color::LightGrey] =   sf::Color(183, 183, 183);
        this->_colors[Common::Color::DarkBlue] =    sf::Color(0,   0,   145);
        this->_colors[Common::Color::LightBlue] =   sf::Color(0,   0,   255);
        this->_colors[Common::Color::DarkGreen] =   sf::Color(0,   145, 0  );
        this->_colors[Common::Color::LightGreen] =  sf::Color(0,   255, 0  );
        this->_colors[Common::Color::DarkTeal] =    sf::Color(0,   145, 145);
        this->_colors[Common::Color::LightTeal] =   sf::Color(0,   255, 255);
        this->_colors[Common::Color::DarkRed] =     sf::Color(145, 0,   0  );
        this->_colors[Common::Color::LightRed] =    sf::Color(255, 0,   0  );
        this->_colors[Common::Color::DarkPink] =    sf::Color(145, 0,   145);
        this->_colors[Common::Color::LightPink] =   sf::Color(255, 0,   255);
        this->_colors[Common::Color::DarkYellow] =  sf::Color(145, 145, 0  );
        this->_colors[Common::Color::LightYellow] = sf::Color(255, 255, 0  );
        this->_colors[Common::Color::White] =       sf::Color(255, 255, 255);
    }

    void Screen::_PopulateBgShapes()
    {
        this->_bgShapes.resize(Common::Color::NbColors);
        this->_AddBgShape(Common::Color::Black);
        this->_AddBgShape(Common::Color::DarkGrey);
        this->_AddBgShape(Common::Color::LightGrey);
        this->_AddBgShape(Common::Color::DarkBlue);
        this->_AddBgShape(Common::Color::LightBlue);
        this->_AddBgShape(Common::Color::DarkGreen);
        this->_AddBgShape(Common::Color::LightGreen);
        this->_AddBgShape(Common::Color::DarkTeal);
        this->_AddBgShape(Common::Color::LightTeal);
        this->_AddBgShape(Common::Color::DarkRed);
        this->_AddBgShape(Common::Color::LightRed);
        this->_AddBgShape(Common::Color::DarkPink);
        this->_AddBgShape(Common::Color::LightPink);
        this->_AddBgShape(Common::Color::DarkYellow);
        this->_AddBgShape(Common::Color::LightYellow);
        this->_AddBgShape(Common::Color::White);
    }

    void Screen::_AddBgShape(Common::Color::Color color)
    {
        this->_bgShapes[color] = sf::Shape::Rectangle(0, 0, this->_client.GetTileW(), this->_client.GetTileH(), this->_colors[color]);
        this->_bgShapes[color].SetScale(this->_client.GetZoom(), this->_client.GetZoom());
        this->_bgShapes[color].EnableOutline(false);
    }
}
