#include "window.hpp"
#include <SFMl/Graphics.hpp>

sf::RenderWindow createWindow() {
    sf::RenderWindow window(sf::VideoMode({1080,1080}), "CPPHead");
    window.setFramerateLimit(60);
    window.setView(sf::View{ {}, static_cast<sf::Vector2f>(window.getSize())});
    return window;
}
