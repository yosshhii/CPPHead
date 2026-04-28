#include "player.hpp"

Player::Player()
        : walkTexture("assets/textures/Owlet_Monster_Walk.png"),
        jumpTexture("assets/textures/Owlet_Monster_Jump.png"),
        attackTexture("assets/textures/Owlet_Monster_Attack.png"),
        sprite(walkTexture),
        walk1Buffer("assets/sounds/walk1.wav"),
        walk2Buffer("assets/sounds/walk2.wav"),
        jumpBuffer("assets/sounds/jump.wav"),
        walk1Sound(walk1Buffer),
        walk2Sound(walk2Buffer),
        jumpSound(jumpBuffer),
        health(3, 0.5f),
        healthTexture("assets/textures/Health/Health_Bar_Horiz.png"),
        healthSprite(healthTexture),
        runDustTexture("assets/textures/dust.png"),
        jumpDustTexture("assets/textures/jumpDust.png")
{
    sprite.setScale({3.f, 2.f});
    sprite.setOrigin({static_cast<float>(frameWidth) / 2.f, static_cast<float>(frameHeight)});
    sprite.setPosition({0.f, 200.f});

    walk1Sound.setVolume(75.f);
    walk2Sound.setVolume(75.f);

    jumpSound.setVolume(15.f);

    healthSprite.setTextureRect(sf::IntRect({0, 0}, {healthBarWidth, healthBarHeight}));
    healthSprite.setScale({healthBarScale, healthBarScale});
    healthSprite.setPosition({healthBarPosX, healthBarPosY});

    sprite.setTextureRect(sf::IntRect({0, 0}, {frameWidth, frameHeight}));
}

void Player::handleInput(const sf::RenderWindow& window) {
    movement = {0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) or (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) )) {
        movement.x += 1;
        sprite.setScale({playerScaleX,playerScaleY});}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) or (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) )) {
        movement.x -= 1;
        sprite.setScale({-playerScaleX,playerScaleY});}

    bool isMoving = (movement.x != 0.f);

    if (isMoving && !isJumping) {
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

        jumpFrame = 0;
        animationTimer = 0.f;

        walk1Sound.stop();
        jumpSound.play();

        for (int i = 0; i < 5; i++) {
            JumpDustParticle particle{sf::Sprite(jumpDustTexture), 0.4f};

            particle.sprite.setTextureRect(sf::IntRect(
                {0, 0},
                {frameWidth, frameHeight}
            ));

            particle.sprite.setPosition({
                sprite.getPosition().x + (rand() % 40 - 45),
                sprite.getPosition().y - 50.f
            });

            particle.velocity = {
                static_cast<float>(rand() % 100 - 50),
                static_cast<float>(-120 + rand() % 40)
            };

            particle.sprite.setScale({dustScale, dustScale});

            jumpDustParticles.push_back(particle);
        }
    }


    // тест получения урона
    // потом удалить нужно будет
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::T)) {
        applyDamage(1);
    }

    bool attackPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::H);

    if (attackPressed && !wasAttackPressed && !isAttacking) {
        isAttacking = true;
        attackFrame = 0;
    }

    wasAttackPressed = attackPressed;
}

