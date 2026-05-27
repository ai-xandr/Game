#ifndef DIVER_HPP
#define DIVER_HPP

#include <SFML/Graphics.hpp>
#include <memory>

namespace Game {

class Diver {
  public:
    Diver(sf::Vector2f startPos);
    void handleInput();
    void update(float deltaTime);
    void attack();
    void draw(sf::RenderWindow &window) const;
    sf::Vector2f getPosition() const;

    void setTexture(const sf::Texture &texture);

  private:
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    sf::Vector2f m_movingDirection;

    float m_maxSpeed = 200.0f;
    float m_acceleration = 500.0f;
    float m_waterFading = 0.87f;

    sf::Texture m_placeholderTexture;
    std::unique_ptr<sf::Sprite> m_sprite;
};
} // namespace Game
#endif