#include "menu.hpp"

Menu::Menu(const std::vector<std::string>& buttonPaths,
           const std::string& backgroundPath,
           int bgFrameIndex,
           float startY,
           float spacingY):
        buttonSwitchBuffer("assets/sounds/buttonSwitch.wav"),
        buttonSwitch(buttonSwitchBuffer),
        buttonPickBuffer("assets/sounds/buttonClick.wav"),
        buttonPick(buttonPickBuffer),
        menuTexture(backgroundPath),
        menuSprite(menuTexture)
{
    int menuWidth = menuTexture.getSize().x;
    int menuHeight = menuTexture.getSize().y;
    int menuHalfWidth = menuWidth / 2;

    menuSprite.setTextureRect(sf::IntRect({menuHalfWidth * bgFrameIndex, 0}, {menuHalfWidth, menuHeight}));
    menuSprite.setScale({scale, scale});
    menuSprite.setPosition({-160.f, -100.f});

    int count = buttonPaths.size();
    textures.resize(count);

    for (int i = 0; i < count; i++) {
        (void)textures[i].loadFromFile(buttonPaths[i]);
        buttons.emplace_back(textures[i]);

        buttons[i].setScale({scale, scale});
        buttons[i].setPosition({-80.f, static_cast<float>(i * spacingY + startY)});
    }

    if (!buttons.empty()) {
        buttons[0].setColor(sf::Color::Green);
    }

    buttonSwitch.setVolume(50.f);
    buttonPick.setVolume(25.f);
}

void Menu::draw(sf::RenderWindow& window) {
    window.draw(menuSprite);
    for (auto& button : buttons) {
        window.draw(button);
    }
}

int Menu::getSelectedIndex() {
    return selectedIndex;
}

void Menu::handleInput(const sf::RenderWindow &window) {
    bool upPressed = (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up) or sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W));
    bool downPressed = (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down) or sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S));

    if (downPressed && !wasDownPressed) {
        selectedIndex = (selectedIndex + 1) % static_cast<int>(buttons.size());
        buttonSwitch.play();
    }

    if (upPressed && !wasUpPressed) {
        selectedIndex--;
        buttonSwitch.play();
        if (selectedIndex < 0) selectedIndex = static_cast<int>(buttons.size()) - 1;
    }

    for (int i = 0; i < static_cast<int>(buttons.size()); i++) {
        if (i == selectedIndex) {
            buttons[i].setColor(sf::Color::Green);
        } else {
            buttons[i].setColor(sf::Color::White);
        }
    }

    wasUpPressed = upPressed;
    wasDownPressed = downPressed;
}

void Menu::ButtonPicker() {
    buttonPick.play();
}