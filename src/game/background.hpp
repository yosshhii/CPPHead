#pragma once
#include <SFML/Graphics.hpp>

class Background {
private:
    sf::Texture texture;
    sf::Sprite sprite1;
    sf::Sprite sprite2;

    float width = 0.f;
    float height = 0.f;

public:
    Background();

    void init();
    void update(const sf::Vector2f& movement, float dt, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
};