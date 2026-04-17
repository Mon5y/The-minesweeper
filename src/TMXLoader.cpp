#include "TMXLoader.h"
#include <iostream>
#include <fstream>
#include <filesystem>

// Внешние объявления для miniz (без заголовочного файла)
extern "C" {
    int mz_inflateInit(void* strm);
    int mz_inflate(void* strm, int flush);
    int mz_inflateEnd(void* strm);
}

TMXLoader::TMXLoader() 
    : mMap(std::make_unique<tmx::Map>()),
      mLastError(""),
      mDebugShape(sf::Vector2f(100, 100))
{
    mDebugShape.setFillColor(sf::Color::Red);
    mDebugShape.setPosition(50, 50);
}

bool TMXLoader::loadMap(const std::string& path) {
    mLastError.clear();
    
    // Проверка существования файла
    if (!std::filesystem::exists(path)) {
        mLastError = "File does not exist: " + path;
        return false;
    }
    
    // Проверка размера файла
    std::uintmax_t fileSize = std::filesystem::file_size(path);
    if (fileSize == 0) {
        mLastError = "File is empty: " + path;
        return false;
    }
    
    std::cout << "Loading TMX file (" << fileSize << " bytes): " << path << std::endl;
    
    // Попытка загрузки карты
    try {
        if (!mMap->load(path)) {
            mLastError = "tmx::Map failed to load file";
            return false;
        }
    } catch (const std::exception& e) {
        mLastError = std::string("Exception: ") + e.what();
        return false;
    }
    
    // Проверка загруженных данных
    if (mMap->getTileCount().x == 0 || mMap->getTileCount().y == 0) {
        mLastError = "Invalid map size: " + 
                     std::to_string(mMap->getTileCount().x) + "x" + 
                     std::to_string(mMap->getTileCount().y);
        return false;
    }
    
    std::cout << "Map loaded successfully: "
              << mMap->getTileCount().x << "x" << mMap->getTileCount().y << " tiles, "
              << mMap->getTileSize().x << "x" << mMap->getTileSize().y << " pixels\n";
    
    return true;
}

void TMXLoader::updateAnimations(float dt) {
    // Заглушка для анимаций
}

void TMXLoader::draw(sf::RenderWindow& window) {
    // Временная отрисовка для диагностики
    window.draw(mDebugShape);
    
    // TODO: Реальная отрисовка карты
    sf::Font font;
    if (font.loadFromFile("assets/fonts/arial.ttf")) {
        sf::Text text;
        text.setFont(font);
        text.setString("Map loaded: " + 
                       std::to_string(mMap->getTileCount().x) + "x" + 
                       std::to_string(mMap->getTileCount().y));
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        window.draw(text);
    }
}