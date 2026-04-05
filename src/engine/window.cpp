#include "window.hpp"

sf::RenderWindow createWindow() {
    sf::RenderWindow window(sf::VideoMode({1080,1080}), "CPPHead");
    window.setFramerateLimit(60);
    window.setView(sf::View{ {}, static_cast<sf::Vector2f>(window.getSize())});
    return window;
}

void handleWindowEvents(sf::RenderWindow& window, bool& isRunning) {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            isRunning = false;
        }
        if (event->is<sf::Event::Resized>()) {
            window.setView(sf::View{ {}, static_cast<sf::Vector2f>(window.getSize())});
        }
    }
}