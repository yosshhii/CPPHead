#pragma once
#include <SFML/Graphics.hpp>

sf::RenderWindow createWindow();


void handleWindowEvents(sf::RenderWindow& window, bool& isRunning);