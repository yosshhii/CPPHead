#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Player {
private:
    sf::Texture walkTexture;
    sf::Texture jumpTexture;
    sf::Sprite sprite;

    sf::SoundBuffer jumpBuffer;
    sf::SoundBuffer walk1Buffer, walk2Buffer;
    sf::Sound jumpSound;
    sf::Sound walk1Sound, walk2Sound;


    int frameWidth = 32;
    int frameHeight = 32;

    int currentFrame = 0;
    float animationTimer = 0.f;
    float animationSpeed = 0.12f;

    //float x, y;
    //float width, height;

    sf::Vector2f velocity{0.f,0.f};
    sf::Vector2f movement{0.f,0.f};

    float jumpForce = -600.f;
    float gravity = 1500.f;
    float maxFallSpeed = 800.f;

    bool isOnGround = true;
    bool isJumping = false;

public:
    Player();

    void init();
    void update(float deltaTime);
    void handleInput(const sf::RenderWindow& window);
    void draw(sf::RenderWindow &window);

    sf::Vector2f getPosition() const;
    sf::Vector2f getMovement() const;
    sf::Vector2f getVelocity() const;
    bool getOnGround() const;
    bool getJumping() const;

    void setPosition(const sf::Vector2f& position);
    void setVelocityY(float value);
    void setOnGround(bool value);
    void setJumping(bool value);
};
