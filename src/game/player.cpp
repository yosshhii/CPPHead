#include "player.hpp"

Player::Player()
        : walkTexture("assets/textures/Owlet_Monster_Walk_6.png"),
        jumpTexture("assets/textures/Owlet_Monster_Jump_8.png"),
        sprite(walkTexture)
{
    sprite.setScale({3.f, 2.f});
    sprite.setOrigin({static_cast<float>(frameWidth) / 2.f, static_cast<float>(frameHeight)});
    sprite.setPosition({0.f, 200.f});
}

void Player::handleInput(const sf::RenderWindow& window) {
    movement = {0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) or (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) )) {
        movement.x += 1;
        sprite.setScale({3.f,2.f});}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) or (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) )) {
        movement.x -= 1;
        sprite.setScale({-3.f, 2.f});}

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space) && isOnGround) {
        velocity.y = jumpForce;
        isOnGround = false;
        isJumping = true;

        currentFrame = 0;
        animationTimer = 0.f;
    }
}

void Player::update(float dt) {
    if (!isOnGround) {
        velocity.y += gravity * dt;
        if (velocity.y > maxFallSpeed)
            velocity.y = maxFallSpeed;}

    sprite.move({0.f, velocity.y * dt});
    bool isMoving = (movement.x != 0);

    if (isJumping) {
        sprite.setTexture(jumpTexture);

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
    } else if (isMoving && isOnGround) {
        sprite.setTexture(walkTexture);

        animationTimer += dt;
        if (animationTimer >= animationSpeed) {
            animationTimer = 0.f;
            currentFrame = (currentFrame + 1) % 6;
        }

        sprite.setTextureRect(sf::IntRect({currentFrame * frameWidth, 0}, {frameWidth, frameHeight}));
    } else if (isOnGround) {
        sprite.setTexture(walkTexture);
        currentFrame = 5;

        sprite.setTextureRect(sf::IntRect({5 * frameWidth, 0}, {frameWidth, frameHeight}));
    }

}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::Vector2f Player::getVelocity() const {
    return velocity;
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

sf::Vector2f Player::getMovement() const {
    return movement;
}

bool Player::getOnGround() const {
    return isOnGround;
}

bool Player::getJumping() const {
    return isJumping;
}

void Player::setPosition(const sf::Vector2f& pos) {
    sprite.setPosition(pos);
}

void Player::setVelocityY(float value) {
    velocity.y = value;
}

void Player::setOnGround(bool value) {
    isOnGround = value;
}

void Player::setJumping(bool value) {
    isJumping = value;
}





