#include "level.hpp"
#include "player.hpp"
#include "background.hpp"

Level::Level()
    : collisionMap ("assets/textures/HillsFloor.jpg"),
      groundTexture("assets/textures/Background3.png"),
      groundSprite1(groundTexture),
      groundSprite2(groundTexture)
{
    groundSprite1.setScale({scale, scale});
    groundSprite2.setScale({scale, scale});
}

void Level::syncWithBackground(const Background& background) {
    groundSprite1.setPosition(background.getSprite1().getPosition());
    groundSprite2.setPosition(background.getSprite2().getPosition());
}

void Level::draw(sf::RenderWindow& window) {
    window.draw(groundSprite1);
    window.draw(groundSprite2);
}

void Level::playerGroundCollision(Player& player, const Background& background) {
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
}