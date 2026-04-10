#pragma once

#include <SFML/Graphics.hpp>

class Player;
class Background;
class Level;
class Menu;

void drawGame(sf::RenderWindow& window, Background& background, Level& level, Player& player);
void drawMenu(sf::RenderWindow& window, Background& background, Level& level, Menu& menu);