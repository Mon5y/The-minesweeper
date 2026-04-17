#pragma once

#include <tmxlite/Map.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

class TMXLoader {
public:
    TMXLoader();
    
    bool loadMap(const std::string& path);
    void updateAnimations(float dt);
    void draw(sf::RenderWindow& window);
    
    const std::string& getLastError() const { return mLastError; }

private:
    std::unique_ptr<tmx::Map> mMap;
    std::string mLastError;
    sf::RectangleShape mDebugShape;
};