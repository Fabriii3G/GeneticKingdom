#include "Pathfinder.h"
#include <queue>
#include <unordered_map>
#include <cmath>

namespace std {
    template <>
    struct hash<sf::Vector2i> {
        size_t operator()(const sf::Vector2i& v) const noexcept {
            return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
        }
    };

    template <>
    struct equal_to<sf::Vector2i> {
        bool operator()(const sf::Vector2i& lhs, const sf::Vector2i& rhs) const {
            return lhs.x == rhs.x && lhs.y == rhs.y;
        }
    };
}

struct Node {
    sf::Vector2i pos;
    float g, f;

    bool operator>(const Node& other) const {
        return f > other.f;
    }
};

float heuristic(const sf::Vector2i& a, const sf::Vector2i& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y); // Distancia Manhattan
}

bool isValid(const sf::Vector2i& pos, const int grid[25][25]) {
    if (pos.x < 0 || pos.x >= 25 || pos.y < 0 || pos.y >= 25)
        return false;

    int cell = grid[pos.y][pos.x];
    return (cell == 0 || cell == 5 || cell == 10); // 0: libre, 5: castillo, 10: enemigo (opcional)
}


std::vector<sf::Vector2i> reconstructPath(const std::unordered_map<sf::Vector2i, sf::Vector2i>& cameFrom, sf::Vector2i current) {
    std::vector<sf::Vector2i> path;
    while (cameFrom.find(current) != cameFrom.end()) {
        path.push_back(current);
        current = cameFrom.at(current);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<sf::Vector2i> findPathAStar(const sf::Vector2i& start, const sf::Vector2i& goal, const int grid[25][25]) {
    std::priority_queue<Node, std::vector<Node>, std::greater<>> openSet;
    std::unordered_map<sf::Vector2i, sf::Vector2i> cameFrom;
    std::unordered_map<sf::Vector2i, float> gScore;

    auto hash = [](const sf::Vector2i& p) {
        return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
        };

    auto eq = [](const sf::Vector2i& a, const sf::Vector2i& b) {
        return a.x == b.x && a.y == b.y;
        };

    std::unordered_map<sf::Vector2i, float, decltype(hash), decltype(eq)> fScore(0, hash, eq);

    gScore[start] = 0;
    fScore[start] = heuristic(start, goal);
    openSet.push({ start, 0, fScore[start] });

    const std::vector<sf::Vector2i> directions = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0}
    };

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        if (current.pos == goal)
            return reconstructPath(cameFrom, current.pos);

        for (const auto& dir : directions) {
            sf::Vector2i neighbor = current.pos + dir;

            if (!isValid(neighbor, grid)) continue;

            float tentativeG = gScore[current.pos] + 1;

            if (!gScore.count(neighbor) || tentativeG < gScore[neighbor]) {
                cameFrom[neighbor] = current.pos;
                gScore[neighbor] = tentativeG;
                float f = tentativeG + heuristic(neighbor, goal);
                fScore[neighbor] = f;
                openSet.push({ neighbor, tentativeG, f });
            }
        }
    }

    return {}; // no path found
}
