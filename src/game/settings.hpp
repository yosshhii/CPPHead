#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "menu.hpp"

class Settings {
private:
    Menu menu;

    sf::Texture trackTexture;
    sf::Sprite trackSprite;

    sf::Texture knobTexture;
    sf::Sprite knobSprite;

    float currentVolume = 50.f;

    float trackScale = 2.5f;
    float knobScale = 2.5f;

    float knobMinX;
    float trackTravel;

public:
    Settings(const std::vector<std::string>& buttonPaths,
             const std::string& backgroundPath,
             int bgFrameIndex,
             float startY,
             float spacingY,
             const std::string& trackPath,
             const std::string& knobPath);

    void draw(sf::RenderWindow& window);
    void handleInput(const sf::RenderWindow& window);

    int getSelectedIndex();
    void ButtonPicker();
};