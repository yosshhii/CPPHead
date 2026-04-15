#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

class Player;
class Background;

class Level {
private:
    sf::Image collisionMap;
    sf::Texture groundTexture;
    sf::Sprite groundSprite1;
    sf::Sprite groundSprite2;

    float scale = 2.5f;
public:
    Level();

    void syncWithBackground(const Background& background);
    void playerGroundCollision(Player& player, const Background& background);
    void draw(sf::RenderWindow& window);
};