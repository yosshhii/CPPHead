#pragma once

#include <SFML/Graphics.hpp>

class Enemy {
private:
    sf::Sprite sprite;
    int health;
    float speed;
    float chaseRadius = 300.f;

    int frameWidth = 64;
    int frameHeight = 64;

    sf::Vector2f position;

    bool isAlive = true;
public:
    Enemy(sf::Texture& texture, sf::Vector2f position);

    void update(float dt, sf::Vector2f playerPos);
    void takeDamage(int damage);
    void death();
    void draw(sf::RenderWindow &window);

    sf::Sprite getSprite();
    bool getIsAlive() const;

    sf::Vector2f getPosition();

    void setPosition(sf::Vector2f pos);
};