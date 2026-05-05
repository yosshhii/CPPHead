#pragma once

#include <SFML/Graphics.hpp>

#include "../game/background.hpp"
#include "../game/player.hpp"
#include "../game/level.hpp"
#include "../game/menu.hpp"
#include "../game/settings.hpp"
#include "../game/enemyManager.hpp"

class Player;
class Background;
class Level;
class Menu;
class Settings;
class Enemy;
class EnemyManager;

void drawGame(sf::RenderWindow& window, Background& background, Level& level, Player& player, EnemyManager& enemyManager, float bgOffset);
void drawMenu(sf::RenderWindow& window, Background& background, Level& level, Menu& menu);
void drawSettings(sf::RenderWindow& window, Background& background, Level& level, Settings& settings);
void drawPauseMenu(sf::RenderWindow& window, Background& background, Level& level, Player& player, Settings& pauseMenu);
void drawGameOver(sf::RenderWindow& window, Background& background, Level& level, Player& player, Menu& gameOverMenu);
