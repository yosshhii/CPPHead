#pragma once

class HealthComponent {
private:
    int maxHealth;
    int currentHealth;
    float invulnerabilityTime;
    float timer;
    bool invulnerable;

public:
    HealthComponent(int hp, float iTime);
    void takeDamage(int amount);
    void heal(int amount);
    void update(float deltaTime);
    void reset();

    int getHealth() const;
    int getMaxHealth() const;
    bool isDead() const;
    bool isInvulnerable() const;
};