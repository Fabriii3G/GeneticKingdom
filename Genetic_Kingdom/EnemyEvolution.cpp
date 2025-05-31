#include "EnemyEvolution.h"
#include <iostream>

EnemyEvolution::EnemyEvolution(const std::string& name,
    float hp, float speed,
    float rArrow, float rMagic, float rArt,
    const std::string& textureFile)
    : name(name),
    bonusHealth(hp),
    bonusSpeed(speed),
    bonusResArrows(rArrow),
    bonusResMagic(rMagic),
    bonusResArtillery(rArt) {

    if (!texture.loadFromFile("evo.png")) {
        std::cerr << "Error al cargar textura de evolución: " << textureFile << "\n";
    }
    else {
        spriteOverlay.setTexture(texture);
    }
}

void EnemyEvolution::applyTo(float& hp, float& speed, float& rA, float& rM, float& rT) const {
    hp += bonusHealth;
    speed += bonusSpeed;
    rA += bonusResArrows;
    rM += bonusResMagic;
    rT += bonusResArtillery;
}

const sf::Sprite& EnemyEvolution::getOverlaySprite() const {
    return spriteOverlay;
}

const std::string& EnemyEvolution::getName() const {
    return name;
}
