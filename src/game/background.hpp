#pragma once
#include <SFML/Graphics.hpp>

class Background {
private:
    sf::Texture backgroundTexture;
    sf::Sprite sprite1;
    sf::Sprite sprite2;

    float width = 0.f;
    float height = 0.f;

    float scale = 2.5f;
    float offsetY = -100.f;

public:
    Background();

    void update(const sf::Vector2f& movement, float dt, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    const sf::Sprite& getSprite1() const;
    const sf::Sprite& getSprite2() const;

    float getWidth() const;
    float getHeight() const;
};