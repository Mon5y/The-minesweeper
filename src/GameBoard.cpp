#include "GameBoard.hpp"
#include <random>
#include <ctime>

GameBoard::GameBoard(int w, int h, int mines) : width(w), height(h), mineCount(mines)
{
    board.resize(height);
    for (int i = 0; i < height; ++i)
    {
        board[i].resize(width);
    }
}

bool GameBoard::valid(int x, int y) const
{
    if ((x >= width) || (y >= height)) return false;
    if ((x < 0) || (y < 0)) return false;
    return true;
}

int GameBoard::getWidth() const { return width; }
int GameBoard::getHeight() const { return height; }
const Cell& GameBoard::getCell(int x, int y) const { return board[y][x]; }

void GameBoard::countNeighborMines()
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

void GameBoard::generateBoard(int StartX, int StartY)
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

void GameBoard::openCell(int x, int y)
{
    if (!valid(x, y)) return;
    if (status != Playing) return;
    if (board[y][x].getState() == Cell::Opened || board[y][x].getState() == Cell::Flagged) return;

    board[y][x].open();

    if (board[y][x].hasMine())
    {
        status = Lose;
        board[y][x].setState(Cell::Exploded);
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                if (board[i][j].hasMine()) {
                    board[i][j].open();
                }
            }
        }
        return;
    }
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

    checkWin();
}

void GameBoard::toggleFlag(int x, int y)
{
    if (!valid(x, y)) return;
    if (status != Playing) return;
    if (board[y][x].getState() != Cell::Opened)
    {
        board[y][x].ToggleFlag();
    }
}

void GameBoard::checkWin()
{
    int OpenedCount = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (board[y][x].getState() == Cell::Opened)
            {
                OpenedCount++;
            }
        }
    }
    if (OpenedCount == (width * height) - mineCount)
    {
        status = Won;
    }
}

void GameBoard::reset()
{
    status = Playing;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            board[y][x] = Cell();
        }
    }
}