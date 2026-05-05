#pragma once

#include <SFML/Graphics.hpp>

class Enemy {
private:
    sf::Sprite sprite;
    int health;
    float speed;
    float chaseRadius = 300.f;
    float attackRadius = 45.f;

    int frameWidth = 64;
    int frameHeight = 64;

    sf::Vector2f position;

    bool isAlive = true;
public:
    Enemy(sf::Texture& texture, sf::Vector2f position);

    void update(float dt, sf::Vector2f playerPos);
    void takeDamage(int damage);
    bool canAttack(sf::Vector2f playerPos) const;
    void death();

    sf::Sprite getSprite();
    bool getIsAlive() const;
    sf::Vector2f getHitboxCenter() const;

    sf::Vector2f getPosition() const;

    void setPosition(sf::Vector2f pos);
};