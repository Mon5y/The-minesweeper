#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window({1600, 900}, "Mineswapper");

    int cell_size = 50;
    int rows = 16;
    int columns = 12;

    int total_w = columns * cell_size;
    int total_h = rows * cell_size;

    int X_start = (1600 - total_w) / 2;
    int Y_start = (900 - total_h) / 2;  // исправлено: было total_w

    std::vector<std::vector<sf::RectangleShape>> grid(rows, std::vector<sf::RectangleShape>(columns));

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            sf::RectangleShape cell(sf::Vector2f(cell_size, cell_size));
            cell.setPosition(X_start + j * cell_size, Y_start + i * cell_size);
            cell.setFillColor(sf::Color(153, 255, 153));
            cell.setOutlineThickness(1);
            cell.setOutlineColor(sf::Color::Black);
            grid[i][j] = cell;
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;

                for (int row = 0; row < rows; ++row)   // исправлено: ++row
                {
                    for (int col = 0; col < columns; ++col) // исправлено: ++col
                    {
                        if (grid[row][col].getGlobalBounds().contains(mouseX, mouseY))
                        {
                            grid[row][col].setFillColor(sf::Color(165, 42, 42));
                            std::cout << "Clicked on cell [" << row << "][" << col << "]\n";
                        }
                    }
                }
            }
        }

        window.clear(); // чёрный фон

        // отрисовка всей сетки (раньше этого не было)
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < columns; ++j)
                window.draw(grid[i][j]);

        window.display();
    }

    return 0;
}