#include "settings.hpp"
#include <iostream>

Settings::Settings(const std::vector<std::string>& buttonPaths,
                   const std::string& backgroundPath,
                   int bgFrameIndex,
                   float startY,
                   float spacingY,
                   const std::string& trackPath,
                   const std::string& knobPath,
                   float volumeStartY)
        : menu(buttonPaths, backgroundPath, bgFrameIndex, startY, spacingY),
          trackTexture(trackPath),
          trackSprite(trackTexture),
          knobTexture(knobPath),
          knobSprite(knobTexture)
{
    float trackX = -80.f;

    trackSprite.setScale({trackScale, trackScale});
    knobSprite.setScale({knobScale, knobScale});

    sf::FloatRect kBounds = knobSprite.getLocalBounds();
    knobSprite.setOrigin({kBounds.size.x / 2.0f, kBounds.size.y / 2.0f});

    trackSprite.setPosition({trackX, volumeStartY});

    float tWidth = trackSprite.getGlobalBounds().size.x;
    float tHeight = trackSprite.getGlobalBounds().size.y;

    float visualPaddingLeft = 20.0f;
    float visualPaddingRight = 18.0f;

    knobMinX = trackX + visualPaddingLeft;
    float knobMaxX = trackX + tWidth - visualPaddingRight;

    trackTravel = knobMaxX - knobMinX;

    float knobY = volumeStartY + (tHeight / 2.0f);

    float startKnobX = knobMinX + (trackTravel * (currentVolume / 100.f));
    knobSprite.setPosition({startKnobX, knobY});

    sf::Listener::setGlobalVolume(currentVolume);
}

void Settings::draw(sf::RenderWindow& window) {
    menu.draw(window);

    window.draw(trackSprite);
    window.draw(knobSprite);
}

void Settings::handleInput(const sf::RenderWindow &window) {
    menu.handleInput(window);

    if (menu.getSelectedIndex() == 0) {
        bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D);
        bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A);

        if (right && currentVolume < 100.f) {
            currentVolume += 0.5f;
            if (currentVolume > 100.f) currentVolume = 100.f;
        }
        if (left && currentVolume > 0.f) {
            currentVolume -= 0.5f;
            if (currentVolume < 0.f) currentVolume = 0.f;
        }

        sf::Listener::setGlobalVolume(currentVolume);

        float newKnobX = knobMinX + (trackTravel * (currentVolume / 100.f));
        float currentKnobY = knobSprite.getPosition().y;

        knobSprite.setPosition({newKnobX, currentKnobY});
    }
}
int Settings::getSelectedIndex() {
    return menu.getSelectedIndex();
}

void Settings::ButtonPicker() {
    menu.ButtonPicker();
}