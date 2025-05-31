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

    if (!enemies.empty()) {
        lastGeneration = enemies;  //Guardar copia para evolución
        std::cout << "[INFO] Se guardo una copia de la generacion anterior ("
            << lastGeneration.size() << " enemigos)\n";
    }
    else {
        std::cout << "[INFO] No se guardo copia, porque la lista de enemigos estaba vacia.\n";
    }

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

    std::cout << "[DEBUG] spawnInitialEnemies: Se generaron " << enemies.size()
        << " enemigos para la generacion " << generation << ".\n";
}



void EnemyManager::updateEnemies(float deltaTime, int& credits) {
    for (auto& e : enemies) {
        if (e->isAlive()) {
            e->move(deltaTime);
        }
        else if (e->killedByPlayer) {
            std::string type = e->getType();
            if (type == "Ogro") {
                credits += 75;
            }
            else if (type == "Harpia") {
                credits += 100;
            }
            else if (type == "Elfo Oscuro") {
                credits += 125;
            }
            else {
                credits += 150;
            }
        }
    }

    int before = enemies.size();
    //  Eliminar enemigos muertos
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const std::shared_ptr<Enemy>& e) {
            return !e->isAlive();
        }), enemies.end());
    
    int after = enemies.size();
    std::cout << "[DEBUG] updateEnemies: eliminados " << (before - after)
        << ", quedan " << after << "\n";
    //std::cout << "[DEBUG] updateEnemies: Cantidad actual de enemigos vivos: " << enemies.size() << "\n";
    
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
    std::cout << "[EVOLUCION] Ejecutando evolucion...\n";

    if (enemies.empty()) {
        std::cout << "[EVOLUCION] La lista de enemigos esta vacia. No se puede evolucionar.\n";
        return;
    }

    std::vector<std::shared_ptr<Enemy>> base = lastGeneration;
    std::vector<std::shared_ptr<Enemy>> newGeneration;

    // Ordenar por fitness
    std::sort(enemies.begin(), enemies.end(), [](const auto& a, const auto& b) {
        return a->getFitness() > b->getFitness();
    });

    

    int topCount = enemies.size() / 2;
    if (topCount == 0) topCount = 1;

    std::cout << "[EVOLUCION] Enemigos ordenados por fitness:\n";
    for (const auto& e : enemies) {
        std::cout << "  - Tipo: " << e->getType() << ", Fitness: " << e->getFitness() << ", Vida: " << e->getHealth() << "\n";
    }

    for (int i = 0; i < enemies.size(); ++i) {
        const Enemy& p1 = *enemies[i % topCount];
        const Enemy& p2 = *enemies[(i + 1) % topCount];

        std::cout << "[CRUCE] Padre 1: " << p1.getType() << " (fit: " << p1.getFitness() << "), "
            << "Padre 2: " << p2.getType() << " (fit: " << p2.getFitness() << ")\n";

        auto child = crossover(p1, p2);
        mutate(child);
        newGeneration.push_back(child);
    }

    enemies = newGeneration;
    generation++;

    std::cout << "[EVOLUCION] Generacion " << generation << " creada con " << enemies.size() << " enemigos nuevos.\n";

    lastGeneration.clear();
}


std::shared_ptr<Enemy> EnemyManager::crossover(const Enemy& parent1, const Enemy& parent2) {
    sf::Vector2i start = { rand() % 25, 24 }; // posici�n random en fila 24

    // Promediar atributos
    float hp = (parent1.getFitness() + parent2.getFitness()) / 2.0f + 100.0f;
    float speed = (parent1.getSpeed() + parent2.getSpeed()) / 2.0f;
    float rArrow = (parent1.getResistanceArrows() + parent2.getResistanceArrows()) / 2.0f;
    float rMagic = (parent1.getResistanceMagic() + parent2.getResistanceMagic()) / 2.0f;
    float rArtillery = (parent1.getResistanceArtillery() + parent2.getResistanceArtillery()) / 2.0f;

    std::cout << "[CRUCE] Hijo generado con stats -> HP: " << hp << ", Velocidad: " << speed
        << ", ResArrow: " << rArrow << ", ResMagic: " << rMagic << ", ResArtillery: " << rArtillery << "\n";

    return std::make_shared<OgreEnemy>(start, hp);

}



void EnemyManager::mutate(std::shared_ptr<Enemy>& enemy) {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    float roll = dist(rng);
    std::cout << "[MUTACION] Valor aleatorio: " << roll << " (umbral: " << mutationRate << ")\n";


    if (roll < mutationRate) {
        auto names = evoManager.getAvailableNames();
        std::uniform_int_distribution<int> idxDist(0, names.size() - 1);

        const std::string& selectedName = names[idxDist(rng)];
        const EnemyEvolution& evo = evoManager.getEvolution(selectedName);

        enemy->addEvolution(evo);
        std::cout << "[MUTACION] Se aplico evolucion: " << selectedName << "\n";

    }else {
        std::cout << "[MUTACION] No se aplico evolucion.\n";
    }
}

std::shared_ptr<Enemy> EnemyManager::randomEnemy() {
    return std::make_shared<OgreEnemy>();
}

std::map<std::string, float> EnemyManager::getAverageFitnessPerType() const {
    std::map<std::string, float> totalFitness;
    std::map<std::string, int> counts;

    for (const auto& enemy : enemies) { 
        std::string type = enemy->getType();
        totalFitness[type] += enemy->getFitness();
        counts[type]++;
    }

    std::map<std::string, float> averageFitness;
    for (const auto& pair : totalFitness) {
        averageFitness[pair.first] = std::round((pair.second / counts[pair.first]) * 100.0f) / 100.0f;

    }

    return averageFitness;
}
