#include "game.hpp"
#include "utility.hpp"
#include <SFML/Config.hpp>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>

namespace Game {

sf::FloatRect Game::getCenterButtonRect(sf::Vector2f size, float yOffset) const {
    const sf::Vector2f ws(static_cast<float>(m_window.getSize().x),
                          static_cast<float>(m_window.getSize().y));
    const sf::Vector2f center(ws.x * 0.5f, ws.y * 0.5f + yOffset);
    return sf::FloatRect(center - size * 0.5f, size);
}

bool Game::isPointInside(sf::Vector2f point, const sf::FloatRect &rect) const {
    return rect.contains(point);
}

Game::Game()
    : m_window(sf::VideoMode({800, 600}), "David the Vodolaz", sf::Style::Default),
      m_camera(sf::FloatRect({0.f, 0.f}, {2000.f, 2000.f}), {800.f, 600.f}),
      m_diver(sf::Vector2f(1000.f, 1000.f)), m_world(sf::FloatRect({0.f, 0.f}, {2000.f, 2000.f})) {
    m_window.setFramerateLimit(60);
    if (!m_bgTexture.loadFromFile("assets/backgrounds/Background.png")) {
        [[maybe_unused]] const bool loadedFallbackBg =
            m_bgTexture.loadFromFile("assets/Background.png");
    }
    if (m_bgTexture.getSize().x > 0)
        m_bgTexture.setRepeated(true);
    m_bgTexture.setSmooth(false);
    if (!m_cloudTexture.loadFromFile("assets/backgrounds/Clouds.png")) {
        [[maybe_unused]] const bool loadedFallbackClouds =
            m_cloudTexture.loadFromFile("assets/Clouds.png");
    }
    if (m_cloudTexture.getSize().x > 0)
        m_cloudTexture.setRepeated(true);
    m_cloudTexture.setSmooth(false);

    m_hasUiFont = m_uiFont.openFromFile("C:/Windows/Fonts/arial.ttf");
    if (!m_hasUiFont)
        m_hasUiFont = m_uiFont.openFromFile("C:/Windows/Fonts/segoeui.ttf");
    m_vignette.setSize(sf::Vector2f(static_cast<float>(m_window.getSize().x),
                                    static_cast<float>(m_window.getSize().y)));
    m_vignette.setFillColor(sf::Color(255, 0, 0, 0));
    if (m_hasUiFont) {
        m_coinText = std::make_unique<sf::Text>(m_uiFont, "", 24);
        m_coinText->setFillColor(sf::Color::Yellow);
        m_coinText->setPosition({20.f, 20.f});
    }
}

void Game::startNewRun() {
    const sf::Vector2f spawn(0.f, 200.f);
    m_diver.reset();
    m_diver.setPosition(spawn);
    m_camera.update(spawn);
    m_world.resetAround(spawn);
    m_state = Playing;
    m_diverDamageCooldown = 0.f;
}

void Game::drawParallaxLayer(const sf::Texture &texture, const sf::View &view, float parallax,
                             sf::Color tint, bool mirror) {
    (void)mirror;
    if (texture.getSize().x == 0 || texture.getSize().y == 0)
        return;

    const sf::Vector2f center = view.getCenter();
    const sf::Vector2f size = view.getSize();
    const float layerCenterX = center.x * parallax;
    const float layerLeft = layerCenterX - size.x / 2.f;
    const float layerTop = center.y - size.y / 2.f;

    const float scaleY = size.y / static_cast<float>(texture.getSize().y);
    const float uniformScale = scaleY;
    const float tileWidth = static_cast<float>(texture.getSize().x) * uniformScale;
    const float startX = std::floor(layerLeft / tileWidth) * tileWidth;
    const int tileCount =
        static_cast<int>(std::ceil((size.x + (layerLeft - startX)) / tileWidth)) + 2;

    for (int i = 0; i < tileCount; ++i) {
        sf::Sprite tile(texture);
        tile.setColor(tint);
        tile.setOrigin({0.f, 0.f});
        tile.setPosition({startX + static_cast<float>(i) * tileWidth, layerTop});
        tile.setScale({uniformScale, uniformScale});
        m_window.draw(tile);
    }
}

void Game::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
#if defined(SFML_VERSION_MAJOR) && (SFML_VERSION_MAJOR >= 3)
    while (auto ev = m_window.pollEvent()) {
        if (ev->is<sf::Event::Closed>())
            m_window.close();
        if (const auto *resizeEvt = ev->getIf<sf::Event::Resized>()) {
            float windowRatio = static_cast<float>(resizeEvt->size.x) / resizeEvt->size.y;
            float worldRatio = WORLD_WIDTH / static_cast<float>(WORLD_HEIGHT);
            sf::FloatRect visibleArea;
            if (windowRatio > worldRatio) {
                float width = WORLD_HEIGHT * windowRatio;
                float x = (WORLD_WIDTH - width) / 2.f;
                visibleArea = sf::FloatRect({x, 0.f}, {width, static_cast<float>(WORLD_HEIGHT)});
            } else {
                float height = WORLD_WIDTH / windowRatio;
                float y = (WORLD_HEIGHT - height) / 2.f;
                visibleArea = sf::FloatRect({0.f, y}, {static_cast<float>(WORLD_WIDTH), height});
            }
            m_camera.setViewSize(visibleArea.size);
            m_camera.update(m_diver.getPosition());
        }
        if (const auto *mb = ev->getIf<sf::Event::MouseButtonPressed>()) {
            if (mb->button == sf::Mouse::Button::Left) {
                const sf::Vector2f click(static_cast<float>(mb->position.x),
                                         static_cast<float>(mb->position.y));
                const sf::FloatRect playRect = getCenterButtonRect({240.f, 72.f}, -20.f);
                const sf::FloatRect menuRect = getCenterButtonRect({240.f, 72.f}, 80.f);
                if (m_state == MainMenu && isPointInside(click, playRect))
                    startNewRun();
                else if (m_state == GameOver && isPointInside(click, playRect))
                    startNewRun();
                else if (m_state == Paused && isPointInside(click, playRect))
                    m_state = Playing;
                else if (m_state == Paused && isPointInside(click, menuRect))
                    m_state = MainMenu;
            }
        }
    }
#else
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            m_window.close();
        if (event.type == sf::Event::Resized) {
            float windowRatio = static_cast<float>(resizeEvt->size.x) / resizeEvt->size.y;
            float worldRatio = WORLD_WIDTH / static_cast<float>(WORLD_HEIGHT);
            sf::FloatRect visibleArea;
            if (windowRatio > worldRatio) {
                float width = WORLD_HEIGHT * windowRatio;
                float x = (WORLD_WIDTH - width) / 2.f;
                visibleArea = sf::FloatRect({x, 0.f}, {width, static_cast<float>(WORLD_HEIGHT)});
            } else {
                float height = WORLD_WIDTH / windowRatio;
                float y = (WORLD_HEIGHT - height) / 2.f;
                visibleArea = sf::FloatRect({0.f, y}, {static_cast<float>(WORLD_WIDTH), height});
            }
            m_camera.setViewSize(visibleArea.size);
            m_camera.update(m_diver.getPosition());
        }
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            const sf::Vector2f click(static_cast<float>(event.mouseButton.x),
                                     static_cast<float>(event.mouseButton.y));
            const sf::FloatRect playRect = getCenterButtonRect({240.f, 72.f}, -20.f);
            const sf::FloatRect menuRect = getCenterButtonRect({240.f, 72.f}, 80.f);
            if (m_state == MainMenu && isPointInside(click, playRect))
                startNewRun();
            else if (m_state == GameOver && isPointInside(click, playRect))
                startNewRun();
            else if (m_state == Paused && isPointInside(click, playRect))
                m_state = Playing;
            else if (m_state == Paused && isPointInside(click, menuRect))
                m_state = MainMenu;
        }
    }
