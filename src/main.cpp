#include <iostream>
#include <SFML/Graphics.hpp>


class Cell
{
    public:
        enum State{Hidden,Opened,Flagged};
    private:
        bool isMine;
        int NeighborMines;
        State state;
    public:
        
    Cell():isMine(false),NeighborMines(0),state(Hidden) {}
        
        void setMine() { isMine = true;}
        bool hasMine() const {return isMine;}

        void setNeighborMine(int count) {NeighborMines = count;}
        int  hasNeighborMine() const {return NeighborMines;}

        State getState() const {return state;}
        void getState(State s) {state = s;}

        void open() {if(state != Flagged) state = Opened;}
        void toggleFlag()
        {
            if(state == Hidden) state = Flagged;
            else if (state == Flagged) state == Hidden;
        }
    
};

int main()
{
    sf::RenderWindow window(sf::VideoMode{1600,900},"minesweper");

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.setFramerateLimit(60);
        window.clear();
        window.display();
    }
    return 0;
}
