#pragma once

#include <SFML/Graphics.hpp>

class Player;
class Background;
class Level;

void draw(sf::RenderWindow& window, Background& background, Level& level, Player& player);