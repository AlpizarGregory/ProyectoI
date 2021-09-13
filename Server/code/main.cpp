#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Breakout");
    while (window.isOpen()) {
        sf::Event event{};{
        }
        while (window.pollEvent(event)) {
            //Ejemplo de mousemovement dentro de la pantalla
            if (event.type == sf::Event::MouseMoved)
                std::cout << "new mouse x: " << event.mouseMove.x << std::endl;
            std::cout << "new mouse y: " << event.mouseMove.y << std::endl;

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    std::cout << "the escape key was pressed" << std::endl;
                }}
            if (event.type == sf::Event::Closed)
                        window.close();

                }
            }
            return 0;
        }