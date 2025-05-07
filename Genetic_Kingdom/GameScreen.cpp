#include <SFML/Graphics.hpp>
#include <iostream>
#include "Grid.h"
const int TILE_SIZE = 16;
const int ROWS = 50;
const int COLS = 50;

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
    sf::RenderWindow gameWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tower Defense - 50x50 Mapa");

    int grid[ROWS][COLS] = { 0 };
    fillTiles(grid);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("map.png")) {
        std::cerr << "No se pudo cargar map.png (800x800 px)\n";
    }
    sf::Texture textureEmpty, textureTower;
    if (!textureTower.loadFromFile("tower.png")) {
        std::cerr << "Error cargando tile_tower.png\n";
    }

    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setPosition(GRID_OFFSET_X, GRID_OFFSET_Y);
    sf::Sprite tileSprite;
    sf::RectangleShape tileShape(sf::Vector2f(TILE_SIZE - 1, TILE_SIZE - 1));


    sf::Sprite towerSprite;
    towerSprite.setTexture(textureTower);
    towerSprite.setScale(
        static_cast<float>(TILE_SIZE) / textureTower.getSize().x,
        static_cast<float>(TILE_SIZE) / textureTower.getSize().y
    );


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
                    }
                }
            }
        }

        gameWindow.clear(sf::Color(30, 30, 30));
        gameWindow.draw(backgroundSprite);
        
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
            towerSprite.setPosition(x, y);
            gameWindow.draw(towerSprite);
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
