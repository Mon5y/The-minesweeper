#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>
#include <random>


class Cell
{
public:
    enum  State{Hidden,Opened,Flagged};    
private:
    bool isMine;
    int NeighborMines;
    State state;
public:
    Cell() : isMine(false),NeighborMines(0),state(Hidden) {}

    void setMine() {isMine = true;}
    bool hasMine() const {return isMine;}

    void setNeighborMines(int  count) { NeighborMines = count;}
    int getNeightborCount() const {return NeighborMines;}

    State getState() const{return state;}
    void setState(State s) {state = s;}

    void open() {if (state != Flagged) state = Opened;}
    void ToggleFlag()
    {
        if(state == Hidden) state = Flagged;
        else if(state == Flagged) state = Hidden;
    }
};

class GameBoard
{
private:
    int width;
    int height;
    int mineCount;
    std::vector<std::vector<Cell>> board;
public:
    GameBoard(int w, int h, int mines)
    {
        width = w;
        height = h;
        mineCount = mines;

        board.resize(height);
        for(int i = 0;i<height;++i)
        {
            board[i].resize(width);
        }
    }
    bool valid (int x, int y)
    {
        if((x>=width)||(y>=height)) return 0;
            else if((x<0)||(y<0)) return 0;
                 else return 1;
    }
public:
    void generateBoard(int StartX,int StartY)
    {
        int placedmines = 0;
        while(placedmines < mineCount)
        {
            int randX = rand() % width;
            int randY = rand() % height;

            if((randX == StartX) && (randY == StartY))
            {
                continue;
            }
            if(!board[randY][randX].hasMine())
            {
                board[randY][randX].setMine();
                placedmines++;
            }
        }
    }
    
private:
    void countNeighborMines()
    {
        for(int  y = 0;y<height; ++y)
        {
            for(int x = 0;x<width;++x)
            {
                if(board[y][x].hasMine()) continue;
                int mineAround = 0;
                for(int dx = -1; dx<=1;++dx)
                {
                    for(int dy = -1;dy<=1;++dy)
                    {
                        if(dx == 0 && dy == 0) continue;
                        int neighborX = x + dx;
                        int neighborY = y + dy;

                        if(valid(neighborX,neighborY) && board[neighborY][neighborX].hasMine())
                        {
                            mineAround ++;
                        }
                    }
                }
                board[y][x].setNeighborMines(mineAround);
            }
        }
    }
public:
   void openCell(int x,int y)
   {
    if(!valid(x,y)) return;

    if(board[y][x].getState() == Cell::Opened || board[y][x].getState() == Cell::Flagged)
    {
        return;
    }
    board[y][x].open();
    if(board[y][x].hasMine()) return;
    if(board[y][x].getNeightborCount() == 0)
    {
        for(int dx = -1;dx <= 1;++dx)
        {
            for(int dy = -1;dy <= 1; ++ dy)
            {
                if(dx == 0 && dy == 0) continue;

                int NeighborX = x + dx;
                int NeighborY = y + dy;

                openCell(NeighborX,NeighborY);
            }
        }
    } 
}
public:
    void toggleFlag(int  x,int y)
    {
        if(!valid(x,y)) return;
        if(board[y][x].getState() != Cell::Opened) board[y][x].ToggleFlag(); 

    }
};

int main()
{
    const int weight = 1600;
    const int hight = 900;
    sf::RenderWindow window({weight,hight},"minesweeper");

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.clear();
        window.display();
    }

}
