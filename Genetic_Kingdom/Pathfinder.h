#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <SFML/System.hpp>
#include <vector>

std::vector<sf::Vector2i> findPathAStar(const sf::Vector2i& start, const sf::Vector2i& goal, const int grid[25][25]);

#endif
