#include "EnemyManager.h"
#include "OgreEnemy.h"
#include "DarkElfEnemy.h"
#include "HarpyEnemy.h"
#include "MercenaryEnemy.h"
#include <algorithm>
#include <iostream>
#include <ctime>

EnemyManager::EnemyManager(float mutationRate)
    : generation(0), mutationRate(mutationRate), rng(static_cast<unsigned>(std::time(nullptr))) {
}

void EnemyManager::spawnInitialEnemies(int count) {
    enemies.clear();
    const int COLS = 25;
    const int filaInicio = 24;

    for (int i = 0; i < count; ++i) {
        int columna = i % 25;
        sf::Vector2i pos(columna, filaInicio);

        switch (i % 4) {
        case 0: enemies.push_back(std::make_shared<OgreEnemy>(pos)); break;
        case 1: enemies.push_back(std::make_shared<HarpyEnemy>(pos)); break;
        case 2: enemies.push_back(std::make_shared<DarkElfEnemy>(pos)); break;
        case 3: enemies.push_back(std::make_shared<MercenaryEnemy>(pos)); break;
        }
    }

    generation++;
}



void EnemyManager::updateEnemies(float deltaTime) {
    for (auto& e : enemies) {
        if (e->isAlive()) {
            e->move(deltaTime);
        }
    }

    //  Eliminar enemigos muertos
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const std::shared_ptr<Enemy>& e) {
            return !e->isAlive();
        }), enemies.end());
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


bool EnemyManager::isWaveReady(float deltaTime) {
    if (!waitingForNextWave) {
        if (enemies.empty()) {
            waitingForNextWave = true;
            cooldownTimer = 0.0f;
        }
        return false;
    }

    cooldownTimer += deltaTime;
    if (cooldownTimer >= waveCooldown) {
        waitingForNextWave = false;
        cooldownTimer = 0.0f;
        enemiesPerWave *= 2; 
        return true;
    }

    return false;
}

int EnemyManager::getEnemiesPerWave() const {
    return enemiesPerWave;
}

float EnemyManager::getCooldownProgress() const {
    return cooldownTimer / waveCooldown;
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

        // Aplicar evolución al hijo generado
        float bonus = generation * 10.0f;
        EnemyEvolution evo(
            "Gen " + std::to_string(generation),
            bonus,                       // HP extra
            0.05f * generation,          // Velocidad extra
            0.05f, 0.05f, 0.05f,         // Resistencias extra
            "evo.png"                    // Sprite overlay de evolución
        );
        child->addEvolution(evo);

        newGeneration.push_back(child);
    }


    enemies = newGeneration;
    generation++;
}

std::shared_ptr<Enemy> EnemyManager::crossover(const Enemy& parent1, const Enemy& parent2) {
    sf::Vector2i start = { rand() % 25, 24 }; // posición random en fila 24

    // Promediar atributos
    float hp = (parent1.getFitness() + parent2.getFitness()) / 2.0f + 100.0f;
    float speed = (parent1.getSpeed() + parent2.getSpeed()) / 2.0f;
    float rArrow = (parent1.getResistanceArrows() + parent2.getResistanceArrows()) / 2.0f;
    float rMagic = (parent1.getResistanceMagic() + parent2.getResistanceMagic()) / 2.0f;
    float rArtillery = (parent1.getResistanceArtillery() + parent2.getResistanceArtillery()) / 2.0f;

    return std::make_shared<OgreEnemy>(start, hp);

}



void EnemyManager::mutate(std::shared_ptr<Enemy>& enemy) {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    if (dist(rng) < mutationRate) {
        auto names = evoManager.getAvailableNames();
        std::uniform_int_distribution<int> idxDist(0, names.size() - 1);

        const std::string& selectedName = names[idxDist(rng)];
        const EnemyEvolution& evo = evoManager.getEvolution(selectedName);

        enemy->addEvolution(evo);
        std::cout << "[MUTACION] Se aplicó evolución: " << selectedName << "\n";
    }
}

std::shared_ptr<Enemy> EnemyManager::randomEnemy() {
    return std::make_shared<OgreEnemy>();
}