#endif
}

void Game::update(float deltaTime) {
    // update view size from actual window size to handle resize across SFML versions
    auto ws = m_window.getSize();
    m_camera.setViewSize({static_cast<float>(ws.x), static_cast<float>(ws.y)});

    if (m_diverDamageCooldown > 0.f)
        m_diverDamageCooldown -= deltaTime;
    const float targetOverlay = (m_state == Playing) ? 0.f : 160.f;
    const float blendSpeed = 7.f;
    m_menuOverlayAlpha +=
        (targetOverlay - m_menuOverlayAlpha) * std::min(1.f, deltaTime * blendSpeed);

    if (m_state == Playing) {
        m_diver.handleInput();
        m_diver.update(deltaTime, m_world.getSeabedY(m_diver.getPosition().x));
        sf::Vector2f diverPos = m_diver.getPosition();
        for (const auto &rock : m_world.getRocks()) {
            if (rock.checkCollision(diverPos, PLAYER_RADIUS)) {
                sf::Vector2f diff = diverPos - rock.getPosition();
                float len = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                if (len > 0.001f) {
                    diff /= len;
                    m_diver.setPosition(rock.getPosition() +
                                        diff * (rock.getRadius() + PLAYER_RADIUS + 1.f));
                }
                m_diver.resetVelocity();
            }
        }
        m_camera.update(m_diver.getPosition());
        m_world.update(deltaTime, m_diver.getPosition(), m_camera.getView());

        for (const auto *fish : m_world.getFishes()) {
            if (fish->getType() == FishType::Predator &&
                fish->isColliding(m_diver.getPosition(), PLAYER_RADIUS) &&
                m_diverDamageCooldown <= 0.f) {
                m_diver.takeDamage(1);
                m_damageVignetteAlpha = 0.5f;
                m_diverDamageCooldown = 0.8f;
                if (m_diver.isDead()) {
                    m_state = GameOver;
                    break;
                }
            }
        }
        if (m_damageVignetteAlpha > 0.f) {
            m_damageVignetteAlpha = std::max(0.f, m_damageVignetteAlpha - deltaTime * 5.f);
        }

#if defined(SFML_VERSION_MAJOR) && (SFML_VERSION_MAJOR >= 3)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            m_diver.attack();
            m_world.handleAttack(m_diver.getPosition(), m_diver.getAttackRange(),
                                 m_diver.getAttackDamage());
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            m_state = Paused;
#else
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            m_diver.attack();
            m_world.handleAttack(m_diver.getPosition(), m_diver.getAttackRange(),
                                 m_diver.getAttackDamage());
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            m_state = Paused;
#endif
    } else if (m_state == Paused) {
        m_camera.update(m_diver.getPosition());
        // Enter / Return handling depends on SFML version
#if defined(SFML_VERSION_MAJOR) && (SFML_VERSION_MAJOR >= 3)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            m_state = Playing;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M))
            m_state = MainMenu;
#else
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            m_state = Playing;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
            m_state = MainMenu;
#endif
    } else if (m_state == MainMenu || m_state == GameOver) {
#if defined(SFML_VERSION_MAJOR) && (SFML_VERSION_MAJOR >= 3)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            startNewRun();
#else
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            startNewRun();
#endif
    }
}

