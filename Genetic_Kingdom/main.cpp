#include <SFML/Graphics.hpp>
#include <iostream>
#include "GameScreen.h"

int main() {

    // Pantalla de inicio
    sf::RenderWindow window(sf::VideoMode(800, 600), " Genetic Empire - Inicio");
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Genetic Kingdom.png")) {
        return -1;
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);


    // Fuente
    sf::Font font;
    if (!font.loadFromFile("Space.ttf")) {
        std::cerr << "No se pudo cargar la fuente.\n";
        return -1;
    }

    // Botón 1: Jugar
    sf::RectangleShape playButton(sf::Vector2f(175, 38));
    playButton.setPosition(150, 442);
    playButton.setFillColor(sf::Color::Transparent);

    sf::Text playText("Jugar", font, 24);
    playText.setPosition(203, 445);
    playText.setFillColor(sf::Color::Black);

    // Botón 2: Salir
    sf::RectangleShape exitButton(sf::Vector2f(175, 38));
    exitButton.setPosition(475, 442);
    exitButton.setFillColor(sf::Color::Transparent);

    sf::Text exitText("Salir", font, 24);
    exitText.setPosition(535, 445);
    exitText.setFillColor(sf::Color::Black);

    // Loop principal
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Detectar clics
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                std::cout << "Clic en: (" << mousePos.x << ", " << mousePos.y << ")\n";
                if (playButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                    window.close(); // cierra ventana de menú
                    runGame();      // abre nueva ventana de juego
                }

                if (exitButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                    window.close();
                }
            }
        }

        // Dibujar
        window.clear();
        window.draw(backgroundSprite);
        window.draw(playButton);
        window.draw(playText);
        window.draw(exitButton);
        window.draw(exitText);
        window.display();
    }

    return 0;
}
