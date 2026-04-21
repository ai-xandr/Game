#include "utility.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    std::cout << Game::getInfo() << std::endl;

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(400, 200), "DevOps CI Test");

    std::cout << "SFML Window initialized. Closing..." << std::endl;

    if (window.isOpen()) {
        window.close();
    }

    return 0;
}