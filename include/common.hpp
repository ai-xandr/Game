#ifndef COMMON_HPP
#define COMMON_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <vector>

namespace Game {
inline constexpr unsigned int WORLD_WIDTH = 2000;
inline constexpr unsigned int WORLD_HEIGHT = 2000;

enum class FishType { Prey, Predator, Neutral, Obstacle };

struct Upgrade {
    std::string name;
    int cost = 0;
    float speedMultiplier = 1.0f;
    int attackRangeBonus = 0;
    int attackDamageBonus = 0;
};

class Fish {
  public:
    Fish(sf::Vector2f startPos, float speed, int points, FishType type)
        : m_position(startPos), m_speed(speed), m_points(points), m_type(type) {
    }
    virtual ~Fish() = default;
    virtual void update(float deltaTime, sf::Vector2f diverPos,
                        const std::vector<Fish *> &allFishes) = 0;
    virtual void draw(sf::RenderWindow &window) = 0;
    virtual bool isColliding(sf::Vector2f point, float radius) const = 0;

    sf::Vector2f getPosition() const {
        return m_position;
    }
    int getPoints() const {
        return m_points;
    }
    bool isDead() const {
        return m_health <= 0;
    }
    FishType getType() const {
        return m_type;
    }

    void takeDamage(int damage) {
        m_health -= damage;
    }
    void setHorizontalDirection(float dirX) {
        if (dirX > 0.f)
            m_velocity.x = std::abs(m_speed);
        else if (dirX < 0.f)
            m_velocity.x = -std::abs(m_speed);
    }

  protected:
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    float m_speed;
    int m_health = 1;
    int m_points;
    FishType m_type;
    float m_baseScale = 1.0f;
    float m_wobbleTime = 0.f;
};

} // namespace Game

#endif