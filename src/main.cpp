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
    Menu mainMenu(
            {
                    "assets/textures/StartButton/Start1.png",
                    "assets/textures/SettingsButton/Settings1.png",
                    "assets/textures/QuitButton/Quit1.png"
            },
            "assets/textures/menu.png"
    );
    Player player;

    sf::Clock clock;

    int state = 0;

    bool isRunning = true;
    while (isRunning) {
        float dt = clock.restart().asSeconds();
        handleWindowEvents(window, isRunning);

        if (state == 0) {
            bool enterPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter);

            mainMenu.handleInput(window);
            background.update({1,0}, dt, window, state);
            drawMenu(window, background, level, mainMenu);

            if (enterPressed) {
                mainMenu.ButtonPicker();

                int selectedIndex = mainMenu.getSelectedIndex();

                if (selectedIndex == 0) state = 1;
                if (selectedIndex == 2) isRunning = false;
            }
        }
        else if (state == 1) {
            player.handleInput(window);
            sf::Vector2f movement = player.getMovement();

            background.update(movement, dt, window, state);
            player.update(dt);

            level.syncWithBackground(background);
            level.playerGroundCollision(player, background);

            drawGame(window, background, level, player);
        }
    }
    return 0;
}