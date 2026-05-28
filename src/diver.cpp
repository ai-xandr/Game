#include "diver.hpp"
#include <algorithm>
#include <cmath>

namespace Game {

Diver::Diver(sf::Vector2f startPos)
    : m_position(startPos), m_velocity(0, 0), m_movingDirection(0, 0) {
    if (!m_sprite.loadFromFile("assets/leha_vodnik/spritesheet_diver.png")) {
        m_sprite.loadFromFile("assets/spritesheet_diver.png");
    }
    m_sprite.setFrameStart(0);
    m_sprite.setFrameCount(3);
    m_sprite.setPingPong(true);
    m_sprite.setFrameSize({765.f, 1528.f});
    m_sprite.setFrameTime(0.75f);
    constexpr float targetHeight = 200.f;
    const float uniformScale = targetHeight / 1528.f;
    m_baseScale = uniformScale;
    m_sprite.setScale({uniformScale, uniformScale});
    m_sprite.setPosition(m_position);
    m_sprite.setOrigin({765.f * 0.5f, 1528.f * 0.5f});
}

void Diver::handleInput() {
    m_movingDirection = sf::Vector2f(0.f, 0.f);
#if defined(SFML_VERSION_MAJOR) && (SFML_VERSION_MAJOR >= 3)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        m_movingDirection.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        m_movingDirection.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        m_movingDirection.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        m_movingDirection.x += 1.f;
#else
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        m_movingDirection.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        m_movingDirection.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        m_movingDirection.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        m_movingDirection.x += 1.f;
#endif
}

void Diver::update(float deltaTime, float seabedY) {
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
        m_currentState = 1;
    } else {
        m_velocity *= m_waterFading;
        if (std::abs(m_velocity.x) < 1.0f && std::abs(m_velocity.y) < 1.0f) {
            m_velocity = sf::Vector2f(0.0f, 0.0f);
            m_currentState = 0;
        }
    }

    m_position += m_velocity * deltaTime;
    const float diverHalfSize = 32.f;
    m_position.y = std::min(m_position.y, seabedY - diverHalfSize);
    m_sprite.setPosition(m_position);

    if (std::abs(m_velocity.x) > 1.f || std::abs(m_velocity.y) > 1.f) {
        float angle = std::atan2(m_velocity.y, m_velocity.x) * 180.f / 3.14159f;
        m_sprite.setRotation(angle + 90.f);
    }

    if (m_attackCooldown > 0.f)
        m_attackCooldown -= deltaTime;

    if (m_attacking) {
        m_attackTimer -= deltaTime;
        if (m_attackTimer <= 0.0f)
            m_attacking = false;
    }

    if (m_appliedAnimationState != m_currentState) {
        if (m_currentState == 0) {
            m_sprite.setScale({m_baseScale, m_baseScale});
            m_sprite.setFrameStart(0); // frame_idle_0..2
            m_sprite.setFrameCount(3);
            m_sprite.setPingPong(true);
        } else {
            float swimScale = m_baseScale * m_swimScaleFactor;
            m_sprite.setScale({swimScale, swimScale});
            m_sprite.setFrameStart(3); // frame_swim_0..3
            m_sprite.setFrameCount(4);
            m_sprite.setPingPong(true);
        }
        m_appliedAnimationState = m_currentState;
    }

    m_sprite.update(deltaTime);
}

bool Diver::attack() {
    if (!m_attacking && m_attackCooldown <= 0.f) {
        m_attacking = true;
        m_attackTimer = 0.25f;
        m_attackCooldown = ATTACK_COOLDOWN;
        return true;
    }
    return false;
}

void Diver::draw(sf::RenderWindow &window) const {
    m_sprite.draw(window);
}

sf::Vector2f Diver::getPosition() const {
    return m_position;
}

void Diver::setPosition(sf::Vector2f pos) {
    m_position = pos;
    m_velocity = sf::Vector2f(0.f, 0.f);
    m_movingDirection = sf::Vector2f(0.f, 0.f);
    m_sprite.setPosition(m_position);
}

void Diver::takeDamage(int damage) {
    m_hp = std::max(0, m_hp - damage);
}

bool Diver::isDead() const {
    return m_hp <= 0;
}

void Diver::reset() {
    m_hp = 10;
    m_attacking = false;
    m_attackTimer = 0.f;
    m_velocity = sf::Vector2f(0.f, 0.f);
    m_movingDirection = sf::Vector2f(0.f, 0.f);
}

int Diver::getHp() const {
    return m_hp;
}

float Diver::getAttackRange() const {
    return m_attackRange;
}

int Diver::getAttackDamage() const {
    return m_attackDamage;
}

void Diver::resetVelocity() {
    m_velocity = sf::Vector2f(0.f, 0.f);
}

} // namespace Game
