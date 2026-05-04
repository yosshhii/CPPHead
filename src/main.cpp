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

    sf::Clock clock;

    int state = 0;
    bool wasEnterPressed = false;
    bool wasEscPressed = false;


    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile("assets/textures/SkeletonEnemy/Idle/Sword/Skeleton_Default_Idle_Sword.png")) {
        std::cerr << "[ERROR] Failed to load enemy texture!" << std::endl;
    }

    std::vector<Enemy> enemies;

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

            if (enemies.empty()) {
                enemies.emplace_back(enemyTexture, sf::Vector2f(800.f, 400.f));
                enemies.emplace_back(enemyTexture, sf::Vector2f(100.f, 400.f));
                enemies.emplace_back(enemyTexture, sf::Vector2f(300.f, 400.f));
            }

            player.handleInput(window);
            sf::Vector2f movement = player.getMovement();

            background.update(movement, dt, window, state);
            player.update(dt);

            float bgOffset = background.getOffsetX();

            for (auto& enemy : enemies) {
                sf::Vector2f playerWorldPos = player.getPosition();
                playerWorldPos.x -= bgOffset;

                enemy.update(dt, playerWorldPos);
            }

            float windowWidth = (float)window.getSize().x;
            level.playerGroundCollision(player, dt, windowWidth, movement.x);

            for (auto& enemy : enemies) {
                if (enemy.getIsAlive()) {

                    sf::FloatRect pBounds = player.getBounds();
                    sf::FloatRect eBounds = enemy.getBounds();

                    // --- МАГИЯ ЗДЕСЬ: Уменьшаем хитбоксы для SFML 3 ---
                    float shrinkFactor = 0.6f; // 60% от оригинального размера

                    // Сжимаем хитбокс игрока к центру
                    float pShrinkX = pBounds.size.x * (1 - shrinkFactor);
                    float pShrinkY = pBounds.size.y * (1 - shrinkFactor);

                    pBounds.position.x += pShrinkX / 2;
                    pBounds.position.y += pShrinkY / 2;
                    pBounds.size.x *= shrinkFactor;
                    pBounds.size.y *= shrinkFactor;

                    // Сжимаем хитбокс врага к центру
                    float eShrinkX = eBounds.size.x * (1 - shrinkFactor);
                    float eShrinkY = eBounds.size.y * (1 - shrinkFactor);

                    eBounds.position.x += eShrinkX / 2;
                    eBounds.position.y += eShrinkY / 2;
                    eBounds.size.x *= shrinkFactor;
                    eBounds.size.y *= shrinkFactor;
                    // ---------------------------------------

                    bool isColliding =
                        pBounds.position.x < eBounds.position.x + eBounds.size.x &&
                        pBounds.position.x + pBounds.size.x > eBounds.position.x &&
                        pBounds.position.y < eBounds.position.y + eBounds.size.y &&
                        pBounds.position.y + pBounds.size.y > eBounds.position.y;

                    if (isColliding) {
                        player.applyDamage(1);
                        break;
                    }
                }
            }

            for (size_t i = 0; i < enemies.size(); ) {
                if (!enemies[i].getIsAlive()) {
                    enemies.erase(enemies.begin() + i);
                } else {
                    i++;
                }
            }

            if (player.isDead()) {
                state = 4;
            }

            drawGame(window, background, level, player);

            for (auto& enemy : enemies) {
                if (enemy.getIsAlive()) {
                    sf::Sprite sprite = enemy.getSprite();

                    sf::Vector2f originalPos = sprite.getPosition();
                    sprite.setPosition({originalPos.x + bgOffset, originalPos.y});

                    window.draw(sprite);
                    sprite.setPosition(originalPos);
                }
            }
            window.display();
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