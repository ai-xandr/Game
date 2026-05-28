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
    enum State { MainMenu, Playing, Paused, GameOver };
    void startNewRun();
    sf::FloatRect getCenterButtonRect(sf::Vector2f size, float yOffset) const;
    bool isPointInside(sf::Vector2f point, const sf::FloatRect &rect) const;

    sf::RenderWindow m_window;
    Camera m_camera;
    Diver m_diver;
    World m_world;
    sf::Texture m_bgTexture;
    sf::Texture m_cloudTexture;
    sf::Font m_uiFont;
    bool m_hasUiFont = false;
    float m_bgParallax = 0.6f;
    float m_cloudParallax = 0.3f;
    State m_state = MainMenu;
    float m_diverDamageCooldown = 0.f;
    float m_menuOverlayAlpha = 0.f;
    void drawParallaxLayer(const sf::Texture &texture, const sf::View &view, float parallax,
                           sf::Color tint, bool mirror);
};
} // namespace Game
#endif