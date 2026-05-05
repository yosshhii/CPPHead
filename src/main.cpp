#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

#include "engine/window.hpp"
#include "engine/draw.hpp"
#include "game/player.hpp"
#include "game/background.hpp"
#include "game/level.hpp"
#include "game/menu.hpp"
#include "game/settings.hpp"
#include "game/enemy.hpp"
#include "game/enemyManager.hpp"

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
            0,
            20.f,
            120.f,
            "assets/textures/Volume/Swiper/Swiper1.png",
            "assets/textures/Volume/Swiper/Swiper2.png",
            50.f
    );
    Settings pauseMenu(
            {
                    "assets/textures/Volume/Volume1.png",
                    "assets/textures/Main Menu/Main Menu1.png"
            },
            "assets/textures/menu.png",
            0,
            20.f,
            120.f,
            "assets/textures/Volume/Swiper/Swiper1.png",
            "assets/textures/Volume/Swiper/Swiper2.png",
            50.f
    );
    Menu gameOverMenu(
            {
                    "assets/textures/Restart/Restart1.png",
                    "assets/textures/Main Menu/Main Menu1.png"
            },
            "assets/textures/menu.png",
            1
    );
    EnemyManager enemyManager;


    sf::Clock clock;

    int state = 0;
    bool wasEnterPressed = false;
    bool wasEscPressed = false;

    bool isRunning = true;
    while (isRunning) {
        float dt = clock.restart().asSeconds();
        handleWindowEvents(window, isRunning);

        bool isEnterPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter);
        bool isEscPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape);

        // --- STATE 0: MENU ---
        if (state == 0) {
            if (isEnterPressed && !wasEnterPressed) {
                mainMenu.ButtonPicker();
                int selectedIndex = mainMenu.getSelectedIndex();

                if (selectedIndex == 0) {
                    state = 1;
                    level.init();

                    player.reset({0.f, 200.f});
                }
                if (selectedIndex == 1) state = 2;
                if (selectedIndex == 2) isRunning = false;
            }

            mainMenu.handleInput(window);
            background.update({1,0}, dt, window, state);
            drawMenu(window, background, level, mainMenu);
        }

        // --- STATE 1: GAME ---
        else if (state == 1) {
            if (isEscPressed && !wasEscPressed) {
                state = 3;
            }


            enemyManager.spawnSkeleton({800.f, 200.f});
            enemyManager.spawnSkeleton({100.f, 200.f});
            enemyManager.spawnSkeleton({300.f, 200.f});

            player.handleInput(window);
            sf::Vector2f movement = player.getMovement();

            background.update(movement, dt, window, state);
            player.update(dt);

            float bgOffset = background.getOffsetX();

            sf::Vector2f playerWorldPos = {
                player.getPosition().x - bgOffset,
                player.getPosition().y
            };
            enemyManager.update(dt, player, playerWorldPos);

            float windowWidth = (float)window.getSize().x;
            level.playerGroundCollision(player, dt, windowWidth, movement.x);

            if (player.isDead()) {
                state = 4;
            }

            drawGame(window, background, level, player, enemyManager, bgOffset);
        }
        // --- STATE 2: SETTINGS ---
        else if (state == 2) {
            settingsMenu.handleInput(window);
            background.update({1,0}, dt, window, state);
            drawSettings(window, background, level, settingsMenu);

            if (isEnterPressed && !wasEnterPressed) {
                settingsMenu.ButtonPicker();

                int selectedIndex = settingsMenu.getSelectedIndex();

                if (selectedIndex == 0) {}
                if (selectedIndex == 1) state = 0;
            }
        }
        // --- STATE 3: PAUSE ---
        else if (state == 3) {
            pauseMenu.handleInput(window);

            drawPauseMenu(window, background, level, player, pauseMenu);

            if (isEscPressed && !wasEscPressed) {
                state = 1;
            }

            if (isEnterPressed && !wasEnterPressed) {
                pauseMenu.ButtonPicker();
                int selectedIndex = pauseMenu.getSelectedIndex();

                if (selectedIndex == 0) state = 1;
                if (selectedIndex == 1) state = 0;
            }
        }
        // --- STATE 4: GAME_OVER ---
        else if (state == 4) {
            gameOverMenu.handleInput(window);

            drawGameOver(window, background, level, player, gameOverMenu);

            if (isEnterPressed && !wasEnterPressed) {
                gameOverMenu.ButtonPicker();
                int selectedIndex = gameOverMenu.getSelectedIndex();

                if (selectedIndex == 0) {
                    level.init();

                    player.reset({0.f, 200.f});

                    state = 1;
                }
                if (selectedIndex == 1) {
                    state = 0;
                }
            }
        }

        wasEnterPressed = isEnterPressed;
        wasEscPressed = isEscPressed;
    }
    return 0;
}