#include "game.hpp"

namespace Game {

Game::Game()
    : m_window(sf::VideoMode({800, 600}), "David the Vodolaz"),
      m_camera(sf::FloatRect({0.f, 0.f}, {2000.f, 2000.f}), {800.f, 600.f}),
      m_diver(sf::Vector2f(1000.f, 1000.f)), m_world(sf::FloatRect({0.f, 0.f}, {2000.f, 2000.f})) {
    m_window.setFramerateLimit(60);
}

void Game::run() {
    while (m_window.isOpen()) {
        while (const std::optional event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                m_window.close();
        }

        m_window.clear(sf::Color(30, 30, 30));
        m_window.display();
    }
}

} // namespace Game