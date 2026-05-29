#ifndef ROCK_HPP
#define ROCK_HPP

#include <SFML/Graphics.hpp>
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
    void generateRandomShape();
    sf::Vector2f m_position;
    float m_radius;
    std::vector<sf::Vector2f> m_points;
    sf::VertexArray m_vertices;
    sf::Texture m_texture;
    bool m_hasTexture = false;
};

} // namespace Game

#endif