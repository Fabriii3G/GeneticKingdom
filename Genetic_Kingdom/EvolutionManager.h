#pragma once
#include "EnemyEvolution.h"
#include <vector>
#include <string>
#include <unordered_map>

class EvolutionManager {
private:
    std::unordered_map<std::string, EnemyEvolution> evolutions;

public:
    EvolutionManager();

    const EnemyEvolution& getEvolution(const std::string& name) const;
    std::vector<std::string> getAvailableNames() const;
};
