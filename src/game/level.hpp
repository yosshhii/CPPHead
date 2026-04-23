#pragma once

#include <SFML/Graphics.hpp>
#include "levelmanager.hpp"
#include <cmath>

class Player;
class Background;

class Level {
private:
    sf::Image collisionMap;
    LevelManager levelManager;

    float scale = 2.5f;
public:
    Level();

    void init();
    void update(float dt, float windowWidth);
    void draw(sf::RenderWindow& window);
    void playerGroundCollision(Player& player, float dt, float windowWidth, float inputX);
    bool checkCollision(sf::Vector2f position);
};