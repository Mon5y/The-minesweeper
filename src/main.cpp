#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>
#include <random>
#include <ctime>
#include <string>

<<<<<<< Updated upstream
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
    enum GameStatus { Playing, Won, Lose };
private:
    GameStatus status = Playing;
public:
    GameStatus getStatus() const { return status; }

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
        if (status != Playing) return;
        if (board[y][x].getState() == Cell::Opened || board[y][x].getState() == Cell::Flagged) return;

        board[y][x].open();

        if (board[y][x].hasMine())
        {
            status = Lose;
            board[y][x].setState(Cell::Exploded);
            for(int i = 0;i<height;++i)
            {
                for(int j = 0;j<width;++j)
                {
                    if(board[i][j].hasMine()) {
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

    void toggleFlag(int x, int y)
    {
        if (!valid(x, y)) return;
        if (status != Playing) return;
        if (board[y][x].getState() != Cell::Opened)
        {
            board[y][x].ToggleFlag();
        }
    }

private:
    void checkWin()
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


public:
    void reset()
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

    void draw(sf::RenderWindow& window, const GameBoard& gameboard, sf::Font& font)
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
                    else if (state == Cell::Exploded) { // <--- Сначала проверяем взрыв!
                        tile.setFillColor(sf::Color(0, 255, 0)); 
                    }
                    else if (state == Cell::Opened) { // <--- Только потом обычные открытые
                        if (gameboard.getCell(x, y).hasMine()) {
                            tile.setFillColor(sf::Color(200, 50, 50)); // Обычная мина
                        } else {
                            tile.setFillColor(sf::Color(180, 180, 180)); // Пустая ячейка
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
};

enum class AppState { MainMenu, Settings, Exit, Gameplay };
=======
// Подключаем наши новые модули
#include "Cell.hpp"
#include "GameBoard.hpp"
#include "Board.hpp"

enum class AppState { MainMenu, Settings, Exit, Gameplay };
enum class Difficulty { Easy, Normal, Hard };
>>>>>>> Stashed changes

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    const int windowWidth = 1600;
    const int windowHeight = 900;

    sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "Minesweeper");

    int cols = 10;
    int rows = 10;
    float cellSize = 32.f;

    float boardWidth = cols * cellSize;
    float boardHeight = rows * cellSize;

    float offsetX = (window.getSize().x - boardWidth) / 2.f;
    float offsetY = (window.getSize().y - boardHeight) / 2.f;

    GameBoard game(cols, rows, 15);
    Board view(cols, rows, cellSize, offsetX, offsetY);
<<<<<<< Updated upstream
=======
    AppState currentState = AppState::MainMenu;
    bool firstClick = true;

    Difficulty currentDiff = Difficulty::Easy;
>>>>>>> Stashed changes

    sf::Font font;
    if (!font.loadFromFile("assets/fonts/minecraft.ttf"))
    {
        std::cout << "!!! FONT CRASH: Cannot load assets/fonts/minecraft.ttf !!!" << std::endl;
        return -1;
    }

<<<<<<< Updated upstream
   //кнопки меню
=======
    // ===== Кнопки главного меню =====
>>>>>>> Stashed changes
    sf::RectangleShape buttonPlay;
    buttonPlay.setSize(sf::Vector2f(300.f, 60.f));
    buttonPlay.setFillColor(sf::Color(200, 200, 200));

<<<<<<< Updated upstream
   
=======
>>>>>>> Stashed changes
    float buttonX = (window.getSize().x / 2.f) - (buttonPlay.getSize().x / 2.f);
    float playY = (window.getSize().y / 2.f) - (buttonPlay.getSize().y / 2.f) - 100.f; 
    buttonPlay.setPosition(sf::Vector2f(buttonX, playY));

    sf::Text textPlay;
    textPlay.setFont(font);
    textPlay.setString("Play");
    textPlay.setCharacterSize(30);
    textPlay.setFillColor(sf::Color::Black);

    float textPlayX = buttonX + (buttonPlay.getSize().x - textPlay.getGlobalBounds().width) / 2.f;
    float textPlayY = playY + (buttonPlay.getSize().y - textPlay.getGlobalBounds().height) / 2.f;
    textPlay.setPosition(sf::Vector2f(textPlayX, textPlayY - 5.f));

<<<<<<< Updated upstream


=======
>>>>>>> Stashed changes
    sf::RectangleShape buttonSettings;
    buttonSettings.setSize(sf::Vector2f(300.f, 60.f));
    buttonSettings.setFillColor(sf::Color(200, 200, 200));

<<<<<<< Updated upstream

=======
>>>>>>> Stashed changes
    float settingsY = playY + 60.f + 20.f;
    buttonSettings.setPosition(sf::Vector2f(buttonX, settingsY));

    sf::Text textSettings; 
    textSettings.setFont(font);
    textSettings.setString("Settings");
    textSettings.setCharacterSize(30);
    textSettings.setFillColor(sf::Color::Black);

    float textSettingsX = buttonX + (buttonSettings.getSize().x - textSettings.getGlobalBounds().width) / 2.f;
    float textSettingsY = settingsY + (buttonSettings.getSize().y - textSettings.getGlobalBounds().height) / 2.f;
    textSettings.setPosition(sf::Vector2f(textSettingsX, textSettingsY - 5.f));

    sf::RectangleShape buttonExit;
    buttonExit.setSize(sf::Vector2f(300.f, 60.f));
    buttonExit.setFillColor(sf::Color(200, 200, 200));

    float exitY = settingsY + 60.f + 20.f;
    buttonExit.setPosition(sf::Vector2f(buttonX, exitY));

    sf::Text textExit; 
    textExit.setFont(font);
    textExit.setString("Exit");
    textExit.setCharacterSize(30);
    textExit.setFillColor(sf::Color::Black);

    float textExitX = buttonX + (buttonExit.getSize().x - textExit.getGlobalBounds().width) / 2.f;
    float textExitY = exitY + (buttonExit.getSize().y - textExit.getGlobalBounds().height) / 2.f;
    textExit.setPosition(sf::Vector2f(textExitX, textExitY - 5.f));

<<<<<<< Updated upstream
    //=====кнопки настроек========
    sf::Text Setting_text;
    Setting_text.setFont(font);
    Setting_text.setString("Setting");
=======
    // ===== Элементы интерфейса настроек =====
    sf::Text Setting_text;
    Setting_text.setFont(font);
    Setting_text.setString("Settings");
>>>>>>> Stashed changes
    Setting_text.setCharacterSize(45);
    Setting_text.setFillColor(sf::Color::White);

    float Setting_x = (window.getSize().x - Setting_text.getGlobalBounds().width) / 2.f;
    float Setting_y = window.getSize().y * 0.07f; 
    Setting_text.setPosition(Setting_x, Setting_y);

<<<<<<< Updated upstream
    // настройки сложности
    int selectDifficult = 0;

    sf::Text textDiff;
    textDiff.setFont(font);
    textDiff.setString("Difficulty");
    textDiff.setCharacterSize(24);
    textDiff.setFillColor(sf::Color(180,180,180));
    textDiff.setPosition(150.f,180.f);

    sf::RectangleShape boxEasy(sf::Vector2f(20.f,20.f));
    sf::RectangleShape boxNormal(sf::Vector2f(20.f,20.f));
    sf::RectangleShape boxHard(sf::Vector2f(20.f,20.f));

    boxEasy.setOutlineThickness(2.f); boxEasy.setOutlineColor(sf::Color::White);
    boxNormal.setOutlineThickness(2.f); boxNormal.setOutlineColor(sf::Color::White);
    boxHard.setOutlineThickness(2.f); boxHard.setOutlineColor(sf::Color::White);

    boxEasy.setPosition(Setting_y-20,Setting_x);
    boxNormal.setPosition(Setting_y-45,Setting_x);
    boxHard.setPosition(Setting_y-70,Setting_x);

    sf::Text textEasy("Easy",font,20); textEasy.setPosition(Setting_y - 20,Setting_x + 20);
    sf::Text textNormal("Normal",font,20); textEasy.setPosition(Setting_y - 20,Setting_x + 20);
    sf::Text textHard("Hard",font,20); textEasy.setPosition(Setting_y - 20,Setting_x + 20);

    bool firstClick = true;
    AppState currentState = AppState::MainMenu;

=======
    float menuStartX = window.getSize().x * 0.05f;
    float menuStartY = window.getSize().y * 0.2f;
    float spacingY = window.getSize().y * 0.06f;

    sf::Text textDiff;
    textDiff.setFont(font);
    textDiff.setString("Difficulty:");
    textDiff.setCharacterSize(26);
    textDiff.setFillColor(sf::Color::White);
    textDiff.setPosition(menuStartX, menuStartY);

    // Сложнсть: Easy
    sf::RectangleShape boxEasy(sf::Vector2f(20.f, 20.f));
    boxEasy.setFillColor(sf::Color::Transparent);
    boxEasy.setOutlineThickness(2.f);
    boxEasy.setOutlineColor(sf::Color::White);

    sf::Text textEasy;
    textEasy.setFont(font);
    textEasy.setString("Easy");
    textEasy.setCharacterSize(24);
    textEasy.setFillColor(sf::Color::White);

    float easyY = menuStartY + spacingY;
    boxEasy.setPosition(menuStartX, easyY);
    textEasy.setPosition(menuStartX + 35.f, easyY - 2.f);

    // Сложность: Normal
    sf::RectangleShape boxNormal(sf::Vector2f(20.f, 20.f));
    boxNormal.setFillColor(sf::Color::Transparent);
    boxNormal.setOutlineThickness(2.f);
    boxNormal.setOutlineColor(sf::Color::White);

    sf::Text textNormal;
    textNormal.setFont(font);
    textNormal.setString("Normal");
    textNormal.setCharacterSize(24);
    textNormal.setFillColor(sf::Color::White);

    float NormalY = easyY + spacingY;
    boxNormal.setPosition(menuStartX, NormalY);
    textNormal.setPosition(menuStartX + 35.f, NormalY - 2.f);

    // Сложность: Hard
    sf::RectangleShape boxHard(sf::Vector2f(20.f, 20.f));
    boxHard.setFillColor(sf::Color::Transparent);
    boxHard.setOutlineThickness(2.f);
    boxHard.setOutlineColor(sf::Color::White);

    sf::Text textHard;
    textHard.setFont(font);
    textHard.setString("Hard");
    textHard.setCharacterSize(24);
    textHard.setFillColor(sf::Color::White);

    float HardY = NormalY + spacingY;
    boxHard.setPosition(menuStartX, HardY);
    textHard.setPosition(menuStartX + 35.f, HardY - 2.f);

    // ===== Главный цикл игры =====
>>>>>>> Stashed changes
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
<<<<<<< Updated upstream
=======

>>>>>>> Stashed changes
            switch (currentState)
            {
                case AppState::MainMenu:
                {
                    if (event.type == sf::Event::MouseButtonPressed)
                    {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);

<<<<<<< Updated upstream
                        if (buttonPlay.getGlobalBounds().contains(mousePosF))   currentState = AppState::Gameplay;
                        else if (buttonSettings.getGlobalBounds().contains(mousePosF))   currentState = AppState::Settings;
                        else if(buttonExit.getGlobalBounds().contains(mousePosF)) currentState = AppState::Exit;
                    }
                    break;
                }

                case AppState::Gameplay:
                {
                    if (event.type == sf::Event::KeyPressed)
                    {
                        if (event.key.code == sf::Keyboard::R)
                        {
=======
                        if (buttonPlay.getGlobalBounds().contains(mousePosF))
                        {
                            int mines = 15;

                            if (currentDiff == Difficulty::Easy)
                            {
                                cols = 10;
                                rows = 10;
                                mines = 12; // Поправили количество мин для баланса
                            }
                            else if (currentDiff == Difficulty::Normal)
                            {
                                cols = 16;
                                rows = 16;
                                mines = 40;
                            }
                            else if (currentDiff == Difficulty::Hard)
                            {
                                cols = 30;
                                rows = 16;
                                mines = 99;
                            }

                            boardWidth = cols * cellSize;
                            boardHeight = rows * cellSize;

                            offsetX = (window.getSize().x - boardWidth) / 2.f;
                            offsetY = (window.getSize().y - boardHeight) / 2.f;

                            game = GameBoard(cols, rows, mines);
                            view = Board(cols, rows, cellSize, offsetX, offsetY);
                            firstClick = true;

                            currentState = AppState::Gameplay;
                        }
                        else if (buttonSettings.getGlobalBounds().contains(mousePosF))   currentState = AppState::Settings;
                        else if (buttonExit.getGlobalBounds().contains(mousePosF))       currentState = AppState::Exit;
                    }
                    break;
                }

                case AppState::Gameplay:
                {
                    if (event.type == sf::Event::KeyPressed)
                    {
                        if (event.key.code == sf::Keyboard::R)
                        {
>>>>>>> Stashed changes
                            game.reset();
                            firstClick = true;
                        }
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
                    break;
                }
<<<<<<< Updated upstream
                case AppState::Settings:
                {
                    if(sf::Event::KeyPressed)
                    {
                        if(event.key.code == sf::Keyboard::Escape)
=======

                case AppState::Settings:
                {
                    if (event.type == sf::Event::KeyPressed)
                    {
                        if (event.key.code == sf::Keyboard::Escape)
>>>>>>> Stashed changes
                        {
                            currentState = AppState::MainMenu;
                        }
                    }
<<<<<<< Updated upstream
                    window.draw(Setting_text);
                    

                }
                break;
=======

                    if (event.type == sf::Event::MouseButtonPressed)
                    {
                        if (event.mouseButton.button == sf::Mouse::Left)
                        {
                            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                            sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);

                            if (boxEasy.getGlobalBounds().contains(mousePosF))
                            {
                                currentDiff = Difficulty::Easy;
                            } 
                            else if (boxNormal.getGlobalBounds().contains(mousePosF))
                            {
                                currentDiff = Difficulty::Normal;
                            } 
                            else if (boxHard.getGlobalBounds().contains(mousePosF))
                            {
                                currentDiff = Difficulty::Hard;
                            }
                        }
                    }
                    break;
                }

>>>>>>> Stashed changes
                case AppState::Exit:
                    window.close();
                    break;
            }
        }

<<<<<<< Updated upstream
        if (currentState == AppState::MainMenu)
                        {
                            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                            sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);

                            if (buttonPlay.getGlobalBounds().contains(mousePosF)) 
                                buttonPlay.setFillColor(sf::Color(230, 230, 230));
                            else 
                                buttonPlay.setFillColor(sf::Color(200, 200, 200));

                            if (buttonSettings.getGlobalBounds().contains(mousePosF)) 
                                buttonSettings.setFillColor(sf::Color(230, 230, 230));
                            else 
                                buttonSettings.setFillColor(sf::Color(200, 200, 200));

                            if (buttonExit.getGlobalBounds().contains(mousePosF)) 
                                buttonExit.setFillColor(sf::Color(230, 230, 230));
                            else 
                                buttonExit.setFillColor(sf::Color(200, 200, 200));
                        }
        window.clear(sf::Color(40, 40, 40));

        switch (currentState)
        {
            case AppState::MainMenu:
                window.draw(buttonPlay);
                window.draw(textPlay);
                window.draw(buttonSettings);
                window.draw(textSettings);
                window.draw(buttonExit);
                window.draw(textExit);
                break;

            case AppState::Gameplay:
                view.draw(window, game, font);
                break;
            case AppState::Settings:
                window.draw(Setting_text);
        }

=======
        // Подсветка кнопок при наведении мыши
        if (currentState == AppState::MainMenu)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);

            if (buttonPlay.getGlobalBounds().contains(mousePosF))        buttonPlay.setFillColor(sf::Color(230, 230, 230));
            else                                                         buttonPlay.setFillColor(sf::Color(200, 200, 200));
>>>>>>> Stashed changes

            if (buttonSettings.getGlobalBounds().contains(mousePosF))    buttonSettings.setFillColor(sf::Color(230, 230, 230));
            else                                                         buttonSettings.setFillColor(sf::Color(200, 200, 200));

            if (buttonExit.getGlobalBounds().contains(mousePosF))        buttonExit.setFillColor(sf::Color(230, 230, 230));
            else                                                         buttonExit.setFillColor(sf::Color(200, 200, 200));
        }

        // Отрисовка кадра
        window.clear(sf::Color(40, 40, 40));

        switch (currentState)
        {
            case AppState::MainMenu:
                window.draw(buttonPlay);
                window.draw(textPlay);
                window.draw(buttonSettings);
                window.draw(textSettings);
                window.draw(buttonExit);
                window.draw(textExit);
                break;

            case AppState::Gameplay:
                view.draw(window, game, font);
                break;

            case AppState::Settings:
                boxEasy.setFillColor(sf::Color::Transparent);
                boxNormal.setFillColor(sf::Color::Transparent);
                boxHard.setFillColor(sf::Color::Transparent);

                if (currentDiff == Difficulty::Easy)         boxEasy.setFillColor(sf::Color::White);
                else if (currentDiff == Difficulty::Normal)   boxNormal.setFillColor(sf::Color::White);
                else if (currentDiff == Difficulty::Hard)     boxHard.setFillColor(sf::Color::White);

                window.draw(Setting_text);
                window.draw(textDiff);
                window.draw(textEasy);
                window.draw(textNormal);
                window.draw(textHard);
                window.draw(boxEasy);
                window.draw(boxNormal);
                window.draw(boxHard);
                break;
        }
        window.display();
    }

    return 0;
}
