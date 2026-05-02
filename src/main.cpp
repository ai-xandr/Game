#include "utility.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

int main() {
    std::cout << Game::getInfo() << std::endl;

    sf::RenderWindow window(sf::VideoMode(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT), "Game Test");
    window.setFramerateLimit(60);

    sf::CircleShape player(Game::PLAYER_RADIUS);
    player.setFillColor(sf::Color::Green);
    player.setOrigin(Game::PLAYER_RADIUS, Game::PLAYER_RADIUS);
    player.setPosition(Game::WINDOW_WIDTH / 2.0f, Game::WINDOW_HEIGHT / 2.0f);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();
        sf::Vector2f movement(0.0f, 0.0f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            movement.y -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            movement.y += 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            movement.x -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            movement.x += 1.0f;

        if (movement.x != 0.0f || movement.y != 0.0f) {
            float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
            movement /= length;
            player.move(movement * Game::PLAYER_SPEED * deltaTime);
        }

        window.clear(sf::Color(30, 30, 30));
        window.draw(player);
        window.display();
    }

    return 0;
}