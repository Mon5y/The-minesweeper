#pragma once
#include <SFML/Graphics.hpp>
#include "GameBoard.hpp"

class Board
{
private:
    int width;
    int height;
    float cellSize;
    sf::Vector2f ofset;

public:
    Board(int w, int h, float cSize, float offX, float offY);

    sf::Vector2i getCellFromMouse(int mouseX, int mouseY);
    void draw(sf::RenderWindow& window, const GameBoard& gameboard, sf::Font& font);
};