#include "level.hpp"
#include "player.hpp"
#include "levelmanager.hpp"
#include "background.hpp"

Level::Level() {}

void Level::init() {
    levelManager.init();
}

void Level::draw(sf::RenderWindow& window) {
    levelManager.draw(window);
}

void Level::playerGroundCollision(Player& player, float dt, float windowWidth, float inputX) {
    float currentSpeed = 0.0f;
    float movementSpeed = 300.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        currentSpeed = movementSpeed;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        currentSpeed = -movementSpeed;
    }
    levelManager.update(dt, windowWidth, inputX * movementSpeed);

    sf::Vector2f pos = player.getPosition();

    sf::Vector2f checkPos(pos.x, pos.y + 2.0f);

    if (levelManager.checkCollision(checkPos)) {
        sf::Vector2f correctionPos = pos;
        while (levelManager.checkCollision(correctionPos) && correctionPos.y > 0) {
            correctionPos.y -= 1.0f;
        }

        player.setPosition({pos.x, correctionPos.y});
        player.setVelocityY(0.f);
        player.setOnGround(true);
        player.setJumping(false);
    } else {
        player.setOnGround(false);
    }
}
void Level::enemyGroundCollision(Enemy& enemy) {
    sf::Vector2f pos = enemy.getPosition();

    sf::Vector2f checkPos = {
        pos.x,
        pos.y + 2.f
    };

    if (levelManager.checkCollisionWorld(checkPos)) {
        sf::Vector2f correctionPos = pos;

        while (levelManager.checkCollisionWorld(correctionPos) && correctionPos.y > 0) {
            correctionPos.y -= 1.f;
        }

        enemy.setPosition({pos.x, correctionPos.y});
        enemy.setVelocityY(0.f);
        enemy.setOnGround(true);
    } else {
        enemy.setOnGround(false);
    }
}

float Level::getWorldOffset() const {
    return levelManager.getWorldOffset();
}