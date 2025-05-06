#include "GameScreen.h"
#include <SFML/Graphics.hpp>
#include <iostream>

const int TILE_SIZE = 32;
const int ROWS = 15;
const int COLS = 20;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int MAP_WIDTH = COLS * TILE_SIZE;
const int MAP_HEIGHT = ROWS * TILE_SIZE;

const int GRID_OFFSET_X = (WINDOW_WIDTH - MAP_WIDTH) / 2;
const int GRID_OFFSET_Y = (WINDOW_HEIGHT - MAP_HEIGHT) / 2;

enum TileType {
    EMPTY = 0,
    TOWER = 2
};

void runGame() {
    sf::RenderWindow gameWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tower Defense - Mapa Personalizado");

    int grid[ROWS][COLS] = { 0 };
    grid[3][6] = 1;
    grid[3][7] = 1;
    grid[4][7] = 1;
    grid[4][8] = 1;
    grid[3][8] = 1;
    grid[3][9] = 1;
    grid[3][10] = 1;
    grid[2][10] = 1;
    grid[2][11] = 1;
    grid[3][11] = 1;
    grid[2][12] = 1;
    grid[1][12] = 1;
    grid[1][13] = 1;
    grid[2][13] = 1;
    grid[0][12] = 1;
    grid[0][11] = 1;
    grid[1][11] = 1;
    grid[0][10] = 1;
    grid[1][9] = 1;
    grid[1][10] = 1;
    grid[0][9] = 1;
    grid[2][9] = 1;
    grid[2][8] = 1;
    grid[2][7] = 1;
    grid[1][7] = 1;
    grid[1][8] = 1;
    grid[0][8] = 1;
    grid[2][6] = 1;
    grid[1][6] = 1;
  
    // Cargar fondo personalizado
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("map.png")) {
        std::cerr << "No se pudo cargar el fondo del mapa (map.png)\n";
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setPosition(GRID_OFFSET_X, GRID_OFFSET_Y);

    // Forma de cada celda
    sf::RectangleShape tileShape(sf::Vector2f(TILE_SIZE - 2, TILE_SIZE - 2));

    while (gameWindow.isOpen()) {
        sf::Event event;
        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                gameWindow.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;

                int col = (mouseX - GRID_OFFSET_X) / TILE_SIZE;
                int row = (mouseY - GRID_OFFSET_Y) / TILE_SIZE;

                if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
                    if (grid[row][col] == EMPTY) {
                        grid[row][col] = TOWER;
                        std::cout << "Torre colocada en: (" << row << ", " << col << ")\n";
                        std::cout << grid[row][col];
                    }
                    else {
                        std::cout << "Hay obstaculo en: (" << row << ", " << col <<")\n";
                        std::cout << grid[row][col];
                    }
                }
            }
        }

        gameWindow.clear(sf::Color(50, 50, 50)); // fondo gris oscuro

        // Dibujar imagen de fondo
        gameWindow.draw(backgroundSprite);

        // Dibujar celdas
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                tileShape.setPosition(GRID_OFFSET_X + col * TILE_SIZE + 1,
                    GRID_OFFSET_Y + row * TILE_SIZE + 1);

                if (grid[row][col] == EMPTY)
                    tileShape.setFillColor(sf::Color(0, 0, 0, 0));
                else if (grid[row][col] == TOWER)
                    tileShape.setFillColor(sf::Color::Blue);
                else {
                    tileShape.setFillColor(sf::Color(0, 0, 0, 0));
                }

                gameWindow.draw(tileShape);
            }
        }

        gameWindow.display();
    }
}