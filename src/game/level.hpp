#pragma once

#include <SFML/Graphics.hpp>
#include "levelmanager.hpp"
#include "enemy.hpp"
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
    void draw(sf::RenderWindow& window);
    void playerGroundCollision(Player& player, float dt, float windowWidth, float inputX);
    void enemyGroundCollision(Enemy& enemy);

    float getWorldOffset() const;
};