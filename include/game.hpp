#ifndef GAME_HPP
#define GAME_HPP

#include "audio_manager.hpp"
#include "camera.hpp"
#include "diver.hpp"
#include "shop.hpp"
#include "world.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

namespace Game {

class Game {
  public:
    Game();
    void run();

  private:
    void processEvents();
    void update(float deltaTime);
    void render();
    void renderStartup();
    void renderShop();
    void renderMenuOverlay();
    void applyPersistentUpgrades();
    bool loadUiFont();

    enum State { Startup, MainMenu, ShopScreen, Playing, Paused, GameOver };

    void startNewRun();
    sf::FloatRect getCenterButtonRect(sf::Vector2f size, float yOffset) const;
    bool isPointInside(sf::Vector2f point, const sf::FloatRect &rect) const;

    float m_damageVignetteAlpha = 0.f;
    sf::RectangleShape m_vignette;

    sf::RenderWindow m_window;
    Camera m_camera;
    Diver m_diver;
    World m_world;
    Shop m_shop;
    AudioManager m_audio;

    sf::Texture m_bgTexture;
    sf::Texture m_cloudTexture;
    sf::Texture m_startupTexture;
    sf::Font m_uiFont;
    bool m_hasUiFont = false;
    bool m_hasStartupTexture = false;

    float m_bgParallax = 0.6f;
    float m_cloudParallax = 0.3f;
    State m_state = Startup;
    float m_diverDamageCooldown = 0.f;
    float m_menuOverlayAlpha = 0.f;

    float m_startupTimer = 0.f;
    float m_startupFade = 1.f;
    static constexpr float m_startupDuration = 2.f;

    float m_cooldownReduction = 0.f;
    float m_damageBonus = 0.f;

    std::unique_ptr<sf::Text> m_coinText;
    std::unique_ptr<sf::Text> m_hpText;
    void drawParallaxLayer(const sf::Texture &texture, const sf::View &view, float parallax,
                           sf::Color tint, bool mirror);
};
} // namespace Game
#endif
