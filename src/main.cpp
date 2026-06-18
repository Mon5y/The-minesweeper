#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>
#include <random>
#include <ctime>

class Cell
{
public:
    enum State { Hidden, Opened, Flagged };
private:
    bool isMine;
    int NeighborMines;
    State state;
public:
    Cell() : isMine(false), NeighborMines(0), state(Hidden) {}

    void setMine() { isMine = true; }
    bool hasMine() const { return isMine; }

    void setNeighborMines(int count) { NeighborMines = count; }
    int getNeightborCount() const { return NeighborMines; }

    State getState() const { return state; }
    void setState(State s) { state = s; }

    void open() { if (state != Flagged) state = Opened; }
    void ToggleFlag()
    {
        if (state == Hidden) state = Flagged;
        else if (state == Flagged) state = Hidden;
    }
};

class GameBoard
{
private:
    int width;
    int height;
    int mineCount;
    std::vector<std::vector<Cell>> board;

    void countNeighborMines()
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                if (board[y][x].hasMine()) continue;
                int mineAround = 0;
                for (int dx = -1; dx <= 1; ++dx)
                {
                    for (int dy = -1; dy <= 1; ++dy)
                    {
                        if (dx == 0 && dy == 0) continue;
                        int neighborX = x + dx;
                        int neighborY = y + dy;

                        if (valid(neighborX, neighborY) && board[neighborY][neighborX].hasMine())
                        {
                            mineAround++;
                        }
                    }
                }
                board[y][x].setNeighborMines(mineAround);
            }
        }
    }

public:
    GameBoard(int w, int h, int mines) : width(w), height(h), mineCount(mines)
    {
        board.resize(height);
        for (int i = 0; i < height; ++i)
        {
            board[i].resize(width);
        }
    }

    bool valid(int x, int y) const
    {
        if ((x >= width) || (y >= height)) return false;
        if ((x < 0) || (y < 0)) return false;
        return true;
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    const Cell& getCell(int x, int y) const { return board[y][x]; }

    void generateBoard(int StartX, int StartY)
    {
        int placedmines = 0;
        while (placedmines < mineCount)
        {
            int randX = rand() % width;
            int randY = rand() % height;

            if (randX == StartX && randY == StartY)
            {
                continue;
            }
            if (!board[randY][randX].hasMine())
            {
                board[randY][randX].setMine();
                placedmines++;
            }
        }
        countNeighborMines();
    }

    void openCell(int x, int y)
    {
        if (!valid(x, y)) return;
        if (board[y][x].getState() == Cell::Opened || board[y][x].getState() == Cell::Flagged) return;

        board[y][x].open();

        if (board[y][x].hasMine()) return;

        if (board[y][x].getNeightborCount() == 0)
        {
            for (int dx = -1; dx <= 1; ++dx)
            {
                for (int dy = -1; dy <= 1; ++dy)
                {
                    if (dx == 0 && dy == 0) continue;

                    int NeighborX = x + dx;
                    int NeighborY = y + dy;

                    openCell(NeighborX, NeighborY);
                }
            }
        }
    }

    void toggleFlag(int x, int y)
    {
        if (!valid(x, y)) return;
        if (board[y][x].getState() != Cell::Opened)
        {
            board[y][x].ToggleFlag();
        }
    }
};

class Board
{
private:
    int width;
    int height;
    float cellSize;
    sf::Vector2f ofset;

public:
    Board(int w, int h, float cSize, float offX, float offY)
        : width(w), height(h), cellSize(cSize), ofset(offX, offY) {}

    sf::Vector2i getCellFromMouse(int mouseX, int mouseY)
    {
        int cellX = static_cast<int>((mouseX - ofset.x) / cellSize);
        int cellY = static_cast<int>((mouseY - ofset.y) / cellSize);
        return sf::Vector2i(cellX, cellY);
    }

    void draw(sf::RenderWindow& window, const GameBoard& gameboard)
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                float posX = ofset.x + x * cellSize;
                float posY = ofset.y + y * cellSize;

                sf::RectangleShape tile(sf::Vector2f(cellSize - 2.f, cellSize - 2.f));
                tile.setPosition({posX, posY});

                Cell::State state = gameboard.getCell(x, y).getState();

                if (state == Cell::Hidden)
                {
                    tile.setFillColor(sf::Color(100, 100, 100));
                }
                else if (state == Cell::Flagged)
                {
                    tile.setFillColor(sf::Color(200, 150, 50));
                }
                else if (state == Cell::Opened)
                {
                    if (gameboard.getCell(x, y).hasMine())
                    {
                        tile.setFillColor(sf::Color(200, 50, 50));
                    }
                    else
                    {
                        tile.setFillColor(sf::Color(180, 180, 180));
                    }
                }

                window.draw(tile);
            }
        }
    }
};

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    const int windowWidth = 1600;
    const int windowHeight = 900;
    
    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}), "Minesweeper");

    GameBoard game(10, 10, 15);
    Board view(10, 10, 32.f, 50.f, 50.f);
    
    bool firstClick = true;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2i cell = view.getCellFromMouse(mousePos.x, mousePos.y);

                if (game.valid(cell.x, cell.y))
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        if (firstClick)
                        {
                            game.generateBoard(cell.x, cell.y);
                            firstClick = false;
                        }
                        game.openCell(cell.x, cell.y);
                    }
                    else if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        if (!firstClick)
                        {
                            game.toggleFlag(cell.x, cell.y);
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(40, 40, 40));
        view.draw(window, game);
        window.display();
    }

    return 0;
}

