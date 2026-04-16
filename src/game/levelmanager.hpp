#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
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
    std::deque<Block> activeBlocks;
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Image> masks;
    float currentX = 0;
    float leftX = 0;
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

public:
    void init() {
        activeBlocks.clear();
        currentX = 0;
        leftX = 0;

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
        spawnBlock("bottom");
    }

    void spawnBlock(std::string type) {
        float width = (float)textures[type].getSize().x;
        if (width <= 0) {
            width = 100.f;
        }
        activeBlocks.emplace_back(textures[type], masks[type], currentX, type);
        currentX += width;
    }

    void spawnBlockLeft(std::string type) {
        float width = (float)textures[type].getSize().x;
        if (width <= 0) {
            width = 100.f;
        }
        leftX -= width;
        activeBlocks.emplace_front(textures[type], masks[type], leftX, type);
    }


    void update(float dt, float windowWidth, float speed) {
        float offset = speed * dt;
        currentX -= offset;
        leftX -= offset;

        for (auto& b : activeBlocks) {
            b.posX -= offset;
            b.visual.setPosition({b.posX, 0.f});
        }

        while (!activeBlocks.empty() && activeBlocks.back().posX > 800.f) {
            currentX -= (float)activeBlocks.back().mask.getSize().x;
            activeBlocks.pop_back();
        }
        while (currentX < 600.f) {
            std::string lastType = activeBlocks.empty() ? "bottom" : activeBlocks.back().type;
            spawnBlock(getNextType(lastType));
        }

        while (!activeBlocks.empty() && activeBlocks.front().posX < -800.f) {
            leftX += (float)activeBlocks.front().mask.getSize().x;
            activeBlocks.pop_front();
        }
        while (leftX > -600.f) {
            std::string firstType = activeBlocks.empty() ? "bottom" : activeBlocks.front().type;
            spawnBlockLeft(getPreviousType(firstType));
        }
    }

    bool checkCollision(sf::Vector2f pos) {
        for (const auto& b : activeBlocks) {
            if (pos.x >= b.posX && pos.x < b.posX + 100.f) {
                float localX = pos.x - b.posX;
                float localY = pos.y;

                if (localY >= 0 && localY < 384.f) {
                    sf::Color color = b.mask.getPixel({(unsigned int)localX, (unsigned int)localY});
                    if (color.r < 50 && color.a > 200) return true;
                }
            }
        }
        return false;
    }


    void draw(sf::RenderWindow& window) {
        for (const auto& b : activeBlocks) {
            std::cout << "Active blocks: " << activeBlocks.size() << std::endl;

            window.draw(b.visual);
        }
    }
};
