#include "draw.hpp"

void drawGame(sf::RenderWindow& window, Background& background, Level& level, Player& player, EnemyManager& enemyManager, float bgOffset) {
    window.clear();

    background.draw(window);
    level.draw(window);
    player.draw(window);
    enemyManager.draw(window, bgOffset);

    window.display();
}

void drawMenu(sf::RenderWindow& window, Background& background, Level& level, Menu& menu) {
    window.clear();

    background.draw(window);
    menu.draw(window);

    window.display();
}

void drawSettings(sf::RenderWindow& window, Background& background, Level& level, Settings& settings) {
    window.clear();

    background.draw(window);
    settings.draw(window);

    window.display();
}

void drawPauseMenu(sf::RenderWindow& window, Background& background, Level& level, Player& player, Settings& pauseMenu) {
    window.clear();

    background.draw(window);
    level.draw(window);
    player.draw(window);

    sf::View currentView = window.getView();

    sf::RectangleShape darkOverlay(currentView.getSize());

    darkOverlay.setOrigin({currentView.getSize().x / 2.0f, currentView.getSize().y / 2.0f});

    darkOverlay.setPosition(currentView.getCenter());

    darkOverlay.setFillColor(sf::Color(0, 0, 0, 170));
    window.draw(darkOverlay);

    pauseMenu.draw(window);

    window.display();
}

void drawGameOver(sf::RenderWindow& window, Background& background, Level& level, Player& player, Menu& gameOverMenu) {
    window.clear();

    background.draw(window);
    level.draw(window);
    player.draw(window);

    sf::View currentView = window.getView();
    sf::RectangleShape redOverlay(currentView.getSize());
    redOverlay.setOrigin({currentView.getSize().x / 2.0f, currentView.getSize().y / 2.0f});
    redOverlay.setPosition(currentView.getCenter());

    redOverlay.setFillColor(sf::Color(150, 0, 0, 180));
    window.draw(redOverlay);

    gameOverMenu.draw(window);

    window.display();
}