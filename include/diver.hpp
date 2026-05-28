#ifndef DIVER_HPP
#define DIVER_HPP

#include "animated_sprite.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

namespace Game {

class Diver {
  public:
    Diver(sf::Vector2f startPos);
    void handleInput();
    void update(float deltaTime, float seabedY);
    void attack();
    void draw(sf::RenderWindow &window) const;
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f pos);
    void takeDamage(int damage);
    bool isDead() const;
    void reset();
    int getHp() const;

    float getAttackRange() const;
    int getAttackDamage() const;

  private:
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    sf::Vector2f m_movingDirection;

    float m_maxSpeed = 200.0f;
    float m_acceleration = 500.0f;
    float m_waterFading = 0.87f;

    int m_hp = 10;
    int m_attackDamage = 1;
    float m_attackRange = 48.0f;
    bool m_attacking = false;
    float m_attackTimer = 0.0f;

    AnimatedSprite m_sprite;
    int m_currentState = 0;
};
} // namespace Game
#endif