void Player::update(float dt) {
    if (!isOnGround) {
        if (velocity.y < 0) {
            velocity.y += gravity * dt;
        } else {
            velocity.y += gravity * 1.2f * dt; // вниз быстрее падает
        }
    }

    sprite.move({0.f, velocity.y * dt});

    if (isOnGround) {
        isJumping = false;
    }

    bool isMoving = (movement.x != 0);

    if (isAttacking) {
        sprite.setTexture(attackTexture);

        animationTimer += dt;

        if (animationTimer >= attackDuration) {
            animationTimer = 0.f;
            if (attackFrame < 3) {
                attackFrame++;
            } else {
                isAttacking = false;
                attackFrame = 0;
            }
        }

        sprite.setTextureRect(sf::IntRect(
            {attackFrame * frameWidth, 0},
            {frameWidth, frameHeight}
        ));

    } else if (isJumping) {
        sprite.setTexture(jumpTexture);

        animationTimer += dt;
        if (velocity.y < 160.f) {
            if (animationTimer >= jumpDuration) {
                animationTimer = 0.f;

                if (jumpFrame < 3) {
                    jumpFrame++;
                }
            }
        }
        else {
            if (animationTimer >= jumpDuration) {
                animationTimer = 0.f;

                if (jumpFrame < 4) {
                    jumpFrame = 4;
                } else if (jumpFrame < 7) {
                    jumpFrame++;
                }
            }
        }

        sprite.setTextureRect(sf::IntRect(
            {jumpFrame * frameWidth, 0},
            {frameWidth, frameHeight}
        ));
    } else if (isMoving && isOnGround) {
        sprite.setTexture(walkTexture);

        animationTimer += dt;
        if (animationTimer >= animationSpeed) {
            animationTimer = 0.f;
            walkFrame = (walkFrame + 1) % 6;
        }

        dustTimer += dt;
        if (dustTimer >= dustAnimationDuration) {
            dustTimer = 0.f;

            DustParticle particle{sf::Sprite(runDustTexture), 0.4f};

            particle.sprite.setTextureRect(sf::IntRect(
                {0, 0},
                {frameWidth, frameHeight}
            ));

            if (movement.x > 0) {
                particle.sprite.setScale({dustScale,dustScale});
                particle.sprite.setPosition({
                sprite.getPosition().x - playerOffsetX,
                sprite.getPosition().y - playerOffsetY
            });
            } else {
                particle.sprite.setScale({-dustScale,dustScale});
                particle.sprite.setPosition({
                sprite.getPosition().x + playerOffsetX,
                sprite.getPosition().y - playerOffsetY
            });
            }
            particle.lifetime = dustLifeTime;

            dustParticles.push_back(particle);
        }


        sprite.setTextureRect(sf::IntRect({walkFrame * frameWidth, 0}, {frameWidth, frameHeight}));
    } else if (isOnGround) {
        sprite.setTexture(walkTexture);
        walkFrame = 5;
        dustTimer = 0.f;

        sprite.setTextureRect(sf::IntRect({5 * frameWidth, 0}, {frameWidth, frameHeight}));
    }

    health.update(dt);

    int currentHp = health.getHealth();

    if (currentHp > 2) {
        healthSprite.setTextureRect(sf::IntRect({0, 0}, {healthBarWidth, healthBarHeight}));
    } else if (currentHp > 1) {
        healthSprite.setTextureRect(sf::IntRect({0, healthBarHeight}, {healthBarWidth, healthBarHeight}));
    } else {
        healthSprite.setTextureRect(sf::IntRect({0, healthBarHeight * 2 + 8}, {healthBarWidth, healthBarHeight}));
    }

    for (auto& particle: dustParticles) {
        particle.lifetime -= dt;

        float alpha = (particle.lifetime / dustLifeTime) * 255.f;
        if (alpha < 0.f) alpha = 0.f;

        particle.sprite.setColor(sf::Color(
        255, 255, 255,
        static_cast<std::uint8_t>(alpha)
        ));

        particle.sprite.move({-movement.x * worldSpeed * dt, -20.f * dt});
    }

    for (auto& particle: jumpDustParticles) {
        particle.lifetime -= dt;

        float alpha = (particle.lifetime / dustLifeTime) * 255.f;
        if (alpha < 0.f) alpha = 0.f;

        particle.sprite.setColor(sf::Color(
        255, 255, 255,
        static_cast<std::uint8_t>(alpha)
        ));

        particle.sprite.move({-movement.x * worldSpeed * dt, -15 * dt});
        //particle.sprite.move({particle.velocity.x * dt,particle.velocity.y * dt});
    }

    // удаление партиклов ходьбы
    for (size_t i = 0; i < dustParticles.size(); ) {
        if (dustParticles[i].lifetime <= 0.f) {
            dustParticles.erase(dustParticles.begin() + i);
        } else {
            i++;
        }
    }

    // удаление партиклов прыжка
    for (size_t i = 0; i < jumpDustParticles.size(); ) {
        if (jumpDustParticles[i].lifetime <= 0.f) {
            jumpDustParticles.erase(jumpDustParticles.begin() + i);
        } else {
            i++;
        }
    }

}

void Player::draw(sf::RenderWindow& window) {
    for (auto& particle: dustParticles) {
        window.draw(particle.sprite);
    }

    for (auto& particle: jumpDustParticles) {
        window.draw(particle.sprite);
    }

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

bool Player::isDead() const {
    return health.isDead();
}

void Player::reset(sf::Vector2f startPosition) {
    sprite.setPosition(startPosition);
    velocity.y = 0.f;
    isOnGround = false;
    isJumping = false;
    isAttacking = false;

    sprite.setTextureRect(sf::IntRect({0, 0}, {frameWidth, frameHeight}));

    health.reset();

    dustParticles.clear();
    jumpDustParticles.clear();
}
