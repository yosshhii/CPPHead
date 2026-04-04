#include <iostream>
#include <SFML/Graphics.hpp>

#include "engine/window.hpp"
#include "game/player.hpp"
#include "game/background.hpp"

int main() {
    sf::RenderWindow window = createWindow();

    Player player;
    Background background;

    sf::Clock clock;

    sf::Image collisionMap ("assets/textures/HillsFloor.jpg");
    sf::Texture ground("assets/textures/Background3.png");
    sf::Sprite groundSprite1{ground}, groundSprite2{ground};
    groundSprite1.setScale({2.5, 2.5});
    groundSprite2.setScale({2.5, 2.5});
    groundSprite1.setPosition(background.getSprite1().getPosition());
    groundSprite2.setPosition(background.getSprite2().getPosition());

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

        //Collision with floor
        sf::Vector2f feetPos = player.getPosition();
        bool wasOnGround = player.getOnGround();
        player.setOnGround(false);

        const sf::Sprite& backgroundSprite1 = background.getSprite1();
        const sf::Sprite& backgroundSprite2 = background.getSprite2();

        const sf::Sprite* currentFloor = nullptr;
        if (backgroundSprite1.getGlobalBounds().contains(feetPos)) currentFloor = &backgroundSprite1;
        else if (backgroundSprite2.getGlobalBounds().contains(feetPos)) currentFloor = &backgroundSprite2;

        if (!currentFloor && wasOnGround) {
            sf::Vector2f checkBelow = {feetPos.x, feetPos.y + 5.f};
            if (backgroundSprite1.getGlobalBounds().contains(checkBelow)) currentFloor = &backgroundSprite1;
            else if (backgroundSprite2.getGlobalBounds().contains(checkBelow)) currentFloor = &backgroundSprite2;
        }

        if (currentFloor) {
            int maskX = static_cast<int>(std::round((feetPos.x - currentFloor->getPosition().x) / 2.5f));
            int maskY = static_cast<int>(std::round((feetPos.y - currentFloor->getPosition().y) / 2.5f));

            maskX = std::clamp(maskX, 0, (int)collisionMap.getSize().x - 1);
            maskY = std::clamp(maskY, 0, (int)collisionMap.getSize().y - 1);

            sf::Color pixelColor = collisionMap.getPixel({(unsigned int)maskX, (unsigned int)maskY});

            if (pixelColor.r < 50) {
                while (maskY > 0 && collisionMap.getPixel({(unsigned int)maskX, (unsigned int)(maskY - 1)}).r < 50) {
                    maskY--;
                }

                float groundY = std::round((maskY * 2.5f) + currentFloor->getPosition().y);
                player.setPosition({feetPos.x, groundY});
                player.setVelocityY(0.f);
                player.setOnGround(true);
                player.setJumping(false);
            }
        }

        //end of collision mechanic

        groundSprite1.setPosition(background.getSprite1().getPosition());
        groundSprite2.setPosition(background.getSprite2().getPosition());

        player.update(dt);

        window.clear();
        background.draw(window);
        window.draw(groundSprite1);
        window.draw(groundSprite2);
        player.draw(window);
        window.display();
    }
    return 0;
}
