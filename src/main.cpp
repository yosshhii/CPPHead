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

    Background background;
    Level level;
    Menu menu;
    Player player;

    sf::Clock clock;

    int state = 0;

    bool isRunning = true;
    while (isRunning) {
        float dt = clock.restart().asSeconds();
        handleWindowEvents(window, isRunning);

        if (state == 0) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter)) {
                menu.ButtonPicker();
                int selectedIndex = menu.getSelectedIndex();

                if (selectedIndex == 0) {
                    level.init();
                    state = 1;
                }
                if (selectedIndex == 2) isRunning = false;
            }

            menu.handleInput(window);
            background.update({1,0}, dt, window, state);
            drawMenu(window, background, level, menu);
        }
        else if (state == 1) {
            player.handleInput(window);
            sf::Vector2f movement = player.getMovement();

            background.update(movement, dt, window, state);
            player.update(dt);

            float windowWidth = (float)window.getSize().x;

            level.playerGroundCollision(player, dt, windowWidth, movement.x);

            drawGame(window, background, level, player);
        }
    }
    return 0;
}