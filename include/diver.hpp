#ifndef DIVER_HPP
#define DIVER_HPP

#include <SFML/Graphics.hpp>

namespace Game {

class Diver {
  public:
    Diver(sf::Vector2f startPos);
    void handleInput();
    void update(float deltaTime);
    void attack();
    void draw(sf::RenderWindow &window) const;
    sf::Vector2f getPosition() const;

  private:
    sf::Vector2f m_position;
};
} // namespace Game
#endif