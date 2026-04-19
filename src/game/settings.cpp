#include "settings.hpp"

Settings::Settings(const std::vector<std::string>& buttonPaths, const std::string& backgroundPath, int bgFrameIndex)
        : menu(buttonPaths, backgroundPath, bgFrameIndex)
{
}

void Settings::draw(sf::RenderWindow& window) {
    menu.draw(window);
}

void Settings::handleInput(const sf::RenderWindow &window) {
    menu.handleInput(window);
}

int Settings::getSelectedIndex() {
    return menu.getSelectedIndex();
}

void Settings::ButtonPicker() {
    menu.ButtonPicker();
}