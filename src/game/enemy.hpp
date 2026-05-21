#pragma once

#include <SFML/Graphics.hpp>
#include "hitbox.hpp"
#include <cmath>

enum class EnemyType {
    SkeletonSword,
    SkeletonUnarmed
};

struct EnemyStats {
    int health;
    float speed;
    float chaseRadius;
    float attackRadius;
    int damage;
    float attackCooldown;
    float attackAnimSpeed;
    int attackFramesCount;
    int idleFramesCount;
    int walkFramesCount;
    int hurtFramesCount;
};

struct EnemyTextures {
    sf::Texture* idle;
    sf::Texture* walk;
    sf::Texture* attack;
    sf::Texture* hurt;
};

class Enemy {
    enum class State { Idle, Chasing, Attacking, Hurt };

    EnemyTextures textures;
    EnemyStats stats;
    sf::Sprite sprite;

    State state = State::Idle;

    int idleFrame = 0;
    int walkFrame = 0;
    int attackFrame = 0;
    int hurtFrame = 0;
    float animationTimer = 0.f;

    float enemyScaleX = 3.f;
    float enemyScaleY = 2.f;
    float facingScaleX = 3.f;

    float velocityY = 0.f;
    bool isOnGround = false;

    int frameWidth = 64;
    int frameHeight = 64;
    float footFixY = 16.f;

    sf::Vector2f position;
    bool isAlive = true;
    int currentHealth;

    float attackCooldownTimer = 0.f;
    bool damageDealtThisAttack = false;

    float hurtTimer = 0.f;

    void updateAnimation(float dt);
    void setState(State newState);

public:
    Enemy(EnemyTextures textures, EnemyStats stats, sf::Vector2f position);

    void update(float dt, sf::Vector2f playerPos);
    void takeDamage(int damage);

    bool shouldDealDamage() const;
    void markDamageDealt();
    int getDamage() const;

    void setVelocityY(float velocity);
    void setOnGround(bool value);
    void setPosition(sf::Vector2f pos);

    sf::Sprite getSprite() const;
    bool getIsAlive() const;
    sf::Vector2f getHitboxCenter() const;
    sf::Vector2f getScale() const;
    sf::Vector2f getPosition() const;
    Hitbox getBodyHitbox() const;
};
