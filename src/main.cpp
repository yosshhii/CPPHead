#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({1080,1080}), "CPPHead");
    window.setView(sf::View{ {}, static_cast<sf::Vector2f>(window.getSize())});

    sf::CircleShape shape{50};
    shape.setOrigin({shape.getRadius(),shape.getRadius()});

    sf::Texture texture("assets/textures/Owlet_Monster.png");
    sf::Sprite sprite{texture};
    sprite.setScale({3,2});

    sf::Clock clock{};
    float speed = 200;

    bool isRunning = true;
    while (isRunning) {
        auto elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {isRunning = false;}
            if (event->is<sf::Event::Resized>()) {window.setView(sf::View{ {}, static_cast<sf::Vector2f>(window.getSize())});}
        }


        sf::Vector2f movement{};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {movement.x += 1;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {movement.x -= 1;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {movement.y -= 1;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {movement.y += 1;}

        sprite.setPosition(sprite.getPosition()+movement * speed * dt );

        window.clear();
        window.draw(sprite);
        window.display();
    }
    return 0;
}