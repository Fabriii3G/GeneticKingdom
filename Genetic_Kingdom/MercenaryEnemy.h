#ifndef MERCENARYENEMY_H
#define MERCENARYENEMY_H

#include "Enemy.h"

class MercenaryEnemy : public Enemy {
private:
    sf::Sprite sprite;
    sf::Texture texture;

public:
    MercenaryEnemy(sf::Vector2i startPos = { 0, 24 }, float hp = 120.0f);
    void move(float deltaTime) override;
    std::string getType() const override;
    void draw(sf::RenderWindow& window, int tileSize, sf::Vector2f offset) const override;
};

#endif
