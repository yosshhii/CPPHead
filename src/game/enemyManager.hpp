#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "enemy.hpp"
#include "player.hpp"

class EnemyManager {
private:
    std::vector<Enemy> enemies;
    sf::Texture skeletonIdleTexture;
    sf::Texture skeletonWalkTexture;

public:
    EnemyManager();

    void spawnSkeleton(sf::Vector2f position);
    void update(float dt, Player& player, sf::Vector2f playerWorldFeet, sf::Vector2f playerWorldCenter);
    void draw(sf::RenderWindow& window, float bgOffset);
    void handlePlayerAttack(Player& player, float levelOffset);

    std::vector<Enemy>& getEnemies();
};
