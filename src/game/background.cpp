#include "background.hpp"

Background::Background():
      texture("assets/textures/Hills.psd"),
      sprite1(texture),
      sprite2(texture) {
}


void Background::init() {
    sprite1.setScale({2.5f, 2.5f});
    sprite2.setScale({2.5f, 2.5f});

    width = texture.getSize().x * sprite1.getScale().x;
    height = texture.getSize().y * sprite1.getScale().y;

    sprite1.setPosition({0.f, -height / 2.f - 100.f});
    sprite2.setPosition({-width, -height / 2.f - 100.f});
}

void Background::update(const sf::Vector2f& movement, float dt, const sf::RenderWindow& window) {
    float speed = 200.f;

    sprite1.setPosition(sprite1.getPosition() - movement * speed * dt);
    sprite2.setPosition(sprite2.getPosition() - movement * speed * dt);

    float rightLimit = window.getSize().x / 2.f;

    if (sprite1.getPosition().x + width <= -1060.f) {
        sprite1.setPosition({sprite2.getPosition().x + width, -height / 2.f - 100.f});
    }
    if (sprite2.getPosition().x + width <= -1060.f) {
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