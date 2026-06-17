#include <iostream>
#include <SFML/Graphics.hpp>



class Cell
{
public:
    enum State{Hidden,Opened,Flagged};    
private:
    bool isMine;
    int NeightboorMines;
    State state;
public:
    Cell() : isMine(false),NeightboorMines(0),state(Hidden) {}

    void setMine() {isMine = true;}
    bool hasMine() const {return isMine;}

    void setNeightbourMines(int  count) { NeightboorMines = count;}
    int getNeightbour() const {return NeightboorMines;}

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
