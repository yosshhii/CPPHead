#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include <vector>
#include <string>
#include <map>
#include <random>

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
    std::mt19937 rng{std::random_device{}()};

    std::map<std::string, std::vector<std::string>> rules = {
        {"Sum_bottom", {"Sum_up"}},
        {"Sum_up",     {"Sum_top"}},
        {"Sum_top",    {"Sum_down"}},
        {"Sum_down",   {"Sum_island", "Sum_bottom"}},
        {"Sum_island", {"Sum_up", "Sum_island"}}
    };

    std::string getNextType(std::string last) {
        const auto& candidates = rules[last];
        std::uniform_int_distribution<std::size_t> dist(0, candidates.size() - 1);
        return candidates[dist(rng)];
    }

public:
    void init() {
        activeBlocks.clear();
        currentX = 0;

        std::vector<std::string> types = {"Sum_bottom", "Sum_up", "Sum_top", "Sum_down", "Sum_island"};
        for (const auto& t : types) {
            textures[t].loadFromFile("assets/textures/floors/" + t + ".png");
            masks[t].loadFromFile("assets/textures/floors/" + t + "_mask.png");
        }
        spawnBlock("Sum_bottom");
    }

    void spawnBlock(std::string type) {
        activeBlocks.emplace_back(textures[type], masks[type], currentX, type);
        currentX += (float)textures[type].getSize().x;
    }

    void update(float dt, float windowWidth, float speed) {
        float offset = speed * dt;
        currentX -= offset;

        for (auto& b : activeBlocks) {
            b.posX -= offset;
            b.visual.setPosition({b.posX, 0.f});
        }

        while (!activeBlocks.empty() && (activeBlocks.front().posX + 100.f) < 0) {
            activeBlocks.pop_front();
        }
        while (currentX < windowWidth + 200.f) {
            std::string lastType = activeBlocks.empty() ? "Sum_bottom" : activeBlocks.back().type;
            spawnBlock(getNextType(lastType));
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
            window.draw(b.visual);
        }
    }
};
