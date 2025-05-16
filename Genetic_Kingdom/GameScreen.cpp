#include <SFML/Graphics.hpp>
#include <iostream>
#include "Grid.h"
#include "EnemyManager.h"


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

    EnemyManager enemyManager;
    int grid[ROWS][COLS] = { 0 };
    fillTiles(grid);  // Celdas con obstáculos (valor 2)

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

    // Sprite de enemigos se cargan en las subclases 

    // Dummy path temporal 
    std::vector<sf::Vector2i> path;
    for (int i = 0; i < 25; ++i) {
        path.push_back({ i, 0 });
    }

    // Crear enemigos y asignarles camino
    enemyManager.spawnInitialEnemies(5);
    enemyManager.setPathsToAll(path);

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

        // Actualizar enemigos
        enemyManager.updateEnemies();

        // Limpiar enemigos previos del grid
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                if (grid[row][col] == 10) {
                    grid[row][col] = EMPTY;
                }
            }
        }

        // Actualizar enemigos en el grid
        for (const auto& enemy : enemyManager.getEnemies()) {
            if (enemy->isAlive()) {
                sf::Vector2i pos = enemy->getPosition();
                if (pos.y >= 0 && pos.y < ROWS && pos.x >= 0 && pos.x < COLS) {
                    grid[pos.y][pos.x] = 10;
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
                else if (tile == 10) {
                    tileShape.setPosition(x, y);
                    tileShape.setFillColor(sf::Color::Transparent);
                    gameWindow.draw(tileShape); // fondo para enemigos si quieres
                }
            }
        }

        // Dibujar enemigos (sprites)
        for (const auto& enemy : enemyManager.getEnemies()) {
            if (enemy->isAlive()) {
                enemy->draw(gameWindow, TILE_SIZE, sf::Vector2f(GRID_OFFSET_X, GRID_OFFSET_Y));
            }
        }

        gameWindow.display();
    }
}
