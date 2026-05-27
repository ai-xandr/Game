#include "diver.hpp"
#include "common.hpp"
#include <cmath>

namespace Game {

Diver::Diver(sf::Vector2f startPos)
    : m_position(startPos), m_velocity(0, 0), m_movingDirection(0, 0) {
    const unsigned int size = 64;
    sf::RenderTexture renderTex(sf::Vector2u(size, size));
    renderTex.clear(sf::Color::Transparent);

    sf::CircleShape circle(size / 2.f);
    circle.setFillColor(sf::Color::Green);
    circle.setOrigin({size / 2.f, size / 2.f});
    circle.setPosition({size / 2.f, size / 2.f});
    renderTex.draw(circle);
    renderTex.display();

    m_placeholderTexture = renderTex.getTexture();
    m_sprite = std::make_unique<sf::Sprite>(m_placeholderTexture);
    m_sprite->setOrigin({size / 2.f, size / 2.f});
    m_sprite->setPosition(m_position);
}

void Diver::handleInput() {
    m_movingDirection = sf::Vector2f(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        m_movingDirection.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        m_movingDirection.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        m_movingDirection.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        m_movingDirection.x += 1.f;
}

void Diver::update(float deltaTime) {
    if (m_movingDirection.x != 0.0f || m_movingDirection.y != 0.0f) {
        float length = std::sqrt(m_movingDirection.x * m_movingDirection.x +
                                 m_movingDirection.y * m_movingDirection.y);
        m_movingDirection /= length;
        sf::Vector2f targetVelocity = m_movingDirection * m_maxSpeed;

        sf::Vector2f diff = targetVelocity - m_velocity;
        float diffLength = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        float maxChange = m_acceleration * deltaTime;

        if (diffLength <= maxChange) {
            m_velocity = targetVelocity;
        } else {
            m_velocity += (diff / diffLength) * maxChange;
        }
    } else {
        m_velocity *= m_waterFading;
        if (std::abs(m_velocity.x) < 1.0f && std::abs(m_velocity.y) < 1.0f)
            m_velocity = sf::Vector2f(0.0f, 0.0f);
    }

    m_position += m_velocity * deltaTime;
    m_position.x = std::clamp(m_position.x, 0.f, static_cast<float>(Game::WORLD_WIDTH));
    m_position.y = std::clamp(m_position.y, 0.f, static_cast<float>(Game::WORLD_HEIGHT));
    m_sprite->setPosition(m_position);

    if (std::abs(m_velocity.x) > 1.f || std::abs(m_velocity.y) > 1.f) {
        float angle = std::atan2(m_velocity.y, m_velocity.x) * 180.f / 3.14159f;
        m_sprite->setRotation(sf::degrees(angle));
    }
}

void Diver::attack() {
    //нож
}

void Diver::draw(sf::RenderWindow &window) const {
    window.draw(*m_sprite);
}

sf::Vector2f Diver::getPosition() const {
    return m_position;
}

void Diver::setTexture(const sf::Texture &texture) {
    m_sprite->setTexture(texture, true);
    m_sprite->setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});
}

} // namespace Game