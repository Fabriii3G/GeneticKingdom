#ifndef DARKELFENEMY_H
#define DARKELFENEMY_H

#include "Enemy.h"

class DarkElfEnemy : public Enemy {
private:
    sf::Sprite sprite;
    sf::Texture texture;

public:
    DarkElfEnemy(sf::Vector2i startPos = { 0, 24 }, float hp = 75.0f);
    void move() override;
    std::string getType() const override;
    void draw(sf::RenderWindow& window, int tileSize, sf::Vector2f offset) const override;
};

#endif
