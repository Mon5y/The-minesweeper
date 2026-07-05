#pragma once

class Cell
{
public:
    enum State { Hidden, Opened, Flagged, Exploded };
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