#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <vector>
#include <memory>
#include <random>
#include "Enemy.h"

class EnemyManager {
private:
    std::vector<std::shared_ptr<Enemy>> enemies;
    int generation;
    float mutationRate;

    std::mt19937 rng;

public:
    EnemyManager(float mutationRate = 0.1f);

    void spawnInitialEnemies(int count); // Generaci�n 0
    void evolve(); // Ejecuta selecci�n + cruce + mutaci�n

    void updateEnemies(); // mueve a todos los enemigos
    void applyDamageAt(sf::Vector2i position, DamageType type, float amount); // permite aplicar da�o a enemigos

    const std::vector<std::shared_ptr<Enemy>>& getEnemies() const;

    int getGeneration() const;
    int countDeadEnemies() const;

    void setPathsToAll(const std::vector<sf::Vector2i>& path); // A* lo genera

private:
    std::shared_ptr<Enemy> crossover(const Enemy& parent1, const Enemy& parent2);
    void mutate(std::shared_ptr<Enemy>& enemy);
    std::shared_ptr<Enemy> randomEnemy();
};

#endif