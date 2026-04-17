#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "health.hpp"

struct DustParticle {
    sf::Sprite sprite;
    float lifetime = 0.f;
};

class Player {
private:
    std::vector<DustParticle> dustParticles;

    sf::Texture walkTexture;
    sf::Texture jumpTexture;
    sf::Texture attackTexture;
    sf::Texture dustTexture;
    sf::Sprite sprite;

    sf::SoundBuffer jumpBuffer;
    sf::SoundBuffer walk1Buffer, walk2Buffer;
    sf::Sound jumpSound;
    sf::Sound walk1Sound, walk2Sound;


    int frameWidth = 32;
    int frameHeight = 32;

    int walkFrame = 0;
    int jumpFrame = 0;
    int attackFrame = 0;

    float animationTimer = 0.f;
    float jumpPeakTimer = 0.f;
    float dustTimer = 0.f;
    float animationSpeed = 0.12f;
    float attackDuration = 0.1f;
    float jumpDuration = 0.12f;

    float dustScale = 2.f;

    sf::Vector2f velocity{0.f,0.f};
    sf::Vector2f movement{0.f,0.f};

    float jumpForce = -600.f;
    float gravity = 1500.f;
    float maxFallSpeed = 800.f;

    bool isOnGround = true;
    bool isJumping = false;
    bool isAttacking = false;

    bool wasAttackPressed = false;

    HealthComponent health;
    sf::Texture healthTexture;
    sf::Sprite healthSprite;

    int healthBarWidth = 384;
    int healthBarHeight = 128;

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

    void applyDamage(int amount);
    const HealthComponent& getHealth() const;
};
