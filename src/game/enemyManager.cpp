#include "enemyManager.hpp"

EnemyManager::EnemyManager() {
    if (!skeletonTexture.loadFromFile(
        "assets/textures/SkeletonEnemy/Idle/Sword/Skeleton_Default_Idle_Sword.png"
    )) {
        throw std::runtime_error("Failed to load skeleton texture");
    }
}

void EnemyManager::spawnSkeleton(sf::Vector2f position) {
    if (enemies.size() < 3) {
        enemies.emplace_back(skeletonTexture, position);
    }
}

void EnemyManager::update(float dt, Player& player, sf::Vector2f playerWorldPos) {
    for (auto& enemy : enemies) {
        enemy.update(dt, playerWorldPos);
    }

    for (size_t i = 0; i < enemies.size(); ) {
        if (!enemies[i].getIsAlive()) {
            enemies.erase(enemies.begin() + i);
        } else {
            i++;
        }
    }

    for (auto& enemy : enemies) {
        enemy.update(dt, playerWorldPos);

        if (enemy.canAttack(playerWorldPos)) {
            player.applyDamage(1);
        }
    }
}

void EnemyManager::draw(sf::RenderWindow& window, float bgOffset) {
    for (auto& enemy : enemies) {
        if (enemy.getIsAlive()) {
            sf::Sprite sprite = enemy.getSprite();

            sf::Vector2f pos = enemy.getPosition();
            sprite.setPosition({pos.x + bgOffset, pos.y});

            window.draw(sprite);
        }
    }
}

std::vector<Enemy>& EnemyManager::getEnemies() {
    return enemies;
}
