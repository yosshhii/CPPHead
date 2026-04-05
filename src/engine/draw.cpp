#include "draw.hpp"

#include "../game/background.hpp"
#include "../game/player.hpp"
#include "../game/level.hpp"

void draw(sf::RenderWindow& window, Background& background, Level& level, Player& player) {
    window.clear();

    background.draw(window);
    level.draw(window);
    player.draw(window);

    window.display();
}