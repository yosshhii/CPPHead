#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>

class Menu {
private:
    std::vector<sf::Sprite> buttons;
    std::vector<sf::Texture> textures;

    sf::Texture menuTexture;
    sf::Sprite menuSprite;

    sf::SoundBuffer buttonSwitchBuffer;
    sf::SoundBuffer buttonPickBuffer;
    sf::Sound buttonSwitch;
    sf::Sound buttonPick;

    float scale = 2.5f;

    int selectedIndex = 0;

    bool wasUpPressed = false;
    bool wasDownPressed = false;
public:
    Menu(const std::vector<std::string>& buttonPaths, const std::string& backgroundPath);

    void handleInput(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    int getSelectedIndex();
    void ButtonPicker();
};
