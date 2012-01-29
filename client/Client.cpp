#include <boost/cstdlib.hpp>
#include <iostream>
#include "Client.hpp"
#include "Screen.hpp"
#include "Network.hpp"
#include "common/Tile.hpp"

namespace Client
{
    Client::Client() :
        _error(false)
    {
    }

    int Client::Run(int ac, char** av)
    {
        if (!this->_Init(ac, av))
            return boost::exit_failure;
        while (this->_renderWindow->IsOpened())
        {
            this->_ProcessEvents();
            if (!this->_error && !this->_network->Tick())
            {
                this->_error = true;
                this->_Error(this->_network->GetError());
            }
            this->_renderWindow->Display();
        }
        this->_network->Disconnect();
        this->_Deinit();
        return boost::exit_success;
    }

    Screen& Client::GetScreen()
    {
        return *this->_screen;
    }

    void Client::_ProcessEvents()
    {
        sf::Event e;
        while (this->_renderWindow->GetEvent(e))
        {
            if (e.Type == sf::Event::Closed)
                this->_renderWindow->Close();
        }
    }

    void Client::_Error(std::string const& e)
    {
        this->_screen->DrawText(0, "Error:", Common::Color::LightRed, Common::Color::DarkGrey);
        this->_screen->DrawText(1, e, Common::Color::LightRed, Common::Color::DarkGrey);
    }

    void Client::_Usage(std::string const& program)
    {
        this->_screen->DrawText(0, "Incorrect parameters.", Common::Color::LightRed, Common::Color::DarkGrey);
        this->_screen->DrawText(2, "Usage:", Common::Color::LightRed, Common::Color::DarkGrey);
        this->_screen->DrawText(3, program + " host port [zoom]", Common::Color::LightRed, Common::Color::DarkGrey);
        this->_screen->DrawText(5, "~~~", Common::Color::LightRed, Common::Color::DarkGrey);
        this->_screen->DrawText(7, "- host: address of server", Common::Color::LightRed, Common::Color::DarkGrey);
        this->_screen->DrawText(8, "- port: port of server", Common::Color::LightRed, Common::Color::DarkGrey);
        this->_screen->DrawText(9, "- zoom: optional zoom factor (0.1 - 10, default 2)", Common::Color::LightRed, Common::Color::DarkGrey);
    }

    bool Client::_Init(int ac, char** av)
    {
        this->_texture = new sf::Image();
        if (!this->_texture->LoadFromFile("client/texture.png"))
        {
            std::cerr << "Failed to load texture." << std::endl;
            return false;
        }
        this->_texture->SetSmooth(false);
        this->_tileW = this->_texture->GetWidth() / TilesPerLine;
        this->_tileH = this->_texture->GetHeight() / TilesPerLine;
        if (this->_tileW < 1 || this->_tileH < 1)
        {
            std::cerr << "Invalid texture format." << std::endl;
            delete this->_texture;
            return false;
        }
        this->_zoom = 2;
        if (ac >= 4)
            this->_zoom = atof(av[3]) >= 0.1 && atof(av[3]) <= 10 ? atof(av[3]) : 2;
        sf::VideoMode m(Common::Tile::NbTileW * this->_tileW * this->_zoom, Common::Tile::NbTileH * this->_tileH * this->_zoom);
        this->_renderWindow = new sf::RenderWindow(m, "luastream", sf::Style::Close);
        this->_renderWindow->SetFramerateLimit(Framerate);
        this->_renderWindow->Clear();
        this->_screen = new Screen(*this);
        this->_network = new Network(*this);
        if (ac >= 3)
        {
            if (!this->_network->Connect(av[1], av[2]))
            {
                this->_error = true;
                this->_Error(this->_network->GetError());
            }
        }
        else
        {
            this->_error = true;
            this->_Usage(av[0]);
        }
        return true;
    }

    void Client::_Deinit()
    {
        delete this->_network;
        delete this->_screen;
        delete this->_renderWindow;
        delete this->_texture;
    }

    void Client::Draw(sf::Drawable const& drawable)
    {
        this->_renderWindow->Draw(drawable);
    }

    sf::Image const& Client::GetTexture() const
    {
        return *this->_texture;
    }

    unsigned int Client::GetTileW() const
    {
        return this->_tileW;
    }

    unsigned int Client::GetTileH() const
    {
        return this->_tileH;
    }

    float Client::GetZoom() const
    {
        return this->_zoom;
    }
}
