#pragma once

#include <SFML/Graphics.hpp>

class Enemy {
private:
    sf::Sprite sprite;
    int health;
    float speed;
    float chaseRadius = 300.f;
    float attackRadius = 50.f;

    int walkFrame = 0;
    int walkFramesCount = 6;

    float animationTimer = 0.f;
    float animationSpeed = 0.12f;

    float enemyScaleX = 3.f;
    float enemyScaleY = 2.f;

    float velocityY = 0.f;
    bool isOnGround = false;

    int frameWidth = 64;
    int frameHeight = 64;

    float footFixY = 16.f;

    sf::Vector2f position;

    bool isAlive = true;
public:
    Enemy(sf::Texture& texture, sf::Vector2f position);

    void update(float dt, sf::Vector2f playerPos);
    void updateAnimation(float dt, bool isMoving);
    void takeDamage(int damage);
    bool canAttack(sf::Vector2f playerPos) const;
    void death();

    void setVelocityY(float velocity);
    void setOnGround(bool value);
    void setPosition(sf::Vector2f pos);

    sf::Sprite getSprite();
    bool getIsAlive() const;
    sf::Vector2f getHitboxCenter() const;
    float getAttackRadius() const;
    sf::Vector2f getScale() const;
    sf::Vector2f getPosition() const;

};