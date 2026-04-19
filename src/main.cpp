#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(200, 200), "CI Test");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    std::cout << "SFML Window created successfully!" << std::endl;
    return 0; 
}