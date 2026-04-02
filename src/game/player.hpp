#pragma once
#include <SFML/Graphics.hpp>

class Player {
private:
    sf::Texture walkTexture;
    sf::Texture jumpTexture;
    sf::Sprite sprite;

    sf::Vector2f movement{};

    int frameWidth = 32;
    int frameHeight = 32;

    int currentFrame = 0;
    float animationTimer = 0.f;
    float animationSpeed = 0.12f;

    //float x, y;
    //float width, height;
    float velocityY = 0.f;
    float jumpForce = -600.f;
    float gravity = 1500.f;
    bool isOnGround = true;

public:
    void init();
    void update(float deltaTime);
    void handleInput(const sf::RenderWindow& window);
    void draw(sf::RenderWindow &window);
};
