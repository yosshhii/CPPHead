#include "enemy.hpp"

Enemy::Enemy(sf::Texture& texture, sf::Vector2f position)
        : sprite(texture), health(1), speed(50.f), position(position) {
    sprite.setPosition(position);
    sprite.setScale({3.f, 2.f});
    sprite.setTextureRect(sf::IntRect(
                {0, 0},
                {frameWidth, frameHeight}));

}

void Enemy::update(float dt, sf::Vector2f playerPos) {
    float distance = std::sqrt(std::pow(playerPos.x - position.x, 2) + std::pow(playerPos.y - position.y, 2));

    if (distance <= chaseRadius) {
        sf::Vector2f direction = playerPos - position;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        direction.x /= length; // нормализуем вектор

        position += direction * speed * dt;
        sprite.setPosition(position);
    }
}


void Enemy::takeDamage(int damage) {
     if (getIsAlive()) {
        health -= damage;
         if (health <= 0) {
             death();
         }
     }
}

void Enemy::draw(sf::RenderWindow &window) {
    if (getIsAlive()) {
        window.draw(sprite);
    }
}

void Enemy::death() {
    isAlive = false;
}

bool Enemy::getIsAlive() const {
    return isAlive;
}

sf::FloatRect Enemy::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Sprite Enemy::getSprite() {
    return sprite;
}

sf::Vector2f Enemy::getPosition() {
    return sprite.getPosition();
}

void Enemy::setPosition(sf::Vector2f pos) {
    sprite.setPosition(pos);
}