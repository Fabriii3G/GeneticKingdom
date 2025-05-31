#include <SFML/Graphics.hpp>
#include <iostream>
#include "Grid.h"
#include "LowTower.h"
#include "MidTower.h"
#include "HighTower.h"
#include <memory>
#include <vector>
#include "Projectile.h"
#include "EnemyManager.h"
#include "Pathfinder.h"
#include <set>
#include <cstdlib> 
#include <ctime>
#include <iomanip>
#include <sstream>


const int COST_LOW = 100;
const int COST_MID = 500;
const int COST_HIGH = 800;

const int TILE_SIZE = 32;
const int ROWS = 25;
const int COLS = 25;

const int WINDOW_WIDTH = 1300;
const int WINDOW_HEIGHT = 1000;

const int MAP_WIDTH = COLS * TILE_SIZE;   // 800 px
const int MAP_HEIGHT = ROWS * TILE_SIZE;  // 800 px

const int GRID_OFFSET_X = (WINDOW_WIDTH - MAP_WIDTH) / 2;
const int GRID_OFFSET_Y = (WINDOW_HEIGHT - MAP_HEIGHT) / 2;


// Funcion que devuelve un n�mero aleatorio entre 1 y 10
int RandomNumber() {
    srand(time(0));
    return rand() % 10 + 1;
}

enum TileType {
    EMPTY = 0,
    LOWTOWER = 1,
    MIDTOWER = 2,
    HIGHTOWER = 3
};

