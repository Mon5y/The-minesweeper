#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>


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
    int getNeightbourCount() const {return NeighborMines;}

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
    int Wight;
    int Height;

    std::vector<std::vector<Cell>> board;
    board.resize(Height);
    for (int i = 0;i<Wight;++i)
    {
        board[i].resize(Wight);
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