void Game::render() {
    m_window.clear(sf::Color(30, 30, 30));

    m_window.setView(m_camera.getView());
    sf::View view = m_camera.getView();
    drawParallaxLayer(m_bgTexture, view, m_bgParallax, sf::Color::White, false);
    drawParallaxLayer(m_cloudTexture, view, m_cloudParallax, sf::Color(255, 255, 255, 200), false);

    if (m_state == Playing || m_state == Paused || m_state == GameOver) {
        m_world.draw(m_window, view);
        m_diver.draw(m_window);
    }

    m_window.setView(m_window.getDefaultView());
    if (m_menuOverlayAlpha > 1.f) {
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(static_cast<float>(m_window.getSize().x),
                                     static_cast<float>(m_window.getSize().y)));
        overlay.setFillColor(sf::Color(42, 46, 68, static_cast<std::uint8_t>(m_menuOverlayAlpha)));
        m_window.draw(overlay);

        sf::RectangleShape playButton;
        sf::RectangleShape secondButton;
        playButton.setSize({240.f, 72.f});
        secondButton.setSize({240.f, 72.f});
        playButton.setOrigin({120.f, 36.f});
        secondButton.setOrigin({120.f, 36.f});
        playButton.setPosition({static_cast<float>(m_window.getSize().x) * 0.5f,
                                static_cast<float>(m_window.getSize().y) * 0.5f - 20.f});
        secondButton.setPosition({static_cast<float>(m_window.getSize().x) * 0.5f,
                                  static_cast<float>(m_window.getSize().y) * 0.5f + 80.f});
        playButton.setFillColor(sf::Color(95, 117, 166, 230));
        secondButton.setFillColor(sf::Color(77, 93, 137, 220));

        if (m_state == MainMenu || m_state == GameOver)
            m_window.draw(playButton);
        else if (m_state == Paused) {
            m_window.draw(playButton);
            m_window.draw(secondButton);
        }

        if (m_hasUiFont && (m_state == MainMenu || m_state == GameOver || m_state == Paused)) {
            sf::Text playText(m_uiFont, "Play", 34);
            playText.setFillColor(sf::Color(235, 241, 255));
            playText.setPosition(
                {playButton.getPosition().x - 36.f, playButton.getPosition().y - 22.f});
            m_window.draw(playText);

            if (m_state == Paused) {
                sf::Text menuText(m_uiFont, "Menu", 30);
                menuText.setFillColor(sf::Color(223, 232, 255));
                menuText.setPosition(
                    {secondButton.getPosition().x - 42.f, secondButton.getPosition().y - 20.f});
                m_window.draw(menuText);
            }
        } else if (m_state == MainMenu || m_state == GameOver || m_state == Paused) {
            sf::CircleShape marker(10.f, 3);
            marker.setFillColor(sf::Color(235, 241, 255));
            marker.setOrigin({10.f, 10.f});
#if defined(SFML_VERSION_MAJOR) && (SFML_VERSION_MAJOR >= 3)
            marker.setRotation(sf::degrees(90.f));
#else
            marker.setRotation(90.f);
#endif
            marker.setPosition(playButton.getPosition());
            m_window.draw(marker);
        }
    }
    m_window.setView(m_window.getDefaultView());
    m_vignette.setSize(sf::Vector2f(static_cast<float>(m_window.getSize().x),
                                    static_cast<float>(m_window.getSize().y)));
    m_vignette.setFillColor(
        sf::Color(255, 0, 0, static_cast<uint8_t>(m_damageVignetteAlpha * 255)));
    m_window.draw(m_vignette);

    if (m_coinText) {
        m_coinText->setString("Points: " + std::to_string(m_world.getCoinCount()));
        m_window.draw(*m_coinText);
    }

    m_window.display();
}

} // namespace Game