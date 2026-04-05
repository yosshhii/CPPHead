#include <iostream>
#include <SFML/Graphics.hpp>

#include "engine/window.hpp"
#include "engine/draw.hpp"
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
        float dt = clock.restart().asSeconds();

        handleWindowEvents(window, isRunning);

        player.handleInput(window);
        sf::Vector2f movement = player.getMovement();

        background.update(movement, dt, window);
        player.update(dt);

        level.syncWithBackground(background);
        level.playerGroundCollision(player, background);

        draw(window, background, level, player);
    }
    return 0;
}