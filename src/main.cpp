#include "utility.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

int main() {
    std::cout << Game::getInfo() << std::endl;

    sf::RenderWindow window(
        sf::VideoMode({(unsigned int)Game::WINDOW_WIDTH, (unsigned int)Game::WINDOW_HEIGHT}),
        "Game Test");
    window.setFramerateLimit(60);

    sf::CircleShape player(Game::PLAYER_RADIUS);
    player.setFillColor(sf::Color::Green);
    player.setOrigin({Game::PLAYER_RADIUS, Game::PLAYER_RADIUS});
    player.setPosition({Game::WINDOW_WIDTH / 2.0f, Game::WINDOW_HEIGHT / 2.0f});

    sf::Clock clock;

    sf::Vector2f diverVelocity(0.0f, 0.0f);
    const float waterFading = 0.87f;
    const float acceleration = 500.0f;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        float deltaTime = clock.restart().asSeconds();

        sf::Vector2f movingDirection(0.0f, 0.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            movingDirection.y -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            movingDirection.y += 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            movingDirection.x -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            movingDirection.x += 1.0f;

        if (movingDirection.x != 0.0f || movingDirection.y != 0.0f) {
            float length = std::sqrt(movingDirection.x * movingDirection.x +
                                     movingDirection.y * movingDirection.y);
            movingDirection /= length;

            sf::Vector2f targetVelocity = movingDirection * Game::PLAYER_SPEED;

            sf::Vector2f diff = targetVelocity - diverVelocity;
            float diffLength = std::sqrt(diff.x * diff.x + diff.y * diff.y);

            float maxChange = acceleration * deltaTime;

            if (diffLength <= maxChange) {
                diverVelocity = targetVelocity;
            } else {
                diverVelocity += (diff / diffLength) * maxChange;
            }
        } else {
            diverVelocity *= waterFading;
            if (std::abs(diverVelocity.x) < 1.0f && std::abs(diverVelocity.y) < 1.0f)
                diverVelocity = sf::Vector2f(0.0f, 0.0f);
        }
        player.move(diverVelocity * deltaTime);

        window.clear(sf::Color(30, 30, 30));
        window.draw(player);
        window.display();
    }

    return 0;
}