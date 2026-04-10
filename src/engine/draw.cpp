#include "draw.hpp"

#include "../game/background.hpp"
#include "../game/player.hpp"
#include "../game/level.hpp"
#include "../game/menu.hpp"

void drawGame(sf::RenderWindow& window, Background& background, Level& level, Player& player) {
    window.clear();

    background.draw(window);
    level.draw(window);
    player.draw(window);

    window.display();
}

void drawMenu(sf::RenderWindow& window, Background& background, Level& level, Menu& menu) {
    window.clear();

    background.draw(window);
    level.draw(window);
    menu.draw(window);

    window.display();
}