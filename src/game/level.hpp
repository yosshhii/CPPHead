#pragma once

#include <SFML/Graphics.hpp>
#include "levelmanager.hpp"
#include <cmath>

class Player;
class Background;

class Level {
private:
    sf::Image collisionMap;
    // sf::Texture groundTexture;
    // sf::Sprite groundSprite1;
    // sf::Sprite groundSprite2;
    LevelManager levelManager;

    float scale = 2.5f;
public:
    Level();

    void init();
    void update(float dt, float windowWidth);
    void draw(sf::RenderWindow& window);
    // void syncWithBackground(const Background& background);
    // void playerGroundCollision(Player& player, const Background& background);
    // void draw(sf::RenderWindow& window);
    void playerGroundCollision(Player& player, float dt, float windowWidth);
    bool checkCollision(sf::Vector2f position);
};