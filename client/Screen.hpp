#ifndef __CLIENT_SCREEN__
#define __CLIENT_SCREEN__

#include <SFML/Graphics.hpp>
#include <vector>
#include <boost/noncopyable.hpp>
#include "common/Color.hpp"

namespace Common
{
    class Packet;
}

namespace Client
{
    class Client;

    class Screen :
        private boost::noncopyable
    {
        public:
            Screen(Client& client);
            ~Screen();
            void DrawTile(unsigned int x, unsigned int y, unsigned char tile, Common::Color::Color fg, Common::Color::Color bg);
            void DrawText(unsigned int line, std::string const& text, Common::Color::Color fg, Common::Color::Color bg);
            void ProcessTilePacket(Common::Packet& p);
        private:
            void _PopulateColors();
            void _PopulateBgShapes();
            void _AddBgShape(Common::Color::Color color);
            Client& _client;
            sf::IntRect* _rect;
            sf::Sprite* _sprite;
            std::vector<sf::Color> _colors;
            std::vector<sf::Shape> _bgShapes;
    };
}

#endif
