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
        : visual(tex),
          mask(m),
          posX(x),
          type(t)
    {
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
    float scrollSpeed = 300.0f;
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

        while (!activeBlocks.empty() && (activeBlocks.front().posX + (float)activeBlocks.front().mask.getSize().x) < 0) {
            activeBlocks.pop_front();
        }
        while (currentX < windowWidth + 1000.f) {
            std::string lastType = activeBlocks.empty() ? "Sum_bottom" : activeBlocks.back().type;
            spawnBlock(getNextType(lastType));
        }
    }

    bool checkCollision(sf::Vector2f pos) {
        for (const auto& b : activeBlocks) {
            float localX = pos.x - b.posX;
            if (localX >= 0 && localX < (float)b.mask.getSize().x && pos.y >= 0 && pos.y < (float)b.mask.getSize().y) {
                return b.mask.getPixel({(unsigned int)localX, (unsigned int)pos.y}) == sf::Color::Black;
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
