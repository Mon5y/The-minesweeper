#include "Board.hpp"
#include <string>

Board::Board(int w, int h, float cSize, float offX, float offY)
    : width(w), height(h), cellSize(cSize), ofset(offX, offY) {}

sf::Vector2i Board::getCellFromMouse(int mouseX, int mouseY)
{
    int cellX = static_cast<int>((mouseX - ofset.x) / cellSize);
    int cellY = static_cast<int>((mouseY - ofset.y) / cellSize);
    return sf::Vector2i(cellX, cellY);
}

void Board::draw(sf::RenderWindow& window, const GameBoard& gameboard, sf::Font& font)
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float posX = ofset.x + x * cellSize;
            float posY = ofset.y + y * cellSize;

            sf::RectangleShape tile(sf::Vector2f(cellSize - 2.f, cellSize - 2.f));
            tile.setPosition({ posX, posY });

            Cell::State state = gameboard.getCell(x, y).getState();

            if (state == Cell::Hidden) {
                tile.setFillColor(sf::Color(100, 100, 100));
            }
            else if (state == Cell::Flagged) {
                tile.setFillColor(sf::Color(200, 150, 50));
            }
            else if (state == Cell::Exploded) {
                tile.setFillColor(sf::Color(0, 255, 0));
            }
            else if (state == Cell::Opened) {
                if (gameboard.getCell(x, y).hasMine()) {
                    tile.setFillColor(sf::Color(200, 50, 50));
                } else {
                    tile.setFillColor(sf::Color(180, 180, 180));
                }
            }

            window.draw(tile);

            if (state == Cell::Opened && !gameboard.getCell(x, y).hasMine())
            {
                int count = gameboard.getCell(x, y).getNeightborCount();
                if (count > 0)
                {
                    sf::Text countText;
                    countText.setFont(font);
                    countText.setString(std::to_string(count));
                    countText.setCharacterSize(18);
                    countText.setFillColor(sf::Color::Blue);

                    float textX = posX + (cellSize - countText.getGlobalBounds().width) / 2.f;
                    float textY = posY + (cellSize - countText.getGlobalBounds().height) / 2.f - 4.f;

                    countText.setPosition({ textX, textY });
                    window.draw(countText);
                }
            }
        }
    }
}