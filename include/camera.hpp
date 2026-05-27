#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>

namespace Game {

class Camera {
  public:
    Camera(sf::FloatRect worldBounds, sf::Vector2f viewSize);
    void update(sf::Vector2f target);
    sf::View getView() const;

  private:
    sf::View m_view;
    sf::FloatRect m_bounds;
};
} // namespace Game
#endif