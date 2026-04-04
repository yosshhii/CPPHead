#include <iostream>
#include <SFML/Graphics.hpp>

#include "engine/window.hpp"
#include "game/player.hpp"
#include "game/background.hpp"
#include "game/level.hpp"

int main() {
    sf::RenderWindow window = createWindow();

    Player player;
    Background background;
    Level level;

    sf::Clock clock;

    bool isRunning = true;
    while (isRunning) {
        auto elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {isRunning = false;}
            if (event->is<sf::Event::Resized>()) {window.setView(sf::View{ {}, static_cast<sf::Vector2f>(window.getSize())});}
        }

        player.handleInput(window);
        sf::Vector2f movement = player.getMovement();

        background.update(movement, dt, window);
        player.update(dt);

        level.syncWithBackground(background);
        level.playerGroundCollision(player, background);

        window.clear();
        background.draw(window);
        level.draw(window);
        player.draw(window);
        window.display();
    }
    return 0;
}
