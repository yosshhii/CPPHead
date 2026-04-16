#include "health.hpp"
#include <algorithm>

HealthComponent::HealthComponent(int hp, float iTime)
        : maxHealth(hp), currentHealth(hp), invulnerabilityTime(iTime), timer(0.0f), invulnerable(false) {}

void HealthComponent::takeDamage(int amount) {
    if (invulnerable || currentHealth <= 0) return;

    currentHealth -= amount;
    if (currentHealth < 0) currentHealth = 0;

    if (currentHealth > 0) {
        invulnerable = true;
        timer = invulnerabilityTime;
    }
}

void HealthComponent::heal(int amount) {
    if (currentHealth <= 0) return;
    currentHealth = std::min(currentHealth + amount, maxHealth);
}

void HealthComponent::update(float deltaTime) {
    if (invulnerable) {
        timer -= deltaTime;
        if (timer <= 0.0f) {
            invulnerable = false;
        }
    }
}

int HealthComponent::getHealth() const { return currentHealth; }
int HealthComponent::getMaxHealth() const { return maxHealth; }
bool HealthComponent::isDead() const { return currentHealth <= 0; }
bool HealthComponent::isInvulnerable() const { return invulnerable; }