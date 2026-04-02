#include "player.hpp"

void Player::init() {
    walkTexture.loadFromFile("assets/textures/Owlet_Monster_Walk_6.png");
    jumpTexture.loadFromFile("assets/textures/Owlet_Monster_Jump_8.png");

    sprite = sf::Sprite(walkTexture);
    sprite.setScale({3.f,2.f});
    sprite.setOrigin({static_cast<float>(frameWidth) / 2.f, static_cast<float>(frameHeight)});
    sprite.setPosition({0.f, 200.f});
}

void Player::handleInput(const sf::RenderWindow& window) {
    movement = {0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) ) {
        movement.x += 1;
        sprite.setScale({3.f,2.f});}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {
        movement.x -= 1;
        sprite.setScale({-3.f, 2.f});}

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space) && isOnGround) {
        velocityY = jumpForce;
        isOnGround = false;
        currentFrame = 0;
        animationTimer = 0.f;
    }
}

void Player::update(float dt) {
    if (!isOnGround) {
        sprite.setTexture(jumpTexture);
    } else {
        sprite.setTexture(walkTexture);
    }

    velocityY += gravity * dt;
    sprite.move({0.f, velocityY * dt});

    float groundY = 200.f;
    if (sprite.getPosition().y >= groundY) {
        sprite.setPosition({sprite.getPosition().x, groundY});
        velocityY = 0.f;
        isOnGround = true;
    }

    bool isMoving = (movement.x != 0.f);

    if (isMoving && isOnGround) {
        animationTimer += dt;
        if (animationTimer >= animationSpeed) {
            animationTimer = 0.f;
            currentFrame = (currentFrame + 1) % 6;
        }

        sprite.setTextureRect(sf::IntRect(
            {currentFrame * frameWidth, 0},
            {frameWidth, frameHeight}
        ));
    } else if (!isMoving && isOnGround) {
        sprite.setTextureRect(sf::IntRect(
            {5 * frameWidth, 0},
            {frameWidth, frameHeight}
        ));
    }

    if (!isOnGround) {
        animationTimer += dt;
        if (animationTimer >= 0.18f) {
            animationTimer = 0.f;
            if (currentFrame < 7) {
                currentFrame++;
            }
        }

        sprite.setTextureRect(sf::IntRect(
            {currentFrame * frameWidth, 0},
            {frameWidth, frameHeight}
        ));
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}