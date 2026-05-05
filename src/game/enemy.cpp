#include "enemy.hpp"

#include <iostream>

#include "background.hpp"
#include "player.hpp"

Enemy::Enemy(sf::Texture& texture, sf::Vector2f position)
        : sprite(texture), health(1), speed(50.f), position(position) {
    sprite.setOrigin({static_cast<float>(frameWidth) / 2.f, static_cast<float>(frameHeight)});
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

        if (direction.x < 0) {
            sprite.setScale({3.f, 2.f});
        } else {sprite.setScale({-3.f, 2.f});}

        position.x += direction.x * speed * dt;
        sprite.setPosition(position);
    }
}

bool Enemy::canAttack(sf::Vector2f playerPos) const {
    sf::Vector2f hitboxCenter = getHitboxCenter();

    float dx = playerPos.x - hitboxCenter.x;
    float dy = playerPos.y - hitboxCenter.y;

    float distance = std::sqrt(dx * dx + dy * dy);

    return distance <= attackRadius;
}

void Enemy::takeDamage(int damage) {
     if (getIsAlive()) {
        health -= damage;
         if (health <= 0) {
             death();
         }
     }
}

sf::Vector2f Enemy::getHitboxCenter() const {
    return {
        position.x,
        position.y - (static_cast<float>(frameHeight) * sprite.getScale().y) / 2.f
    };
}

void Enemy::death() {
    isAlive = false;
}

bool Enemy::getIsAlive() const {
    return isAlive;
}

sf::Sprite Enemy::getSprite() {
    return sprite;
}

sf::Vector2f Enemy::getPosition() const {
    return position;
}

void Enemy::setPosition(sf::Vector2f pos) {
    position = pos;
    sprite.setPosition(pos);
}