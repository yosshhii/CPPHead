#pragma once

#include <SFML/Graphics.hpp>

class Enemy {
private:
    sf::Sprite sprite;
    int health;
    float speed;
    float chaseRadius = 300.f;
    float attackRadius = 50.f;

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