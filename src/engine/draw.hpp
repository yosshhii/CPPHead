#pragma once

#include <SFML/Graphics.hpp>

class Player;
class Background;
class Level;
class Menu;
class Settings;

void drawGame(sf::RenderWindow& window, Background& background, Level& level, Player& player);
void drawMenu(sf::RenderWindow& window, Background& background, Level& level, Menu& menu);
void drawSettings(sf::RenderWindow& window, Background& background, Level& level, Settings& settings);