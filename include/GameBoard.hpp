#pragma once
#include <vector>
#include "Cell.hpp"

class GameBoard
{
private:
    int width;
    int height;
    int mineCount;
    std::vector<std::vector<Cell>> board;

    void countNeighborMines();

public:
    enum GameStatus { Playing, Won, Lose };
private:
    GameStatus status = Playing;
public:
    GameStatus getStatus() const { return status; }

    GameBoard(int w, int h, int mines);

    bool valid(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
    const Cell& getCell(int x, int y) const;

    void generateBoard(int StartX, int StartY);
    void openCell(int x, int y);
    void toggleFlag(int x, int y);
    void reset();

private:
    void checkWin();
};