void runGame() {

  

    std::vector<Projectile> projectiles;
    std::set<Tower*> upgradedTowers;
    sf::Clock clock; // Para calcular deltaTime
    int deaths = 0;
    int credits = 100000;
    int NumWaves = 1;

    sf::RenderWindow gameWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Genetic Empire");

    EnemyManager enemyManager;
    int grid[ROWS][COLS] = { 0 };
    fillTiles(grid);
    float mutRate = enemyManager.getMutationRate();
    // Cargar recursos
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("map.png");

    sf::Texture textureLow, textureMid, textureHigh, textureUpgrade1, textureUpgrade2, textureUpgrade3;
    textureLow.loadFromFile("TieFighter.png");
    textureMid.loadFromFile("Slave1.png");
    textureHigh.loadFromFile("XWing.png");


    textureUpgrade1.loadFromFile("evo_magic.png");
    textureUpgrade2.loadFromFile("evo_arrow.png");
    textureUpgrade3.loadFromFile("evo_speed.png");

    sf::Sprite upgradeOverlaySprite;
    upgradeOverlaySprite.setTexture(textureUpgrade1);


    sf::Font font;
    if (!font.loadFromFile("Space.ttf")) {
        std::cerr << "No se pudo cargar la fuente\n";
    }

    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setPosition(GRID_OFFSET_X, GRID_OFFSET_Y);

    sf::RectangleShape tileShape(sf::Vector2f(TILE_SIZE - 1, TILE_SIZE - 1));
    std::vector<std::vector<Tower*>> towers(ROWS, std::vector<Tower*>(COLS, nullptr));

    // --- Botones ---
    const int buttonWidth = 200;
    const int buttonHeight = 40;
    const int buttonY = WINDOW_HEIGHT - 60;

    sf::RectangleShape buttonLow(sf::Vector2f(buttonWidth, buttonHeight));
    buttonLow.setPosition(100, buttonY);
    buttonLow.setFillColor(sf::Color::White);

    sf::RectangleShape buttonMid(sf::Vector2f(buttonWidth + 30, buttonHeight));
    buttonMid.setPosition(400, buttonY);
    buttonMid.setFillColor(sf::Color::White);

    sf::RectangleShape buttonHigh(sf::Vector2f(buttonWidth + 20, buttonHeight));
    buttonHigh.setPosition(730, buttonY);
    buttonHigh.setFillColor(sf::Color::White);

    sf::Text textLow("Torre bajo nivel: 100", font, 18);
    textLow.setFillColor(sf::Color::Black);
    textLow.setPosition(110, buttonY + 8);

    sf::Text textMid("Torre mediano nivel: 500", font, 18);
    textMid.setFillColor(sf::Color::Black);
    textMid.setPosition(410, buttonY + 8);

    sf::Text textHigh("Torre de alto nivel: 800", font, 18);
    textHigh.setFillColor(sf::Color::Black);
    textHigh.setPosition(740, buttonY + 8);

    // Stats
    sf::Text waveText, deathsText, RateText, MutText;
    waveText.setFont(font);
    waveText.setCharacterSize(20);
    waveText.setFillColor(sf::Color::White);
    waveText.setPosition(20, 50);
    waveText.setString("Generaciones transcurridas: " + std::to_string(NumWaves));
    deathsText.setFont(font);
    deathsText.setCharacterSize(20);
    deathsText.setFillColor(sf::Color::White);
    deathsText.setPosition(20, 110);
    deathsText.setString("Muertes: " + std::to_string(deaths));
    MutText.setFont(font);
    MutText.setCharacterSize(20);
    MutText.setFillColor(sf::Color::White);
    MutText.setPosition(20, 140);
    MutText.setString("Mutaciones: " + std::to_string(0));
    RateText.setFont(font);
    RateText.setCharacterSize(20);
    RateText.setFillColor(sf::Color::White);
    RateText.setPosition(20, 80);
    RateText.setString("Probabilidad de mutar: " + std::to_string(mutRate) + "%");

    sf::Text fitnessText;
    fitnessText.setFont(font);
    fitnessText.setCharacterSize(20);
    fitnessText.setFillColor(sf::Color::White);
    fitnessText.setPosition(20, 170);
    fitnessText.setString("Fitness promedio:");


    // Variable para saber que torre fue seleccionada
    enum TowerType { LOW, MID, HIGH };
    TowerType selectedTowerType = LOW;

    // Creditos
    sf::Text creditText;
    creditText.setFont(font);
    creditText.setCharacterSize(20);
    creditText.setFillColor(sf::Color::White);
    creditText.setPosition(20, 20);
    creditText.setString("Creditos: " + std::to_string(credits));

    int NumTower = 0; // Contador unico de torres
    std::map<Tower*, int> towerIDs; // Mapa para guardar el ID asignado a cada torre

    

    // Crear enemigos y asignarles camino
    enemyManager.spawnInitialEnemies(4);
    for (auto& enemy : enemyManager.getEnemies()) {
        sf::Vector2i start = enemy->getPosition();
        sf::Vector2i goal = { 18, 5 };

        std::vector<sf::Vector2i> path = findPathAStar(start, goal, grid);
        enemy->setPath(path);
    }


    // --- Loop principal ---
    while (gameWindow.isOpen()) {
        float deltaTime = clock.restart().asSeconds(); 

        sf::Event event;
        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                gameWindow.close();

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {

                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;

                // Verifica clic en botones
                if (buttonLow.getGlobalBounds().contains(mouseX, mouseY)) {
                    selectedTowerType = LOW;
                    std::cout << "Seleccionada torre de bajo nivel\n";
                    continue;
                }
                if (buttonMid.getGlobalBounds().contains(mouseX, mouseY)) {
                    selectedTowerType = MID;
                    std::cout << "Seleccionada torre de nivel medio\n";
                    continue;
                }
                if (buttonHigh.getGlobalBounds().contains(mouseX, mouseY)) {
                    selectedTowerType = HIGH;
                    std::cout << "Seleccionada torre de alto nivel\n";
                    continue;
                }

                // Verifica clic dentro del grid
                if (mouseX >= GRID_OFFSET_X && mouseX < GRID_OFFSET_X + MAP_WIDTH &&
                    mouseY >= GRID_OFFSET_Y && mouseY < GRID_OFFSET_Y + MAP_HEIGHT) {

                    int col = (mouseX - GRID_OFFSET_X) / TILE_SIZE;
                    int row = (mouseY - GRID_OFFSET_Y) / TILE_SIZE;

                    // Si ya hay una torre en esa celda, imprimir mensaje de mejora
                    if (towers[row][col] != nullptr && towers[row][col]->upgradeCounter == 0 && credits >= 500) {
                        credits -= 500;
                        Tower* clickedTower = towers[row][col];
                        int towerId = towerIDs[clickedTower];
                        upgradedTowers.insert(clickedTower);
                        clickedTower->upgrade();

                    }
                    else if (towers[row][col] != nullptr && towers[row][col]->upgradeCounter == 1 && credits >= 1000) {
                        credits -= 1000;
                        Tower* clickedTower = towers[row][col];
                        int towerId = towerIDs[clickedTower];
                        upgradedTowers.insert(clickedTower);
                        clickedTower->upgrade();

                    }
                    else if (towers[row][col] != nullptr && towers[row][col]->upgradeCounter == 2 && credits >= 1500) {
                        credits -= 1500;
                        Tower* clickedTower = towers[row][col];
                        int towerId = towerIDs[clickedTower];
                        upgradedTowers.insert(clickedTower);
                        clickedTower->upgrade();

                    }

                    if (grid[row][col] == EMPTY) {
                        int cost = 0;
                        Tower* newTower = nullptr;

                        if (selectedTowerType == LOW && credits >= COST_LOW) {
                            cost = COST_LOW;
                            newTower = new LowTower();
                            newTower->setTexture(textureLow, TILE_SIZE);
                            grid[row][col] = LOWTOWER;
                        }
                        else if (selectedTowerType == MID && credits >= COST_MID) {
                            cost = COST_MID;
                            newTower = new MidTower();
                            newTower->setTexture(textureMid, TILE_SIZE);
                            grid[row][col] = MIDTOWER;
                        }
                        else if (selectedTowerType == HIGH && credits >= COST_HIGH) {
                            cost = COST_HIGH;
                            newTower = new HighTower();
                            newTower->setTexture(textureHigh, TILE_SIZE);
                            grid[row][col] = HIGHTOWER;
                        }

                        if (newTower != nullptr) {
                            credits -= cost;
                            towers[row][col] = newTower;
                            towerIDs[newTower] = NumTower++;
                            creditText.setString("Creditos: " + std::to_string(credits));
                            std::cout << "Torre colocada en: (" << row << ", " << col << "), Coste: " << cost << ", Creditos restantes: " << credits << "\n";

                            // Recalcular caminos de todos los enemigos despues de colocar una torre
                            for (auto& enemy : enemyManager.getEnemies()) {
                                if (enemy->isAlive()) {
                                    sf::Vector2i current = enemy->getPosition();
                                    sf::Vector2i goal = { 18, 5 };

                                    std::vector<sf::Vector2i> newPath = findPathAStar(current, goal, grid);

                                    if (!newPath.empty()) {
                                        enemy->setPath(newPath);
                                    }
                                    else {
                                        std::cout << "[ADVERTENCIA] Enemigo en (" << current.y << ", " << current.x << ") no tiene camino disponible\n";
                                    }
                                }
                            }

                        }
                        else {
                            std::cout << "No tienes suficientes creditos para esta torre.\n";
                        }
                    }
                }
            }
        }



        for (auto& enemy : enemyManager.getEnemies()) {
            if (enemy->isAlive()) {
                sf::Vector2i current = enemy->getPosition();
                if (current == sf::Vector2i(18, 5)) {
                    enemy->kill();
                }
            }
        }

        // Actualizar enemigos
        enemyManager.updateEnemies(deltaTime, credits, deaths);
        creditText.setString("Creditos: " + std::to_string(credits));
        deathsText.setString("Muertes: " + std::to_string(deaths));
        // Calcular y mostrar fitness promedio antes de limpiar enemigos
        auto averages = enemyManager.getAverageFitnessPerType();

        std::ostringstream oss;
        oss << "Fitness promedio:\n";
        for (const auto& pair : averages) {
            if (pair.first == "Ogro") {
                oss << "Clone Tropper" << ": " << std::fixed << std::setprecision(2) << pair.second << "\n";
            } else if (pair.first == "Harpia") {
                oss << "Darth Maul" << ": " << std::fixed << std::setprecision(2) << pair.second << "\n";
            }
            else if (pair.first == "Elfo Oscuro") {
                oss << "Jawa" << ": " << std::fixed << std::setprecision(2) << pair.second << "\n";
            }
            else {
                oss << "Greedo" << ": " << std::fixed << std::setprecision(2) << pair.second << "\n";
            }
            
        }
        fitnessText.setString(oss.str());


        if (enemyManager.isWaveReady(deltaTime)) {

            std::cout << "[INFO] Preparando nueva oleada...\n";

            std::cout << "[INFO] Ejecutando evolucion antes de la oleada " << NumWaves + 1 << "...\n";
            enemyManager.evolve();  //llamado para hacer mutaciones


            int cantidad = enemyManager.getEnemiesPerWave();
            std::cout << "[INFO] Nueva oleada con " << cantidad << " enemigos\n";

            enemyManager.spawnInitialEnemies(cantidad);
            NumWaves += 1;

            for (auto& enemy : enemyManager.getEnemies()) {
                sf::Vector2i start = enemy->getPosition();
                sf::Vector2i goal = { 18, 5 };
                std::vector<sf::Vector2i> path = findPathAStar(start, goal, grid);
                enemy->setPath(path);
            }
        }

        waveText.setString("Generaciones transcurridas: " + std::to_string(NumWaves));

        // Limpiar enemigos previos del grid
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                if (grid[row][col] == 10) {
                    grid[row][col] = EMPTY;

                }
            }
        }

        //Actualizar enemigos en el grid
        for (const auto& enemy : enemyManager.getEnemies()) {
            if (enemy->isAlive()) {
                sf::Vector2i pos = enemy->getPosition();
                if (pos.y >= 0 && pos.y < ROWS && pos.x >= 0 && pos.x < COLS) {
                    int cell = grid[pos.y][pos.x];
                    if (cell == 0 || cell == 5 || cell == 10) {
                        grid[pos.y][pos.x] = 10;
                    }
                }
            }
        }


        for (const auto& enemy : enemyManager.getEnemies()) {
            if (enemy->isAlive()) {
                enemy->draw(gameWindow, TILE_SIZE, sf::Vector2f(GRID_OFFSET_X, GRID_OFFSET_Y));
            }
        }

        gameWindow.clear(sf::Color::Black);
        gameWindow.draw(backgroundSprite);

        // Actualizar torres
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                if (towers[row][col]) {
                    float x = GRID_OFFSET_X + col * TILE_SIZE;
                    float y = GRID_OFFSET_Y + row * TILE_SIZE;
                    towers[row][col]->update(x, y, grid, row, col, projectiles, deltaTime, RandomNumber());
                }
            }
        }

        for (auto it = projectiles.begin(); it != projectiles.end();) {
            it->update(deltaTime);
            if (it->hasReachedTarget()) {
                int enemyRow = it->getTargetRow();
                int enemyCol = it->getTargetCol();

                sf::Vector2i targetPos(enemyCol, enemyRow);

                // Aplica dano al enemigo en esa posicion
                enemyManager.applyDamageAt(targetPos, it->getDamageType(), it->getDamageAmount());
                


                if (grid[enemyRow][enemyCol] == 10) {
                    grid[enemyRow][enemyCol] = 0;
                    std::cout << "Enemigo golpeado en (" << enemyRow << ", " << enemyCol << ")\n";
                }

                it = projectiles.erase(it);
            }
            else {
                ++it;
            }
        }



        // Dibujar el grid
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                float x = GRID_OFFSET_X + col * TILE_SIZE;
                float y = GRID_OFFSET_Y + row * TILE_SIZE;

                if (grid[row][col] == LOWTOWER && towers[row][col]) {
                    towers[row][col]->draw(gameWindow, x, y);
                    int lvl = towers[row][col]->upgradeCounter;

                    if (upgradedTowers.count(towers[row][col])) {
                        sf::Texture* overlayTexture = nullptr;

                        if (lvl == 1) overlayTexture = &textureUpgrade1;
                        else if (lvl == 2) overlayTexture = &textureUpgrade2;
                        else if (lvl == 3) overlayTexture = &textureUpgrade3;

                        if (overlayTexture != nullptr) {
                            upgradeOverlaySprite.setTexture(*overlayTexture);
                            upgradeOverlaySprite.setPosition(x, y);
                            upgradeOverlaySprite.setScale(
                                TILE_SIZE / (float)overlayTexture->getSize().x,
                                TILE_SIZE / (float)overlayTexture->getSize().y
                            );
                            gameWindow.draw(upgradeOverlaySprite);
                        }
                    }
                }

                else if (grid[row][col] == MIDTOWER && towers[row][col]) {
                    towers[row][col]->draw(gameWindow, x, y);
                    int lvl = towers[row][col]->upgradeCounter;

                    if (upgradedTowers.count(towers[row][col])) {
                        sf::Texture* overlayTexture = nullptr;

                        if (lvl == 1) overlayTexture = &textureUpgrade1;
                        else if (lvl == 2) overlayTexture = &textureUpgrade2;
                        else if (lvl == 3) overlayTexture = &textureUpgrade3;

                        if (overlayTexture != nullptr) {
                            upgradeOverlaySprite.setTexture(*overlayTexture);
                            upgradeOverlaySprite.setPosition(x, y);
                            upgradeOverlaySprite.setScale(
                                TILE_SIZE / (float)overlayTexture->getSize().x,
                                TILE_SIZE / (float)overlayTexture->getSize().y
                            );
                            gameWindow.draw(upgradeOverlaySprite);
                        }
                    }
                }
                else if (grid[row][col] == HIGHTOWER && towers[row][col]) {
                    towers[row][col]->draw(gameWindow, x, y);
                    int lvl = towers[row][col]->upgradeCounter;

                    if (upgradedTowers.count(towers[row][col])) {
                        sf::Texture* overlayTexture = nullptr;

                        if (lvl == 1) overlayTexture = &textureUpgrade1;
                        else if (lvl == 2) overlayTexture = &textureUpgrade2;
                        else if (lvl == 3) overlayTexture = &textureUpgrade3;

                        if (overlayTexture != nullptr) {
                            upgradeOverlaySprite.setTexture(*overlayTexture);
                            upgradeOverlaySprite.setPosition(x, y);
                            upgradeOverlaySprite.setScale(
                                TILE_SIZE / (float)overlayTexture->getSize().x,
                                TILE_SIZE / (float)overlayTexture->getSize().y
                            );
                            gameWindow.draw(upgradeOverlaySprite);
                        }
                    }
                }
                else {
                    tileShape.setPosition(x, y);
                    tileShape.setFillColor(sf::Color(0, 0, 0, 0));
                    gameWindow.draw(tileShape);
                }
            }
        }

        // Dibujar proyectiles
        for (const auto& p : projectiles) {
            p.draw(gameWindow);
        }

        // Dibujar enemigos (sprites)
        for (const auto& enemy : enemyManager.getEnemies()) {
            if (enemy->isAlive()) {
                enemy->draw(gameWindow, TILE_SIZE, sf::Vector2f(GRID_OFFSET_X, GRID_OFFSET_Y));
            }
        }


        // Dibujar botones
        gameWindow.draw(buttonLow);
        gameWindow.draw(buttonMid);
        gameWindow.draw(buttonHigh);
        gameWindow.draw(textLow);
        gameWindow.draw(textMid);
        gameWindow.draw(textHigh);
        gameWindow.draw(creditText);
        gameWindow.draw(waveText);
        gameWindow.draw(fitnessText);
        gameWindow.draw(deathsText);
        gameWindow.draw(RateText);
        gameWindow.draw(MutText);
        gameWindow.display();
    }

}