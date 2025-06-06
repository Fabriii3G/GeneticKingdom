#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <vector>
#include <memory>
#include <random>
#include "Enemy.h"
#include "EvolutionManager.h"

class EnemyManager {
private:
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Enemy>> lastGeneration;

    int generation;
    float mutationRate = 1.0f;
    int enemiesPerWave = 4;
    std::mt19937 rng;

    EvolutionManager evoManager;

    bool waitingForNextWave = false;
    float waveCooldown = 5.0f;
    float cooldownTimer = 0.0f;

public:
    EnemyManager(float mutationRate = 0.1f);

    float getMutationRate();

    void spawnInitialEnemies(int count); // Generaci�n 0
    void evolve(); // Ejecuta selecci�n + cruce + mutaci�n

    void updateEnemies(float deltaTime, int& credits, int& deaths); // mueve a todos los enemigos
    void applyDamageAt(sf::Vector2i position, DamageType type, float amount); // permite aplicar da�o a enemigos

    const std::vector<std::shared_ptr<Enemy>>& getEnemies() const;
    
    bool isWaveReady(float deltaTime);
    int getEnemiesPerWave() const;
    float getCooldownProgress() const;

    int getGeneration() const;
    int countDeadEnemies() const;

    void setPathsToAll(const std::vector<sf::Vector2i>& path); // A* lo genera

    std::map<std::string, float> getAverageFitnessPerType() const;


private:
    std::shared_ptr<Enemy> crossover(const Enemy& parent1, const Enemy& parent2);
    void mutate(std::shared_ptr<Enemy>& enemy);
    std::shared_ptr<Enemy> randomEnemy();

};

#endif