#include "player.hpp"

Player::Player()
        : walkTexture("assets/textures/Owlet_Monster_Walk_6.png"),
        jumpTexture("assets/textures/Owlet_Monster_Jump_8.png"),
        sprite(walkTexture),
        walk1Buffer("assets/sounds/walk1.wav"),
        walk2Buffer("assets/sounds/walk2.wav"),
        jumpBuffer("assets/sounds/jump.wav"),
        walk1Sound(walk1Buffer),
        walk2Sound(walk2Buffer),
        jumpSound(jumpBuffer),
        health(100, 1),
        healthTexture("assets/textures/Health/Health_Bar_Horiz.png"),
        healthSprite(healthTexture)
{
    sprite.setScale({3.f, 2.f});
    sprite.setOrigin({static_cast<float>(frameWidth) / 2.f, static_cast<float>(frameHeight)});
    sprite.setPosition({0.f, 200.f});

    walk1Sound.setVolume(50.f);
    walk2Sound.setVolume(50.f);

    jumpSound.setVolume(15.f);

    healthSprite.setTextureRect(sf::IntRect({0, 0}, {healthBarWidth, healthBarHeight}));
    healthSprite.setScale({0.6f, 0.6f});
    healthSprite.setPosition({-530.f, 270.f});
}

void Player::handleInput(const sf::RenderWindow& window) {
    movement = {0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) or (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) )) {
        movement.x += 1;
        sprite.setScale({3.f,2.f});}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) or (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) )) {
        movement.x -= 1;
        sprite.setScale({-3.f,2.f});}

    bool isMoving = (movement.x != 0.f);

    if (isMoving & !isJumping) {
        if ((walk1Sound.getStatus() != sf::SoundSource::Status::Playing) && (walk2Sound.getStatus() != sf::SoundSource::Status::Playing)) {
            int random = rand() % 2;
            if (random == 0) walk1Sound.play();
            if (random == 1) walk2Sound.play();
        }
    } else {
        walk1Sound.stop();
        walk2Sound.stop();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space) && isOnGround) {
        velocity.y = jumpForce;
        isOnGround = false;
        isJumping = true;

        currentFrame = 0;
        animationTimer = 0.f;

        walk1Sound.stop();
        jumpSound.play();
    }


    // тест получения урона
    // потом удалить нужно будет
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::T)) {
        applyDamage(10);
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

    health.update(dt);

    if (health.isDead()) {
        // тут должна быть логика при смерти
        return;
    }

    int currentHp = health.getHealth();

    if (currentHp > 70) {
        healthSprite.setTextureRect(sf::IntRect({0, 0}, {healthBarWidth, healthBarHeight}));
    } else if (currentHp > 30) {
        healthSprite.setTextureRect(sf::IntRect({0, healthBarHeight}, {healthBarWidth, healthBarHeight}));
    } else {
        healthSprite.setTextureRect(sf::IntRect({0, healthBarHeight * 2}, {healthBarWidth, healthBarHeight}));
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(healthSprite);
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

void Player::applyDamage(int amount) {
    health.takeDamage(amount);
}

const HealthComponent& Player::getHealth() const {
    return health;
}





