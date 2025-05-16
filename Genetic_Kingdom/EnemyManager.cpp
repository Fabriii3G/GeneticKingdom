#include "EnemyManager.h"
#include "OgreEnemy.h" // Luego incluirás los demás tipos
#include <algorithm>
#include <iostream>
#include <ctime>

EnemyManager::EnemyManager(float mutationRate)
    : generation(0), mutationRate(mutationRate), rng(static_cast<unsigned>(std::time(nullptr))) {
}

void EnemyManager::spawnInitialEnemies(int count) {
    enemies.clear();
    for (int i = 0; i < count; ++i) {
        enemies.push_back(std::make_shared<OgreEnemy>()); // temporal, se irán agregando tipos
    }
    generation = 0;
}

void EnemyManager::updateEnemies() {
    for (auto& e : enemies) {
        if (e->isAlive()) {
            e->move();
        }
    }
}

void EnemyManager::applyDamageAt(sf::Vector2i position, DamageType type, float amount) {
    for (auto& e : enemies) {
        if (e->isAlive() && e->getPosition() == position) {
            e->receiveDamage(type, amount);
        }
    }
}

const std::vector<std::shared_ptr<Enemy>>& EnemyManager::getEnemies() const {
    return enemies;
}

int EnemyManager::getGeneration() const {
    return generation;
}

int EnemyManager::countDeadEnemies() const {
    return std::count_if(enemies.begin(), enemies.end(), [](const std::shared_ptr<Enemy>& e) {
        return !e->isAlive();
        });
}

void EnemyManager::setPathsToAll(const std::vector<sf::Vector2i>& path) {
    for (auto& e : enemies) {
        e->setPath(path);
    }
}

void EnemyManager::evolve() {
    std::vector<std::shared_ptr<Enemy>> newGeneration;

    // Selección por fitness (orden descendente)
    std::sort(enemies.begin(), enemies.end(), [](const auto& a, const auto& b) {
        return a->getFitness() > b->getFitness();
        });

    int topCount = enemies.size() / 2;

    // Cruce entre los mejores
    for (int i = 0; i < enemies.size(); ++i) {
        const Enemy& p1 = *enemies[i % topCount];
        const Enemy& p2 = *enemies[(i + 1) % topCount];

        auto child = crossover(p1, p2);
        mutate(child);
        newGeneration.push_back(child);
    }

    enemies = newGeneration;
    generation++;
}

std::shared_ptr<Enemy> EnemyManager::crossover(const Enemy& parent1, const Enemy& parent2) {
    // Por ahora retorna un nuevo ogro promedio (personalizar según atributos)
    sf::Vector2i start = { 0, 0 };
    float hp = (parent1.getFitness() + parent2.getFitness()) / 2.0f + 100.0f;
    return std::make_shared<OgreEnemy>(start, hp);
}

void EnemyManager::mutate(std::shared_ptr<Enemy>& enemy) {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    if (dist(rng) < mutationRate) {
        // Ejemplo: aumenta vida un poco
        std::cout << "Mutación ocurrida\n";
        // Aquí podrías implementar métodos como `enemy->mutateHealth(+10)`
    }
}

std::shared_ptr<Enemy> EnemyManager::randomEnemy() {
    return std::make_shared<OgreEnemy>();
}
