#include "EvolutionManager.h"
#include <iostream>

EvolutionManager::EvolutionManager() {
    evolutions.emplace("SpeedBoost", EnemyEvolution("SpeedBoost", 0, 0.5f, 0, 0, 0, "evo_speed.png"));
    evolutions.emplace("Tank", EnemyEvolution("Tank", 50, -0.2f, 0.2f, 0.1f, 0.1f, "evo_tank.png"));
    evolutions.emplace("MagicShield", EnemyEvolution("MagicShield", 0, 0.0f, 0, 0.4f, 0, "evo_magic.png"));
    evolutions.emplace("ArrowShell", EnemyEvolution("ArrowShell", 0, 0.0f, 0.4f, 0, 0, "evo_arrow.png"));
}

const EnemyEvolution& EvolutionManager::getEvolution(const std::string& name) const {
    auto it = evolutions.find(name);
    if (it == evolutions.end()) {
        std::cerr << "[ERROR] Evolución no encontrada: " << name << "\n";
        return evolutions.begin()->second; // fallback
    }
    return it->second;
}

std::vector<std::string> EvolutionManager::getAvailableNames() const {
    std::vector<std::string> names;
    for (const auto& [key, _] : evolutions) {
        names.push_back(key);
    }
    return names;
}
