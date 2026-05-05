#include "enemyManager.hpp"

EnemyManager::EnemyManager() {
    if (!skeletonWalkTexture.loadFromFile(
        "assets/textures/SkeletonEnemy/Run/Sword/Skeleton_Default_Run_Sword.png"
    )) {
        throw std::runtime_error("Failed to load skeleton texture");
    }
    if (!skeletonIdleTexture.loadFromFile(
        "assets/textures/SkeletonEnemy/Idle/Sword/Skeleton_Default_Idle_Sword.png"
    )) {
        throw std::runtime_error("Failed to load skeleton texture");
    }
}

void EnemyManager::spawnSkeleton(sf::Vector2f position) {
    if (enemies.size() < 3) {
        enemies.emplace_back(skeletonIdleTexture, skeletonWalkTexture, position);
    }
}

void EnemyManager::update(float dt, Player& player, sf::Vector2f playerWorldFeet, sf::Vector2f playerWorldCenter) {
    for (auto& enemy : enemies) {
        enemy.update(dt, playerWorldFeet);
    }

    for (size_t i = 0; i < enemies.size(); ) {
        if (!enemies[i].getIsAlive()) {
            enemies.erase(enemies.begin() + i);
        } else {
            i++;
        }
    }

    for (auto& enemy : enemies) {
        if (enemy.canAttack(playerWorldCenter)) {
            player.applyDamage(1);
        }
    }
}

void EnemyManager::draw(sf::RenderWindow& window, float levelOffset) {
    for (auto& enemy : enemies) {
        if (enemy.getIsAlive()) {
            sf::Sprite sprite = enemy.getSprite();

            sf::Vector2f pos = enemy.getPosition();
            sprite.setPosition({pos.x - levelOffset, pos.y});

            window.draw(sprite);
        }
    }
}

std::vector<Enemy>& EnemyManager::getEnemies() {
    return enemies;
}
