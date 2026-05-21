#include "background.hpp"

Background::Background():
      backgroundTexture("assets/textures/Hills.psd"),
      sprite1(backgroundTexture),
      sprite2(backgroundTexture)
{
    sprite1.setScale({scale, scale});
    sprite2.setScale({scale, scale});

    width = backgroundTexture.getSize().x * sprite1.getScale().x;
    height = backgroundTexture.getSize().y * sprite1.getScale().y;

    sprite1.setPosition({0.f, -height / 2.f - 100.f});
    sprite2.setPosition({-width, -height / 2.f - 100.f});
}

void Background::update(const sf::Vector2f& movement, float dt, const sf::RenderWindow& window, int state) {

    if (state == 1) {
        float speed = 200.f;
        sprite1.setPosition(sprite1.getPosition() - movement * speed * dt);
        sprite2.setPosition(sprite2.getPosition() - movement * speed * dt);
    } else if (state == 0 || state == 2) {
        float speed = 50.f;
        sprite1.setPosition(sprite1.getPosition() - movement * speed * dt);
        sprite2.setPosition(sprite2.getPosition() - movement * speed * dt);
    }


    float rightLimit = window.getSize().x / 2.f;
    float leftLimit = -1060.f;

    if (sprite1.getPosition().x + width <= leftLimit) {
        sprite1.setPosition({sprite2.getPosition().x + width, -height / 2.f - 100.f});
    }
    if (sprite2.getPosition().x + width <= leftLimit) {
        sprite2.setPosition({sprite1.getPosition().x + width, -height / 2.f - 100.f});
    }
    if (sprite1.getPosition().x >= rightLimit) {
        sprite1.setPosition({sprite2.getPosition().x - width, -height / 2.f - 100.f});
    }
    if (sprite2.getPosition().x >= rightLimit) {
        sprite2.setPosition({sprite1.getPosition().x - width, -height / 2.f - 100.f});
    }
}

void Background::draw(sf::RenderWindow& window) {
    window.draw(sprite1);
    window.draw(sprite2);
}

const sf::Sprite &Background::getSprite1() const {
    return sprite1;
}

const sf::Sprite &Background::getSprite2() const {
    return sprite2;
}

float Background::getWidth() const {
    return width;
}

float Background::getHeight() const {
    return height;
}

float Background::getOffsetX() const {
    return sprite1.getPosition().x;
}