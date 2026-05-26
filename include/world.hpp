#ifndef WORLD_HPP
#define WORLD_HPP

#include "common.hpp"
#include <vector>

namespace Game {

class World {
  public:
    World(sf::FloatRect bounds);
    ~World();
    void update(float deltaTime, sf::Vector2f diverPos);
    void draw(sf::RenderWindow &window) const;
    std::vector<Fish *> &getFishes();

  private:
    std::vector<Fish *> m_fishes;
};
} // namespace Game
#endif