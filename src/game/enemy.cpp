#include "enemy.hpp"

Enemy::Enemy(EnemyTextures textures, EnemyStats stats, sf::Vector2f position)
    : textures(textures), stats(stats), currentHealth(stats.health), position(position), sprite(*textures.idle)
{
    sprite.setOrigin({static_cast<float>(frameWidth) / 2.f, static_cast<float>(frameHeight) - footFixY});
    sprite.setPosition(position);
    sprite.setScale({enemyScaleX, enemyScaleY});
    sprite.setTextureRect(sf::IntRect({0, 0}, {frameWidth, frameHeight}));
}

void Enemy::setState(State newState) {
    if (state == newState) return;
    state = newState;
    animationTimer = 0.f;
    idleFrame = 0;
    walkFrame = 0;
    attackFrame = 0;
    hurtFrame = 0;
}

void Enemy::update(float dt, sf::Vector2f playerPos) {
    if (!isAlive) return;

    if (!isOnGround)
        velocityY += 900.f * dt;
    position.y += velocityY * dt;

    attackCooldownTimer = std::min(attackCooldownTimer + dt, stats.attackCooldown);

    float dx = playerPos.x - position.x;
    float dy = playerPos.y - position.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance <= stats.chaseRadius)
        facingScaleX = (dx < 0) ? enemyScaleX : -enemyScaleX;

    if (state == State::Hurt) {
        hurtTimer += dt;
        if (hurtTimer >= 0.4f)
            setState(State::Idle);
        updateAnimation(dt);
        sprite.setScale({facingScaleX, enemyScaleY});
        sprite.setPosition(position);
        return;
    }

    if (state == State::Attacking) {
        updateAnimation(dt);
        sprite.setScale({facingScaleX, enemyScaleY});
        sprite.setPosition(position);
        return;
    }

    if (distance <= stats.attackRadius && attackCooldownTimer >= stats.attackCooldown) {
        attackCooldownTimer = 0.f;
        damageDealtThisAttack = false;
        setState(State::Attacking);
    } else if (distance <= stats.chaseRadius) {
        setState(State::Chasing);
        float len = std::sqrt(dx * dx + dy * dy);
        position.x += (dx / len) * stats.speed * dt;
    } else {
        setState(State::Idle);
    }

    updateAnimation(dt);
    sprite.setScale({facingScaleX, enemyScaleY});
    sprite.setPosition(position);
}

void Enemy::updateAnimation(float dt) {
    const float baseSpeed = 0.12f;
    animationTimer += dt;

    switch (state) {
        case State::Idle:
            sprite.setTexture(*textures.idle);
            if (animationTimer >= baseSpeed) {
                animationTimer = 0.f;
                idleFrame = (idleFrame + 1) % stats.idleFramesCount;
            }
            sprite.setTextureRect(sf::IntRect({idleFrame * frameWidth, 0}, {frameWidth, frameHeight}));
            break;

        case State::Chasing:
            sprite.setTexture(*textures.walk);
            if (animationTimer >= baseSpeed) {
                animationTimer = 0.f;
                walkFrame = (walkFrame + 1) % stats.walkFramesCount;
            }
            sprite.setTextureRect(sf::IntRect({walkFrame * frameWidth, 0}, {frameWidth, frameHeight}));
            break;

        case State::Attacking:
            sprite.setTexture(*textures.attack);
            if (animationTimer >= stats.attackAnimSpeed) {
                animationTimer = 0.f;
                attackFrame++;
                if (attackFrame >= stats.attackFramesCount) {
                    setState(State::Idle);
                    return;
                }
            }
            sprite.setTextureRect(sf::IntRect({attackFrame * frameWidth, 0}, {frameWidth, frameHeight}));
            break;

        case State::Hurt:
            sprite.setTexture(*textures.hurt);
            if (animationTimer >= baseSpeed) {
                animationTimer = 0.f;
                hurtFrame = (hurtFrame + 1) % stats.hurtFramesCount;
            }
            sprite.setTextureRect(sf::IntRect({hurtFrame * frameWidth, 0}, {frameWidth, frameHeight}));
            break;
    }
}

bool Enemy::shouldDealDamage() const {
    return state == State::Attacking &&
           !damageDealtThisAttack &&
           attackFrame >= stats.attackFramesCount / 2;
}

void Enemy::markDamageDealt() {
    damageDealtThisAttack = true;
}

int Enemy::getDamage() const {
    return stats.damage;
}

void Enemy::takeDamage(int damage) {
    if (!isAlive) return;
    currentHealth -= damage;
    if (currentHealth <= 0) {
        isAlive = false;
        return;
    }
    state = State::Hurt;
    hurtFrame = 0;
    hurtTimer = 0.f;
    animationTimer = 0.f;
}

sf::Vector2f Enemy::getHitboxCenter() const {
    return {
        position.x,
        position.y - (static_cast<float>(frameHeight) * std::abs(sprite.getScale().y)) / 2.f
    };
}

bool Enemy::getIsAlive() const { return isAlive; }

sf::Sprite Enemy::getSprite() const { return sprite; }

sf::Vector2f Enemy::getPosition() const { return position; }

void Enemy::setPosition(sf::Vector2f pos) {
    position = pos;
    sprite.setPosition(pos);
}

void Enemy::setVelocityY(float velocity) { velocityY = velocity; }

void Enemy::setOnGround(bool value) { isOnGround = value; }

sf::Vector2f Enemy::getScale() const { return sprite.getScale(); }

Hitbox Enemy::getBodyHitbox() const {
    float width = 45.f;
    float height = 80.f;
    sf::Vector2f center = getHitboxCenter();
    return {center.x - width / 2.f, center.y - height / 2.f, width, height};
}
