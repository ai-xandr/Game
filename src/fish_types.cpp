#include "fish_types.hpp"
#include "common.hpp"
#include <algorithm>
#include <cmath>

namespace Game {

GoldFish::GoldFish(sf::Vector2f startPos) : Fish(startPos, 60.f, 3, FishType::Prey) {
    m_sprite.loadFromFile("assets/leha_vodnik/fish1.png");
    sf::Vector2u fishW = m_sprite.getTextureSize();
    m_sprite.setFrameCount(1);
    m_sprite.setFrameSize({static_cast<float>(fishW.x), static_cast<float>(fishW.y)});
    m_sprite.setFrameTime(0.f);

    const float targetWidth = 80.f;
    m_baseScale = targetWidth / fishW.x;
    m_sprite.setScale({m_baseScale, m_baseScale});

    m_sprite.setPosition(m_position);
    m_sprite.setOrigin({fishW.x / 2.f, fishW.y / 2.f});
    m_velocity = sf::Vector2f(m_speed, 0.f);

    m_health = 2;
}

void GoldFish::update(float deltaTime, sf::Vector2f diverPos,
                      const std::vector<Fish *> &allFishes) {
    const float t = m_position.x * 0.01f + m_position.y * 0.02f;
    m_velocity.y = std::sin(t) * 24.f;
    if (std::abs(diverPos.x - m_position.x) < 120.f) {
        m_velocity.x = (m_position.x < diverPos.x ? -m_speed : m_speed) * 1.2f;
    } else if (std::abs(m_velocity.x) < 0.1f) {
        m_velocity.x = m_speed;
    }
    m_position += m_velocity * deltaTime;

    m_wobbleTime += deltaTime;
    float wobble = std::sin(m_wobbleTime * 8.f) * 0.04f;

    if (m_velocity.x < -0.1f)
        m_sprite.setScale({-m_baseScale, m_baseScale * (1.f + wobble)});
    else if (m_velocity.x > 0.1f)
        m_sprite.setScale({m_baseScale, m_baseScale * (1.f + wobble)});

    m_sprite.setPosition(m_position);
    m_sprite.update(deltaTime);
}

void GoldFish::draw(sf::RenderWindow &window) {
    m_sprite.draw(window);
}

bool GoldFish::isColliding(sf::Vector2f point, float radius) const {
    float dx = m_position.x - point.x;
    float dy = m_position.y - point.y;
    float r = 20.f + radius;
    return dx * dx + dy * dy <= r * r;
}

Tuna::Tuna(sf::Vector2f startPos) : Fish(startPos, 90.f, 5, FishType::Prey) {
    m_sprite.loadFromFile("assets/leha_vodnik/fish2.png");
    sf::Vector2u fishW = m_sprite.getTextureSize();
    m_sprite.setFrameCount(1);
    m_sprite.setFrameSize({static_cast<float>(fishW.x), static_cast<float>(fishW.y)});
    m_sprite.setFrameTime(0.f);
    const float targetWidth = 100.f;
    m_baseScale = targetWidth / fishW.x;
    m_sprite.setScale({m_baseScale, m_baseScale});

    m_sprite.setPosition(m_position);
    m_sprite.setOrigin({fishW.x / 2.f, fishW.y / 2.f});
    m_velocity = sf::Vector2f(m_speed, 0.f);

    m_health = 4;
}

void Tuna::update(float deltaTime, sf::Vector2f diverPos, const std::vector<Fish *> &allFishes) {
    const float t = m_position.x * 0.007f + m_position.y * 0.017f;
    m_velocity.y = std::sin(t) * 35.f;
    if (std::abs(diverPos.x - m_position.x) < 140.f) {
        m_velocity.x = (m_position.x < diverPos.x ? -m_speed : m_speed) * 1.3f;
    }
    m_position += m_velocity * deltaTime;

    m_wobbleTime += deltaTime;
    float wobble = std::sin(m_wobbleTime * 7.f) * 0.03f;

    if (m_velocity.x < -0.1f)
        m_sprite.setScale({-m_baseScale, m_baseScale * (1.f + wobble)});
    else if (m_velocity.x > 0.1f)
        m_sprite.setScale({m_baseScale, m_baseScale * (1.f + wobble)});

    m_sprite.setPosition(m_position);
    m_sprite.update(deltaTime);
}

void Tuna::draw(sf::RenderWindow &window) {
    m_sprite.draw(window);
}

bool Tuna::isColliding(sf::Vector2f point, float radius) const {
    float dx = m_position.x - point.x;
    float dy = m_position.y - point.y;
    float r = 25.f + radius;
    return dx * dx + dy * dy <= r * r;
}

Shark::Shark(sf::Vector2f startPos) : Fish(startPos, 150.f, 15, FishType::Predator) {
    m_sprite.loadFromFile("assets/leha_vodnik/fish3.png");
    sf::Vector2u fishW = m_sprite.getTextureSize();
    m_sprite.setFrameCount(1);
    m_sprite.setFrameSize({static_cast<float>(fishW.x), static_cast<float>(fishW.y)});
    m_sprite.setFrameTime(0.f);
    const float targetWidth = 260.f;
    m_baseScale = targetWidth / fishW.x;
    m_sprite.setScale({m_baseScale, m_baseScale});

    m_sprite.setPosition(m_position);
    m_sprite.setOrigin({fishW.x / 2.f, fishW.y / 2.f});
    m_velocity = sf::Vector2f(m_speed, 0.f);

    m_health = 7;
}

void Shark::update(float deltaTime, sf::Vector2f diverPos, const std::vector<Fish *> &allFishes) {
    float dx = diverPos.x - m_position.x;
    float dy = diverPos.y - m_position.y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist < 200.f) {
        if (dist > 10.f) {
            dx /= dist;
            dy /= dist;
            m_velocity.x = dx * m_speed;
            m_velocity.y = dy * (m_speed * 0.5f);
        }
    } else {
        m_velocity.x = m_speed * (m_velocity.x >= 0.f ? 1.f : -1.f);
        m_velocity.y *= 0.95f;
    }

    m_position += m_velocity * deltaTime;

    m_wobbleTime += deltaTime;
    float wobble = std::sin(m_wobbleTime * 6.f) * 0.035f;

    if (m_velocity.x > 0.1f)
        m_sprite.setScale({-m_baseScale, m_baseScale * (1.f + wobble)});
    else if (m_velocity.x < -0.1f)
        m_sprite.setScale({m_baseScale, m_baseScale * (1.f + wobble)});

    m_sprite.setPosition(m_position);
    m_sprite.update(deltaTime);
}

void Shark::draw(sf::RenderWindow &window) {
    m_sprite.draw(window);
}

bool Shark::isColliding(sf::Vector2f point, float radius) const {
    float dx = m_position.x - point.x;
    float dy = m_position.y - point.y;
    float r = 30.f + radius;
    return dx * dx + dy * dy <= r * r;
}

} // namespace Game
