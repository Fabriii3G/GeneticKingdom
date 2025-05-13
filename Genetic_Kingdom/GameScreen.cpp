#include <SFML/Graphics.hpp>
#include <iostream>
#include "Grid.h"

const int TILE_SIZE = 32;
const int ROWS = 25;
const int COLS = 25;

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 900;

const int MAP_WIDTH = COLS * TILE_SIZE;   // 800 px
const int MAP_HEIGHT = ROWS * TILE_SIZE;  // 800 px

const int GRID_OFFSET_X = (WINDOW_WIDTH - MAP_WIDTH) / 2;
const int GRID_OFFSET_Y = (WINDOW_HEIGHT - MAP_HEIGHT) / 2;

enum TileType {
    EMPTY = 0,
    TOWER = 1
};

void runGame() {
    sf::RenderWindow gameWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Genetic Empire");

    int grid[ROWS][COLS] = { 0 };
    fillTiles(grid);  // Carga valores iniciales si tienes celdas especiales

    // Cargar imagen de fondo
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("map.png")) {
        std::cerr << "No se pudo cargar map.png\n";
    }

    // Cargar textura de la torre
    sf::Texture textureTower;
    if (!textureTower.loadFromFile("slave1.png")) {
        std::cerr << "Error cargando slave1.png\n";
    }

    // Sprite de fondo
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setPosition(GRID_OFFSET_X, GRID_OFFSET_Y);

    // Sprite de torre
    sf::Sprite Ship;
    Ship.setTexture(textureTower);
    Ship.setScale(
        static_cast<float>(TILE_SIZE) / textureTower.getSize().x,
        static_cast<float>(TILE_SIZE) / textureTower.getSize().y
    );

    // Celda transparente para visualización
    sf::RectangleShape tileShape(sf::Vector2f(TILE_SIZE - 1, TILE_SIZE - 1));

    while (gameWindow.isOpen()) {
        sf::Event event;
        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                gameWindow.close();

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {

                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;

                // Verifica si el clic está dentro del área del mapa
                if (mouseX >= GRID_OFFSET_X && mouseX < GRID_OFFSET_X + MAP_WIDTH &&
                    mouseY >= GRID_OFFSET_Y && mouseY < GRID_OFFSET_Y + MAP_HEIGHT) {

                    int col = (mouseX - GRID_OFFSET_X) / TILE_SIZE;
                    int row = (mouseY - GRID_OFFSET_Y) / TILE_SIZE;

                    if (grid[row][col] == EMPTY) {
                        grid[row][col] = TOWER;
                        std::cout << "Slave1 colocada en: (" << row << ", " << col << ")\n";
                    }
                }
            }
        }

        gameWindow.clear(sf::Color(30, 30, 30));
        gameWindow.draw(backgroundSprite);

        // Dibujar el grid
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                int tile = grid[row][col];
                float x = GRID_OFFSET_X + col * TILE_SIZE;
                float y = GRID_OFFSET_Y + row * TILE_SIZE;

                if (tile == EMPTY) {
                    tileShape.setPosition(x, y);
                    tileShape.setFillColor(sf::Color(0, 0, 0, 0));
                    gameWindow.draw(tileShape);
                }
                else if (tile == TOWER) {
                    Ship.setPosition(x, y);
                    gameWindow.draw(Ship);
                }
                else {
                    tileShape.setPosition(x, y);
                    tileShape.setFillColor(sf::Color(0, 0, 0, 0));
                    gameWindow.draw(tileShape);
                }
            }
        }

        gameWindow.display();
    }
}
