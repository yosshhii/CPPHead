#include "enemyManager.hpp"
#include <algorithm>
#include <stdexcept>

static void loadTexture(sf::Texture& tex, const std::string& path) {
    if (!tex.loadFromFile(path))
        throw std::runtime_error("Failed to load texture: " + path);
}

static EnemyStats makeSwordStats() {
    // health, speed, chaseRadius, attackRadius, damage, attackCooldown, attackAnimSpeed, attackFrames, idleFrames, walkFrames, hurtFrames
    return {2, 55.f, 300.f, 65.f, 1, 1.5f, 0.09f, 6, 6, 6, 2};
}

static EnemyStats makeUnarmedStats() {
    return {1, 85.f, 320.f, 55.f, 1, 1.0f, 0.09f, 6, 6, 6, 2};
}

EnemyManager::EnemyManager() {
    loadTexture(swordIdleTexture,     "assets/textures/SkeletonEnemy/Idle/Sword/Skeleton_Default_Idle_Sword.png");
    loadTexture(swordWalkTexture,     "assets/textures/SkeletonEnemy/Run/Sword/Skeleton_Default_Run_Sword.png");
    loadTexture(swordAttackTexture,   "assets/textures/SkeletonEnemy/Attack/Sword/Skeleton_Default_Attack_Sword.png");
    loadTexture(unarmedIdleTexture,   "assets/textures/SkeletonEnemy/Idle/Unarmed/Skeleton_Default_Idle_Unarmed.png");
    loadTexture(unarmedWalkTexture,   "assets/textures/SkeletonEnemy/Run/Unarmed/Skeleton_Default_Run_Unarmed.png");
    loadTexture(unarmedAttackTexture, "assets/textures/SkeletonEnemy/Attack/Unarmed/Skeleton_Default_Attack_Unarmed.png");
    loadTexture(hurtTexture,          "assets/textures/SkeletonEnemy/Hurt/Skeleton_Default_Hurt.png");
}

void EnemyManager::reset() {
    enemies.clear();
    spawnTimer = 0.f;
    pendingSpawns = maxEnemies; // spawn all enemies quickly at game start
}

void EnemyManager::spawnRandom(sf::Vector2f playerWorldPos) {
    std::uniform_real_distribution<float> offsetDist(400.f, 750.f);
    std::uniform_int_distribution<int> sideDist(0, 1);
    std::uniform_int_distribution<int> typeDist(0, 2); // 0 = unarmed, 1-2 = sword (2:1 ratio)

    float offset = offsetDist(rng);
    if (sideDist(rng) == 0) offset = -offset; // spawn left or right

    float spawnX = playerWorldPos.x + offset;
    float spawnY = 100.f;

    EnemyType type = (typeDist(rng) == 0) ? EnemyType::SkeletonUnarmed : EnemyType::SkeletonSword;

    EnemyTextures tex;
    EnemyStats stats;

    switch (type) {
        case EnemyType::SkeletonSword:
            tex   = {&swordIdleTexture, &swordWalkTexture, &swordAttackTexture, &hurtTexture};
            stats = makeSwordStats();
            break;
        case EnemyType::SkeletonUnarmed:
            tex   = {&unarmedIdleTexture, &unarmedWalkTexture, &unarmedAttackTexture, &hurtTexture};
            stats = makeUnarmedStats();
            break;
    }

    enemies.emplace_back(tex, stats, sf::Vector2f{spawnX, spawnY});
}

void EnemyManager::update(float dt, Player& player, sf::Vector2f playerWorldFeet, sf::Vector2f playerWorldCenter) {
    for (auto& enemy : enemies)
        enemy.update(dt, playerWorldFeet);

    for (auto& enemy : enemies) {
        if (enemy.shouldDealDamage()) {
            player.applyDamage(enemy.getDamage());
            enemy.markDamageDealt();
        }
    }

    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& e) { return !e.getIsAlive(); }),
        enemies.end()
    );

    if ((int)enemies.size() < maxEnemies) {
        if (pendingSpawns > 0) {
            spawnRandom(playerWorldFeet);
            pendingSpawns--;
        } else {
            spawnTimer += dt;
            if (spawnTimer >= spawnInterval) {
                spawnTimer = 0.f;
                spawnRandom(playerWorldFeet);
            }
        }
    } else {
        spawnTimer = 0.f;
    }
}

void EnemyManager::draw(sf::RenderWindow& window, float levelOffset) {
    for (auto& enemy : enemies) {
        sf::Sprite sprite = enemy.getSprite();
        sf::Vector2f pos = enemy.getPosition();
        sprite.setPosition({pos.x - levelOffset, pos.y});
        window.draw(sprite);
    }
}

std::vector<Enemy>& EnemyManager::getEnemies() {
    return enemies;
}

static bool intersects(const Hitbox& a, const Hitbox& b) {
    return a.x < b.x + b.width  &&
           a.x + a.width  > b.x &&
           a.y < b.y + b.height &&
           a.y + a.height > b.y;
}

void EnemyManager::handlePlayerAttack(Player& player, float levelOffset) {
    if (!player.canDealAttackDamage()) return;

    Hitbox attackHitbox = player.getAttackHitbox(levelOffset);

    for (auto& enemy : enemies) {
        if (!enemy.getIsAlive()) continue;
        if (intersects(attackHitbox, enemy.getBodyHitbox())) {
            enemy.takeDamage(1);
            player.markAttackDamageDealt();
            break;
        }
    }
}
