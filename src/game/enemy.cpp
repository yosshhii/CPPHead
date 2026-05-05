#include "enemy.hpp"

#include <iostream>

#include "background.hpp"
#include "player.hpp"

Enemy::Enemy(sf::Texture& texture, sf::Vector2f position)
        : sprite(texture), health(1), speed(50.f), position(position) {
    sprite.setOrigin({static_cast<float>(frameWidth) / 2.f, static_cast<float>(frameHeight) - footFixY});
    sprite.setPosition(position);
    sprite.setScale({enemyScaleX, enemyScaleY});
    sprite.setTextureRect(sf::IntRect(
                {0, 0},
                {frameWidth, frameHeight}));
}

void Enemy::update(float dt, sf::Vector2f playerPos) {
    bool isMoving = false;

    if (!isOnGround) {
        velocityY += 900.f * dt;
    }

    position.y += velocityY * dt;

    float distance = std::sqrt(std::pow(playerPos.x - position.x, 2) + std::pow(playerPos.y - position.y, 2));

    if (distance <= chaseRadius) {
        sf::Vector2f direction = playerPos - position;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        direction.x /= length; // нормализуем вектор

        if (direction.x < 0) {
            sprite.setScale({enemyScaleX, enemyScaleY});
        } else {sprite.setScale({-enemyScaleX, enemyScaleY});}

        position.x += direction.x * speed * dt;
        isMoving = true;
    }

    updateAnimation(dt, isMoving);

    sprite.setPosition(position);
}

void Enemy::updateAnimation(float dt, bool isMoving) {
    if (isMoving) {
        animationTimer += dt;

        if (animationTimer >= animationSpeed) {
            animationTimer = 0.f;
            walkFrame = (walkFrame + 1) % walkFramesCount;
        }
    } else {
        walkFrame = 0;
        animationTimer = 0.f;
    }

    sprite.setTextureRect(sf::IntRect(
        {walkFrame * frameWidth, 0},
        {frameWidth, frameHeight}
    ));
}

bool Enemy::canAttack(sf::Vector2f playerCenter) const {
    sf::Vector2f enemyCenter = getHitboxCenter();

    float attackWidth = 70.f;
    float attackHeight = 70.f;

    bool insideX =
        playerCenter.x >= enemyCenter.x - attackWidth / 2.f &&
        playerCenter.x <= enemyCenter.x + attackWidth / 2.f;

    bool insideY =
        playerCenter.y >= enemyCenter.y - attackHeight / 2.f &&
        playerCenter.y <= enemyCenter.y + attackHeight / 2.f;

    return insideX && insideY;
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

void Enemy::setVelocityY(float velocity) {
    velocityY = velocity;
}

void Enemy::setOnGround(bool value) {
    isOnGround = value;
}

float Enemy::getAttackRadius() const {
    return attackRadius;
}

sf::Vector2f Enemy::getScale() const {
    return sprite.getScale();
}