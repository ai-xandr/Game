#ifndef DIVER_HPP
#define DIVER_HPP

#include "animated_sprite.hpp"
#include <SFML/Graphics.hpp>

namespace Game {

class Diver {
  public:
    Diver(sf::Vector2f startPos);
    void handleInput();
    void update(float deltaTime, float seabedY);
    bool attack();
    void draw(sf::RenderWindow &window) const;
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f pos);
    void takeDamage(int damage);
    void resetVelocity();
    bool isDead() const;
    void reset();
    int getHp() const;

    float getAttackRange() const;
    int getAttackDamage() const;
    void setCooldownReduction(float reduction);
    void setDamageMultiplier(float multiplier);

  private:
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    sf::Vector2f m_movingDirection;

    float m_maxSpeed = 200.0f;
    float m_acceleration = 500.0f;
    float m_waterFading = 0.87f;

    int m_hp = 10;
    int m_baseAttackDamage = 1;
    float m_attackRange = 48.0f;
    float m_cooldownReduction = 0.f;
    float m_damageMultiplier = 1.f;
    static constexpr float m_baseAttackCooldown = 0.25f;
    bool m_attacking = false;
    float m_attackTimer = 0.0f;
    float m_attackCooldown = 0.f;
    static constexpr float ATTACK_COOLDOWN = 0.5f;

    AnimatedSprite m_sprite;
    int m_currentState = 0;
    int m_appliedAnimationState = -1;

    float m_baseScale = 1.0f;
    float m_swimScaleFactor = 1.3f;
};
} // namespace Game
#endif