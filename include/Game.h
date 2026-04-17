#pragma once

#include <SFML/Graphics.hpp>
#include "TMXLoader.h"

class Game {
public:
    Game();
    void run(); // Объявление метода

private:
    void processEvents();
    void update(float dt);
    void render();

    sf::RenderWindow mWindow;
    TMXLoader mMapLoader;
};