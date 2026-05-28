#include "fish_types.hpp"
#include "common.hpp"
#include <algorithm>
#include <cmath>

namespace Game {

GoldFish::GoldFish(sf::Vector2f startPos) : Fish(startPos, 60.f, 3, FishType::Prey) {
    m_sprite.loadFromFile("assets/sprites/fish/fish.png");
    const unsigned int fishW = m_sprite.getTextureSize().x;
    m_sprite.setFrameCount(std::max(1u, fishW / 40u));
    m_sprite.setFrameSize({40.f, 40.f});
    m_sprite.setFrameTime(0.14f);
    m_sprite.setPosition(m_position);
    m_sprite.setOrigin({20.f, 20.f});
    m_velocity = sf::Vector2f(m_speed, 0.f);
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

    if (m_velocity.x < -0.1f)
        m_sprite.setScale({-1.f, 1.f});
    else if (m_velocity.x > 0.1f)
        m_sprite.setScale({1.f, 1.f});

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
    m_sprite.loadFromFile("assets/sprites/fish/fish-big.png");
    const unsigned int fishW = m_sprite.getTextureSize().x;
    m_sprite.setFrameCount(std::max(1u, fishW / 50u));
    m_sprite.setFrameSize({50.f, 40.f});
    m_sprite.setFrameTime(0.1f);
    m_sprite.setPosition(m_position);
    m_sprite.setOrigin({25.f, 20.f});
    m_velocity = sf::Vector2f(m_speed, 0.f);
}

void Tuna::update(float deltaTime, sf::Vector2f diverPos, const std::vector<Fish *> &allFishes) {
    const float t = m_position.x * 0.007f + m_position.y * 0.017f;
    m_velocity.y = std::sin(t) * 35.f;
    if (std::abs(diverPos.x - m_position.x) < 180.f) {
        m_velocity.x = (m_position.x < diverPos.x ? -m_speed : m_speed) * 1.3f;
    }
    m_position += m_velocity * deltaTime;

    if (m_velocity.x < -0.1f)
        m_sprite.setScale({-1.f, 1.f});
    else if (m_velocity.x > 0.1f)
        m_sprite.setScale({1.f, 1.f});

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
    m_sprite.loadFromFile("assets/sprites/fish/mine-big.png");
    const unsigned int fishW = m_sprite.getTextureSize().x;
    m_sprite.setFrameCount(std::max(1u, fishW / 60u));
    m_sprite.setFrameSize({60.f, 50.f});
    m_sprite.setFrameTime(0.09f);
    m_sprite.setPosition(m_position);
    m_sprite.setOrigin({30.f, 25.f});
    m_velocity = sf::Vector2f(m_speed * 0.8f, 0.f);
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

    if (m_velocity.x < -0.1f)
        m_sprite.setScale({-1.f, 1.f});
    else if (m_velocity.x > 0.1f)
        m_sprite.setScale({1.f, 1.f});

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

Crab::Crab(sf::Vector2f startPos) : Fish(startPos, 80.f, 4, FishType::Neutral) {
    m_sprite.loadFromFile("assets/sprites/fish/mine.png");
    const unsigned int fishW = m_sprite.getTextureSize().x;
    m_sprite.setFrameCount(std::max(1u, fishW / 35u));
    m_sprite.setFrameSize({35.f, 30.f});
    m_sprite.setFrameTime(0.2f);
    m_sprite.setPosition(m_position);
    m_sprite.setOrigin({17.5f, 15.f});
    m_velocity = sf::Vector2f(m_speed * 0.45f, 0.f);
}

void Crab::update(float deltaTime, sf::Vector2f diverPos, const std::vector<Fish *> &allFishes) {
    if (std::abs(diverPos.x - m_position.x) < 110.f)
        m_velocity.x = (m_position.x < diverPos.x ? -1.f : 1.f) * (m_speed * 0.7f);

    m_position += m_velocity * deltaTime;
    m_position.y += std::sin(m_position.x * 0.004f) * 0.6f;

    if (m_velocity.x < -0.1f)
        m_sprite.setScale({-1.f, 1.f});
    else if (m_velocity.x > 0.1f)
        m_sprite.setScale({1.f, 1.f});

    m_sprite.setPosition(m_position);
    m_sprite.update(deltaTime);
}

void Crab::draw(sf::RenderWindow &window) {
    m_sprite.draw(window);
}

bool Crab::isColliding(sf::Vector2f point, float radius) const {
    float dx = m_position.x - point.x;
    float dy = m_position.y - point.y;
    float r = 17.5f + radius;
    return dx * dx + dy * dy <= r * r;
}

Shell::Shell(sf::Vector2f startPos) : Fish(startPos, 40.f, 2, FishType::Obstacle) {
    m_sprite.loadFromFile("assets/sprites/fish/mine-small.png");
    const unsigned int fishW = m_sprite.getTextureSize().x;
    m_sprite.setFrameCount(std::max(1u, fishW / 25u));
    m_sprite.setFrameSize({25.f, 25.f});
    m_sprite.setFrameTime(0.26f);
    m_sprite.setPosition(m_position);
    m_sprite.setOrigin({12.5f, 12.5f});
    m_velocity = sf::Vector2f(8.f, 0.f);
}

void Shell::update(float deltaTime, sf::Vector2f diverPos, const std::vector<Fish *> &allFishes) {
    m_position.x += std::sin(m_position.y * 0.03f) * 3.f * deltaTime;
    if (m_velocity.x < -0.1f)
        m_sprite.setScale({-1.f, 1.f});
    else
        m_sprite.setScale({1.f, 1.f});
    m_sprite.setPosition(m_position);
    m_sprite.update(deltaTime);
}

void Shell::draw(sf::RenderWindow &window) {
    m_sprite.draw(window);
}

bool Shell::isColliding(sf::Vector2f point, float radius) const {
    float dx = m_position.x - point.x;
    float dy = m_position.y - point.y;
    float r = 12.5f + radius;
    return dx * dx + dy * dy <= r * r;
}

Algae::Algae(sf::Vector2f startPos) : Fish(startPos, 30.f, 0, FishType::Obstacle) {
    m_sprite.loadFromFile("assets/sprites/fish/fish-dart.png");
    const unsigned int fishW = m_sprite.getTextureSize().x;
    m_sprite.setFrameCount(std::max(1u, fishW / 20u));
    m_sprite.setFrameSize({20.f, 40.f});
    m_sprite.setFrameTime(0.3f);
    m_sprite.setPosition(m_position);
    m_sprite.setOrigin({10.f, 20.f});
    m_velocity = sf::Vector2f(0.f, 0.f);
}

void Algae::update(float deltaTime, sf::Vector2f diverPos, const std::vector<Fish *> &allFishes) {
    m_position.x += std::sin(m_position.y * 0.02f) * 1.2f * deltaTime;
    m_sprite.setPosition(m_position);
    m_sprite.update(deltaTime);
}

void Algae::draw(sf::RenderWindow &window) {
    m_sprite.draw(window);
}

bool Algae::isColliding(sf::Vector2f point, float radius) const {
    float dx = m_position.x - point.x;
    float dy = m_position.y - point.y;
    float r = 10.f + radius;
    return dx * dx + dy * dy <= r * r;
}

} // namespace Game
