#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdint>
#include "include/Vector2f.h"
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

class Board
{
private:
    int width,heiht;
    float cellSize;
    std::Vector2f offset;
public:
    board(int w,int h,float cellSize,float offset)
    {
        width = w;heiht = h; cellSize = 30.f;offset = 0.5f;
    }
    void draw(sf::RectangleWindow& window, const GameBoard& gameboard)
    {
        for(int y = 0;y<heiht;++y)
        {
            for(int x = 0;x< width;++x)
            {
                float PosX = offset + x*cellSize;
                float PosY = offset + y*sellSize;

                sf::RectangleShape cell({cellSize - 1.f,cellSize - 1.f});
                cell.setPosition(PosX,PosY);

                Cell::state  = gameboard.getBoard.getCellState(x,y);
                if(state = cell::Hidden)
                {
                    cell.setFillColor(sf::color(100,100,100))
                } else if (state == cell::Flagged)
                {
                    cell.setFillColor(sf::color(200,150,50));
                }
                else
                {
                    cell.setFillColor(sf::color(200,200,200))
                }
            }
            window.draw(cell);
            }
        }
    

};

	int main()
{
    const int weight = 1600;
    const int hight = 900;
    sf::RenderWindow window({weight, hight}, "Minesweeper");
    
    // Создаем игровое поле
    GameBoard game(30, 20, 99); // 30x20 с 99 минами
    game.generateBoard(0, 0);
    
    // Создаем доску для отрисовки
    Board board(30, 20, 30.f, 20.f, 20.f);
    
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if(event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2i cell = board.getCellFromMouse(mousePos.x, mousePos.y);
                
                if(cell.x >= 0 && cell.y >= 0)
                {
                    if(event.mouseButton.button == sf::Mouse::Left)
                    {
                        game.openCell(cell.x, cell.y);
                    }
                    else if(event.mouseButton.button == sf::Mouse::Right)
                    {
                        game.toggleFlag(cell.x, cell.y);
                    }
                }
            }
        }
        
        window.clear(sf::Color::White);
        board.draw(window, game);
        window.display();
    }
    return 0;
}
