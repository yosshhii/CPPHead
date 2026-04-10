#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Menu {
private:
    std::vector<sf::Sprite> buttons;
    std::vector<sf::Texture> textures;

    float scale = 2.5f;

    int selectedIndex = 0;

    bool wasUpPressed = false;
    bool wasDownPressed = false;
public:
    Menu();

    void handleInput(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    int getSelectedIndex();

};
