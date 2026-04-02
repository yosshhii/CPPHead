#include <iostream>
#include <SFML/Graphics.hpp>

#include "engine/window.hpp"

int main() {
    sf::RenderWindow window = createWindow();

    int frameWidth = 32;
    int frameHeight = 32;

    sf::Texture playerTexture("assets/textures/Owlet_Monster_Walk_6.png");
    sf::Texture playerJumpTexture("assets/textures/Owlet_Monster_Jump_8.png");
    sf::Sprite playerSprite{playerTexture};
    playerSprite.setScale({3,2});

    playerSprite.setOrigin({
    static_cast<float>(frameWidth) / 2.f,
    static_cast<float>(frameHeight)
});
    playerSprite.setPosition({0, 200 });


    sf::Texture background("assets/textures/Hills.psd");
    sf::Sprite backgroundSprite1{background};
    sf::Sprite backgroundSprite2{background};
    backgroundSprite1.setScale({2.5,2.5});
    backgroundSprite2.setScale({2.5,2.5});

    float backgroundWidth = background.getSize().x * backgroundSprite1.getScale().x;
    float backgroundHeight = background.getSize().y * backgroundSprite1.getScale().y;
    backgroundSprite1.setPosition({0,-backgroundHeight/2 - 100});
    backgroundSprite2.setPosition({-backgroundWidth,-backgroundHeight/2 - 100});

    sf::Clock clock{};
    float speed = 200;

    int currentFrame = 0;
    float animationTimer = 0.f;
    float animationSpeed = 0.12f;

    float velocityY = 0.f;
    float gravity = 1500.f;
    float jumpForce = -600.f;

    bool isOnGround = true;

    bool isRunning = true;
    while (isRunning) {
        auto elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {isRunning = false;}
            if (event->is<sf::Event::Resized>()) {window.setView(sf::View{ {}, static_cast<sf::Vector2f>(window.getSize())});}
        }

        sf::Vector2f movement{};
        if (window.hasFocus()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) ) {movement.x += 1; playerSprite.setScale({3.f,2.f});}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {movement.x -= 1; playerSprite.setScale({-3.f, 2.f});}

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space) && isOnGround) {
                velocityY = jumpForce;
                isOnGround = false;

                currentFrame = 0;
                animationTimer = 0.f;
            }

            if (!isOnGround) {
                playerSprite.setTexture(playerJumpTexture);
            } else {
                playerSprite.setTexture(playerTexture);
            }

            velocityY += gravity * dt;
            playerSprite.move({0.f, velocityY * dt});

            float groundY = 200.f;

            if (playerSprite.getPosition().y >= groundY) {
                playerSprite.setPosition({playerSprite.getPosition().x, groundY});
                velocityY = 0.f;
                isOnGround = true;
            }
        }

        bool isMoving = (movement.x != 0 || movement.y != 0);

        if (isMoving && isOnGround) {
            animationTimer += dt;
            if (animationTimer >= animationSpeed) {
                animationTimer = 0.f;
                currentFrame = (currentFrame + 1) % 6;
                playerSprite.setTextureRect(sf::IntRect({currentFrame * frameWidth, 0}, {frameWidth, frameHeight}));
            }
        }

        if (!isMoving && isOnGround) {
            playerSprite.setTextureRect(sf::IntRect({5 * frameWidth, 0}, {frameWidth, frameHeight}));
        }

        backgroundSprite1.setPosition(backgroundSprite1.getPosition() - movement * speed * dt);
        backgroundSprite2.setPosition({backgroundSprite2.getPosition() - movement * speed * dt});

        float rightLimit = window.getSize().x / 2.f;

        if (backgroundSprite1.getPosition().x + backgroundWidth <= -1060) {
            backgroundSprite1.setPosition({backgroundSprite2.getPosition().x + backgroundWidth, -backgroundHeight/2 - 100 });
        }
        if (backgroundSprite2.getPosition().x + backgroundWidth <= -1060) {
            backgroundSprite2.setPosition({backgroundSprite1.getPosition().x + backgroundWidth, -backgroundHeight/2 - 100});
        }
        if (backgroundSprite1.getPosition().x >= rightLimit) {
            backgroundSprite1.setPosition({backgroundSprite2.getPosition().x - backgroundWidth, -backgroundHeight/2 - 100 });
        }
        if (backgroundSprite2.getPosition().x >= rightLimit) {
            backgroundSprite2.setPosition({backgroundSprite1.getPosition().x - backgroundWidth, -backgroundHeight/2 - 100});
        }

        window.clear();
        window.draw(backgroundSprite1);
        window.draw(backgroundSprite2);
        window.draw(playerSprite);
        window.display();
    }
    return 0;
}
