#ifndef GAME_HPP
#define GAME_HPP

#include "camera.hpp"
#include "diver.hpp"
#include "world.hpp"
#include <SFML/Graphics.hpp>

namespace Game {

class Game {
  public:
    Game();
    void run();

  private:
    void processEvents();
    void update(float deltaTime);
    void render();

    sf::RenderWindow m_window;
    Camera m_camera;
    Diver m_diver;
    World m_world;
};
} // namespace Game
#endif