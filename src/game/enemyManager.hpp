#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "enemy.hpp"
#include "player.hpp"

class EnemyManager {
    std::vector<Enemy> enemies;

    sf::Texture swordIdleTexture;
    sf::Texture swordWalkTexture;
    sf::Texture swordAttackTexture;

    sf::Texture unarmedIdleTexture;
    sf::Texture unarmedWalkTexture;
    sf::Texture unarmedAttackTexture;

    sf::Texture hurtTexture;

    std::mt19937 rng{std::random_device{}()};
    float spawnTimer = 0.f;
    float spawnInterval = 3.f;
    int maxEnemies = 5;
    int pendingSpawns = 0;

    void spawnRandom(sf::Vector2f playerWorldPos);

public:
    EnemyManager();

    void reset();
    void update(float dt, Player& player, sf::Vector2f playerWorldFeet, sf::Vector2f playerWorldCenter);
    void draw(sf::RenderWindow& window, float levelOffset);
    void handlePlayerAttack(Player& player, float levelOffset);

    std::vector<Enemy>& getEnemies();
};
