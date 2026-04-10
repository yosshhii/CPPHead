#include <iostream>
#include <SFML/Graphics.hpp>

#include "engine/window.hpp"
#include "engine/draw.hpp"
#include "game/player.hpp"
#include "game/background.hpp"
#include "game/level.hpp"
#include "game/menu.hpp"

int main() {
    sf::RenderWindow window = createWindow();

    int state = 0;

    if (state == 0) {
        Background background;
        Level level;
        Menu menu;
        bool isRunning = true;
        while (isRunning) {
            handleWindowEvents(window, isRunning);
            drawMenu(window, background, level, menu);

            menu.handleInput(window);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter)) {
                int selectedIndex = menu.getSelectedIndex();

                if (selectedIndex == 0) {
                    state = 1;
                    isRunning = false;
                };
                if (selectedIndex == 2) isRunning = false;
            }
        }
    }

    if (state == 1) {
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

            drawGame(window, background, level, player);
        }
    }
    return 0;
}