#include "menu.hpp"

Menu::Menu() {
    textures.resize(3);

    (void)textures[0].loadFromFile("assets/textures/StartButton/Start1.png");
    (void)textures[1].loadFromFile("assets/textures/SettingsButton/Settings1.png");
    (void)textures[2].loadFromFile("assets/textures/QuitButton/Quit1.png");

    buttons.emplace_back(textures[0]);
    buttons.emplace_back(textures[1]);
    buttons.emplace_back(textures[2]);

    for (int i = 0; i < 3; i++) {
        buttons[i].setScale({scale, scale});
        buttons[i].setPosition({-80.f, static_cast<float>(i * 70)});
    }

    buttons[0].setColor(sf::Color::Green);
}

void Menu::draw(sf::RenderWindow& window) {
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
    }

    if (upPressed && !wasUpPressed) {
        selectedIndex--;
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
