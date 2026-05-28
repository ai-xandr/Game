#ifndef ROCK_HPP
#define ROCK_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace Game {

class Rock {
  public:
    Rock(sf::Vector2f position, float radius);
    void draw(sf::RenderWindow &window) const;
    bool checkCollision(const sf::Vector2f &point, float radius) const;
    sf::Vector2f getPosition() const;
    float getRadius() const;

  private:
    sf::Vector2f m_position;
    float m_radius;
    sf::ConvexShape m_shape;
    void generateRandomShape();
    sf::Texture m_texture;
    std::unique_ptr<sf::Sprite> m_sprite;
    bool m_hasTexture = false;
};
} // namespace Game
#endif