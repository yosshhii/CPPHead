#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "menu.hpp"

class Settings {
private:
    Menu menu;

public:
    Settings(const std::vector<std::string>& buttonPaths, const std::string& backgroundPath, int bgFrameIndex);

    void draw(sf::RenderWindow& window);
    void handleInput(const sf::RenderWindow& window);

    int getSelectedIndex();
    void ButtonPicker();
};