#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class EnemyEvolution {
private:
    std::string name;
    float bonusHealth;
    float bonusSpeed;
    float bonusResArrows;
    float bonusResMagic;
    float bonusResArtillery;

    sf::Texture texture;
    sf::Sprite spriteOverlay;

public:
    EnemyEvolution(const std::string& name,
        float hp, float speed,
        float rArrow, float rMagic, float rArt,
        const std::string& textureFile);

    void applyTo(float& hp, float& speed, float& rA, float& rM, float& rT) const;

    const sf::Sprite& getOverlaySprite() const;
    const std::string& getName() const;
};
