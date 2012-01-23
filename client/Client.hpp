#ifndef __CLIENT_CLIENT__
#define __CLIENT_CLIENT__

#include <SFML/Graphics.hpp>
#include <boost/noncopyable.hpp>

namespace Client
{
    class Screen;
    class Network;

    class Client :
        private boost::noncopyable
    {
        public:
            enum
            {
                TilesPerLine = 16,
                Framerate = 20,
            };
            Client();
            int Run(int ac, char** av);
            void Draw(sf::Drawable const& drawable);
            sf::Image const& GetTexture() const;
            unsigned int GetTileW() const;
            unsigned int GetTileH() const;
            float GetZoom() const;
        private:
            bool _Init(int ac, char** av);
            void _Deinit();
            void _ProcessEvents();
            void _Error(std::string const& e);
            void _Usage(std::string const& program);
            sf::RenderWindow* _renderWindow;
            sf::Image* _texture;
            unsigned int _tileW;
            unsigned int _tileH;
            float _zoom;
            Screen* _screen;
            Network* _network;
            bool _error;
    };
}

#endif
