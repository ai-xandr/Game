#include "game.hpp"

namespace Game {

Game::Game()
    : m_window(sf::VideoMode({800, 600}), "Underwater Game"),
      m_camera(sf::FloatRect({0.f, 0.f}, {2000.f, 2000.f}), {800.f, 600.f}),
      m_diver(sf::Vector2f(1000.f, 1000.f)), m_world(sf::FloatRect({0.f, 0.f}, {2000.f, 2000.f})) {
    m_window.setFramerateLimit(60);
}

void Game::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    while (const std::optional event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            m_window.close();
    }
}

void Game::update(float deltaTime) {
    m_diver.handleInput();
    m_diver.update(deltaTime);
    m_camera.update(m_diver.getPosition());
    m_world.update(deltaTime, m_diver.getPosition());
}

void Game::render() {
    m_window.clear(sf::Color(30, 30, 30));

    m_window.setView(m_camera.getView());
    m_world.draw(m_window); // для рыб
    m_diver.draw(m_window);

    m_window.display();
}

} // namespace Game