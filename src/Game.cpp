#include "Game.h"
#include <stdexcept>
#include <iostream>
#include <filesystem>

Game::Game() : mWindow(sf::VideoMode(800, 600), "Undertale-like Game") {
    mWindow.setFramerateLimit(60);
    
    // Путь к карте
    std::string mapPath = "assets/maps/level1.tmx";
    
    // Получение абсолютного пути для диагностики
    std::filesystem::path fullPath = std::filesystem::absolute(mapPath);
    std::cout << "Loading map from: " << fullPath.string() << std::endl;
    
    if (!mMapLoader.loadMap(mapPath)) {
        std::string errorMsg = "Failed to load map: " + mMapLoader.getLastError();
        std::cerr << errorMsg << std::endl;
        throw std::runtime_error(errorMsg);
    }
    std::cout << "Map loaded successfully!" << std::endl;
}

void Game::run() {
    sf::Clock clock;
    while (mWindow.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed || 
           (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
            mWindow.close();
        }
    }
}

void Game::update(float dt) {
    mMapLoader.updateAnimations(dt);
}

void Game::render() {
    mWindow.clear();
    mMapLoader.draw(mWindow);
    mWindow.display();
}