#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>
#include <random>
#include <iostream>

struct Block {
    sf::Sprite visual;
    sf::Image mask;
    float posX;
    std::string type;

    Block(const sf::Texture& tex, const sf::Image& m, float x, std::string t)
        : visual(tex), mask(m), posX(x), type(t) {
        visual.setScale({1.0f, 1.0f});
        visual.setPosition({posX, 0.f});
    }

    Block(Block&&) noexcept = default;
    Block& operator=(Block&&) noexcept = default;
};

class LevelManager {
private:
    std::map<float, Block> worldBlocks;
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Image> masks;
    float worldOffset = 0; // Насколько мы сдвинулись
    float lastGeneratedRightX = 0;
    float lastGeneratedLeftX = 0;
    std::mt19937 rng{std::random_device{}()};

    std::map<std::string, std::vector<std::string>> rules = {
        {"bottom", {"up", "island", "bottom"}},
        {"up",     {"top"}},
        {"top",    {"down", "top"}},
        {"down",   {"island", "bottom"}},
        {"island", {"up", "island", "bottom"}}
    };

    std::map<std::string, std::vector<std::string>> reverseRules = {
        {"up",     {"bottom", "island"}},
        {"top",    {"up", "top"}},
        {"down",   {"top"}},
        {"island", {"down", "island", "bottom"}},
        {"bottom", {"down", "bottom", "island"}}
    };

    std::string getNextType(std::string last) {
        const auto& candidates = rules[last];
        std::uniform_int_distribution<std::size_t> dist(0, candidates.size() - 1);
        return candidates[dist(rng)];
    }

    std::string getPreviousType(std::string first) {
        if (reverseRules.find(first) == reverseRules.end()) return "bottom";
        const auto& candidates = reverseRules[first];
        std::uniform_int_distribution<std::size_t> dist(0, candidates.size() - 1);
        return candidates[dist(rng)];
    }

    std::string getLastType() {
        if (worldBlocks.empty()) return "bottom";
        return worldBlocks.rbegin()->second.type;
    }

    std::string getFirstType() {
        if (worldBlocks.empty()) return "bottom";
        return worldBlocks.begin()->second.type;
    }

public:
    void init() {
        worldBlocks.clear();
        worldOffset = 0;
        lastGeneratedRightX = 0;
        lastGeneratedLeftX = 0;

        std::vector<std::string> types = {"bottom", "up", "top", "down", "island"};
        for (const auto& t : types) {
            std::string texPath = "assets/textures/floors/Sum_" + t + ".png";
            if (!textures[t].loadFromFile(texPath)) {
                std::cerr << "ERROR: Failed to load texture: " << texPath << std::endl;
            }
            std::string maskPath = "assets/textures/floors/" + t + "_mask.png";
            if (!masks[t].loadFromFile(maskPath)) {
                std::cerr << "ERROR: Failed to load mask: " << maskPath << std::endl;
            }
        }
        spawnBlockAt(0, "bottom");
    }

    void spawnBlockAt(float x, std::string type) {
        if (worldBlocks.find(x) == worldBlocks.end()) {
            worldBlocks.emplace(std::piecewise_construct,
                               std::forward_as_tuple(x),
                               std::forward_as_tuple(textures[type], masks[type], x, type));
        }
    }

    void update(float dt, float windowWidth, float speed) {
        worldOffset += speed * dt;

        float viewMargin = 1000.f;

        while (lastGeneratedRightX < worldOffset + windowWidth + viewMargin) {
            std::string next = getNextType(getLastType());
            float width = (float)textures[next].getSize().x;
            if (width <= 0) width = 150.f;
            lastGeneratedRightX += width;
            spawnBlockAt(lastGeneratedRightX, next);
        }

        while (lastGeneratedLeftX > worldOffset - viewMargin) {
            std::string prev = getPreviousType(getFirstType());
            float width = (float)textures[prev].getSize().x;
            if (width <= 0) width = 150.f;
            lastGeneratedLeftX -= width;
            spawnBlockAt(lastGeneratedLeftX, prev);
        }
    }

    bool checkCollision(sf::Vector2f pos) {
        float worldPosX = pos.x + worldOffset;

        for (const auto& [x, b] : worldBlocks) {
            float width = 150.f;
            if (worldPosX >= b.posX && worldPosX < b.posX + width) {
                float localX = worldPosX - b.posX;
                float localY = pos.y;

                if (localY >= 0 && localY < 384.f) {
                    if ((unsigned int)localX < b.mask.getSize().x) {
                        sf::Color color = b.mask.getPixel({(unsigned int)localX, (unsigned int)localY});
                        if (color.r < 50 && color.a > 200) return true;
                    }
                }
            }
        }
        return false;
    }

    bool checkCollisionWorld(sf::Vector2f worldPos) {
        float worldPosX = worldPos.x;

        for (const auto& [x, b] : worldBlocks) {
            float width = static_cast<float>(b.mask.getSize().x);

            if (worldPosX >= b.posX && worldPosX < b.posX + width) {
                float localX = worldPosX - b.posX;
                float localY = worldPos.y;

                if (localX >= 0 &&
                    localY >= 0 &&
                    localX < static_cast<float>(b.mask.getSize().x) &&
                    localY < static_cast<float>(b.mask.getSize().y)) {

                    sf::Color color = b.mask.getPixel({
                        static_cast<unsigned int>(localX),
                        static_cast<unsigned int>(localY)
                    });

                    return color.r < 50 && color.a > 200;
                    }
            }
        }

        return false;
    }

    void draw(sf::RenderWindow& window) {
        for (auto& [x, b] : worldBlocks) {
            float screenX = b.posX - worldOffset;
            if (screenX > -800.f && screenX < 700.f) {
                b.visual.setPosition({screenX, 0.f});
                window.draw(b.visual);
            }
        }
    }

    float getWorldOffset() const {
        return worldOffset;
    }
};
