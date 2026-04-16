#include "level.hpp"
#include "player.hpp"
#include "levelmanager.hpp"
#include "background.hpp"

Level::Level() {
    levelManager.init();
}

// void Level::syncWithBackground(const Background& background) {
// }

void Level::draw(sf::RenderWindow& window) {
    levelManager.draw(window);
}

void Level::playerGroundCollision(Player& player, float dt, float windowWidth) {
    float currentSpeed = 0.0f;
    float movementSpeed = 400.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        currentSpeed = movementSpeed;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        currentSpeed = -movementSpeed;
    }

    levelManager.update(dt, windowWidth, currentSpeed);

    sf::Vector2f feetPos = player.getPosition();

    sf::Vector2f checkPos(feetPos.x, feetPos.y + 2.0f);

    if (levelManager.checkCollision(checkPos)) {
        sf::Vector2f correctionPos = feetPos;

        while (levelManager.checkCollision(correctionPos) && correctionPos.y > 0) {
            correctionPos.y -= 1.0f;
        }

        player.setPosition({feetPos.x, correctionPos.y});
        player.setVelocityY(0.f);
        player.setOnGround(true);
        player.setJumping(false);
    }
    else {
        player.setOnGround(false);
    }
}