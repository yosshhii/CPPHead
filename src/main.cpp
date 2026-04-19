#include <iostream>
#include <SFML/Graphics.hpp>

#include "engine/window.hpp"
#include "engine/draw.hpp"
#include "game/player.hpp"
#include "game/background.hpp"
#include "game/level.hpp"
#include "game/menu.hpp"
#include "game/settings.hpp"

int main() {
    sf::RenderWindow window = createWindow();

    Background background;
    Level level;
    Player player;
    Menu mainMenu(
            {
                    "assets/textures/StartButton/Start1.png",
                    "assets/textures/SettingsButton/Settings1.png",
                    "assets/textures/QuitButton/Quit1.png"
            },
            "assets/textures/menu.png",
            1
    );
    Settings settingsMenu(
            {
                    "assets/textures/Volume/Volume1.png",
                    "assets/textures/Main Menu/Main Menu1.png"
            },
            "assets/textures/menu.png",
            0
    );

    sf::Clock clock;

    int state = 0;
    bool wasEnterPressed = false;

    bool isRunning = true;
    while (isRunning) {
        float dt = clock.restart().asSeconds();
        handleWindowEvents(window, isRunning);

        bool isEnterPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter);

        if (state == 0) {
            mainMenu.handleInput(window);
            background.update({1,0}, dt, window, state);
            drawMenu(window, background, level, mainMenu);

            if (isEnterPressed && !wasEnterPressed) {
                mainMenu.ButtonPicker();

                int selectedIndex = mainMenu.getSelectedIndex();

                if (selectedIndex == 0) state = 1;
                if (selectedIndex == 1) state = 2;
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
        else if (state == 2) {
            settingsMenu.handleInput(window);
            background.update({1,0}, dt, window, state);
            drawSettings(window, background, level, settingsMenu);

            if (isEnterPressed && !wasEnterPressed) {
                settingsMenu.ButtonPicker();

                int selectedIndex = settingsMenu.getSelectedIndex();

                if (selectedIndex == 0) ;
                if (selectedIndex == 1) state = 0;
            }
        }
        wasEnterPressed = isEnterPressed;
    }
    return 0;
}