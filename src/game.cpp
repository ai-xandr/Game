#include "game.hpp"
#include "utility.hpp"
#include <SFML/Config.hpp>
#include <algorithm>
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

bool Game::loadUiFont() {
    // prefer bundled ChangaOne font
    if (m_uiFont.openFromFile("assets/fonts/ChangaOne-Regular.ttf"))
        return true;

    const char *fontPaths[] = {
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/segoeui.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/TTF/DejaVuSans.ttf",
    };
    for (const char *path : fontPaths) {
        if (m_uiFont.openFromFile(path))
            return true;
    }
    return false;
}

Game::Game()
    : m_window(sf::VideoMode({800, 600}), "David the Vodolaz", sf::Style::Default),
      m_camera(sf::FloatRect({0.f, 0.f}, {2000.f, 2000.f}), {800.f, 600.f}),
      m_diver(sf::Vector2f(1000.f, 1000.f)), m_world(sf::FloatRect({0.f, 0.f}, {2000.f, 2000.f})) {
    m_window.setFramerateLimit(60);

    if (!m_bgTexture.loadFromFile("assets/backgrounds/Background.jpg")) {
        [[maybe_unused]] const bool loadedFallbackBg =
            m_bgTexture.loadFromFile("assets/Background.jpg");
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

    m_hasStartupTexture = m_startupTexture.loadFromFile("assets/leha_vodnik/startup.png");
    if (m_hasStartupTexture)
        m_startupTexture.setSmooth(true);

    m_shop.loadAssets();
    m_audio.load();

    // Use a single static background image for menus and shop
    m_hasMenuBg = m_menuBgTexture.loadFromFile("assets/backgrounds/Background.png");
    if (m_hasMenuBg)
        m_menuBgTexture.setSmooth(true);

    // disable animated menu frames (we use static Background.png)
    m_menuFrames.clear();
    m_hasMenuFrames = false;

    // force simple rectangle buttons (do not use buttons.png)
    m_hasButtonTexture = false;

    m_hasUiFont = loadUiFont();
    m_vignette.setSize(sf::Vector2f(static_cast<float>(m_window.getSize().x),
                                    static_cast<float>(m_window.getSize().y)));
    m_vignette.setFillColor(sf::Color(255, 0, 0, 0));
    if (m_hasUiFont) {
        m_coinText = std::make_unique<sf::Text>(m_uiFont, "", 24);
        m_coinText->setFillColor(sf::Color::White);
        m_coinText->setPosition({20.f, 20.f});
    }
    if (m_hasUiFont) {
        m_hpText = std::make_unique<sf::Text>(m_uiFont, "", 24);
        m_hpText->setFillColor(sf::Color::White);
        m_hpText->setPosition({20.f, 50.f});
    }

    m_state = Startup;
    m_startupTimer = 0.f;
    m_startupFade = 1.f;
    m_audio.playStartup();
}

void Game::applyPersistentUpgrades() {
    m_diver.setCooldownReduction(m_cooldownReduction);
    m_diver.setDamageBonus(m_damageBonus);
}

void Game::startNewRun() {
    const sf::Vector2f spawn(0.f, 200.f);
    m_diver.reset();
    applyPersistentUpgrades();
    m_diver.setPosition(spawn);
    m_camera.update(spawn);
    m_world.resetAround(spawn);
    m_state = Playing;
    m_audio.startBackgroundMusic();
    m_diverDamageCooldown = 0.f;
}

void Game::drawParallaxLayer(const sf::Texture &texture, const sf::View &view, float parallax,
                             sf::Color tint, bool mirror) {
    (void)mirror;
    if (texture.getSize().x == 0 || texture.getSize().y == 0)
        return;

    const sf::Vector2f center = view.getCenter();
    const sf::Vector2f size = view.getSize();
    const float layerCenterX = center.x * (1.0f - parallax);
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
        const float deltaTime = clock.restart().asSeconds();
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
            const float windowRatio =
                static_cast<float>(resizeEvt->size.x) / static_cast<float>(resizeEvt->size.y);
            const float worldRatio = WORLD_WIDTH / static_cast<float>(WORLD_HEIGHT);
            sf::FloatRect visibleArea;
            if (windowRatio > worldRatio) {
                const float width = WORLD_HEIGHT * windowRatio;
                const float x = (WORLD_WIDTH - width) / 2.f;
                visibleArea = sf::FloatRect({x, 0.f}, {width, static_cast<float>(WORLD_HEIGHT)});
            } else {
                const float height = WORLD_WIDTH / windowRatio;
                const float y = (WORLD_HEIGHT - height) / 2.f;
                visibleArea = sf::FloatRect({0.f, y}, {static_cast<float>(WORLD_WIDTH), height});
            }
            m_camera.setViewSize(visibleArea.size);
            m_camera.update(m_diver.getPosition());
        }
        if (const auto *mb = ev->getIf<sf::Event::MouseButtonPressed>()) {
            if (mb->button != sf::Mouse::Button::Left)
                continue;

            const sf::Vector2f click(static_cast<float>(mb->position.x),
                                     static_cast<float>(mb->position.y));
            const sf::Vector2f windowSize(static_cast<float>(m_window.getSize().x),
                                          static_cast<float>(m_window.getSize().y));
            const sf::FloatRect playRect = getCenterButtonRect({240.f, 72.f}, -20.f);
            const sf::FloatRect shopRect = getCenterButtonRect({240.f, 72.f}, 80.f);
            const sf::FloatRect menuRect = getCenterButtonRect({240.f, 72.f}, 80.f);
            const sf::FloatRect exitRect = getCenterButtonRect({240.f, 72.f}, 180.f);
            // if we're in the playing state, treat left-click as attack
            if (m_state == Playing) {
                if (m_diver.attack()) {
                    const int hits = m_world.handleAttack(
                        m_diver.getPosition(), m_diver.getAttackRange(), m_diver.getAttackDamage());
                    if (hits > 0)
                        m_audio.playBeat();
                    else
                        m_audio.playWhoosh();
                }
            } else if (m_state == MainMenu && isPointInside(click, playRect))
                startNewRun();
            else if (m_state == MainMenu && isPointInside(click, shopRect)) {
                m_state = ShopScreen;
                m_audio.startMenuMusic();
            } else if (m_state == MainMenu && isPointInside(click, exitRect)) {
                m_window.close();
            } else if (m_state == GameOver && isPointInside(click, playRect))
                startNewRun();
            else if (m_state == Paused && isPointInside(click, playRect))
                m_state = Playing;
            else if (m_state == Paused && isPointInside(click, menuRect)) {
                m_state = MainMenu;
                m_audio.startMenuMusic();
            } else if (m_state == Paused && isPointInside(click, exitRect)) {
                m_window.close();
            } else if (m_state == ShopScreen) {
                int coins = m_world.getCoinCount();
                const auto result = m_shop.handleClick(click, windowSize, coins,
                                                       m_cooldownReduction, m_damageBonus);
                m_world.setCoinCount(coins);
                if (result == Shop::ClickResult::Purchased) {
                    applyPersistentUpgrades();
                    m_audio.playPurchaseSequence();
                } else if (result == Shop::ClickResult::Back) {
                    m_state = MainMenu;
                    m_audio.startMenuMusic();
                }
            }
        }
    }
#else
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            m_window.close();
        if (event.type == sf::Event::Resized) {
            const float windowRatio =
                static_cast<float>(event.size.width) / static_cast<float>(event.size.height);
            const float worldRatio = WORLD_WIDTH / static_cast<float>(WORLD_HEIGHT);
            sf::FloatRect visibleArea;
            if (windowRatio > worldRatio) {
                const float width = WORLD_HEIGHT * windowRatio;
                const float x = (WORLD_WIDTH - width) / 2.f;
                visibleArea = sf::FloatRect({x, 0.f}, {width, static_cast<float>(WORLD_HEIGHT)});
            } else {
                const float height = WORLD_WIDTH / windowRatio;
                const float y = (WORLD_HEIGHT - height) / 2.f;
                visibleArea = sf::FloatRect({0.f, y}, {static_cast<float>(WORLD_WIDTH), height});
            }
            m_camera.setViewSize(visibleArea.size);
            m_camera.update(m_diver.getPosition());
        }
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            const sf::Vector2f click(static_cast<float>(event.mouseButton.x),
                                     static_cast<float>(event.mouseButton.y));
            const sf::Vector2f windowSize(static_cast<float>(m_window.getSize().x),
                                          static_cast<float>(m_window.getSize().y));
            const sf::FloatRect playRect = getCenterButtonRect({240.f, 72.f}, -20.f);
            const sf::FloatRect shopRect = getCenterButtonRect({240.f, 72.f}, 80.f);
            const sf::FloatRect menuRect = getCenterButtonRect({240.f, 72.f}, 80.f);
            const sf::FloatRect exitRect = getCenterButtonRect({240.f, 72.f}, 180.f);
            // if we're in the playing state, treat left-click as attack
            if (m_state == Playing) {
                if (m_diver.attack()) {
                    const int hits = m_world.handleAttack(
                        m_diver.getPosition(), m_diver.getAttackRange(), m_diver.getAttackDamage());
                    if (hits > 0)
                        m_audio.playBeat();
                    else
                        m_audio.playWhoosh();
                }
            } else if (m_state == MainMenu && isPointInside(click, playRect))
                startNewRun();
            else if (m_state == MainMenu && isPointInside(click, shopRect)) {
                m_state = ShopScreen;
                m_audio.startMenuMusic();
            } else if (m_state == MainMenu && isPointInside(click, exitRect)) {
                m_window.close();
            } else if (m_state == GameOver && isPointInside(click, playRect))
                startNewRun();
            else if (m_state == Paused && isPointInside(click, playRect))
                m_state = Playing;
            else if (m_state == Paused && isPointInside(click, menuRect)) {
                m_state = MainMenu;
                m_audio.startMenuMusic();
            } else if (m_state == Paused && isPointInside(click, exitRect)) {
                m_window.close();
            } else if (m_state == ShopScreen) {
                int coins = m_world.getCoinCount();
                const auto result = m_shop.handleClick(click, windowSize, coins,
                                                       m_cooldownReduction, m_damageMultiplier);
                m_world.setCoinCount(coins);
                if (result == Shop::ClickResult::Purchased) {
                    applyPersistentUpgrades();
                    m_audio.playPurchaseSequence();
                } else if (result == Shop::ClickResult::Back) {
                    m_state = MainMenu;
                    m_audio.startMenuMusic();
                }
            }
        }
    }
#endif
}

void Game::update(float deltaTime) {
    m_audio.update(deltaTime);

    (void)deltaTime;

    const auto ws = m_window.getSize();
    m_camera.setViewSize({static_cast<float>(ws.x), static_cast<float>(ws.y)});

    if (m_state == Startup) {
        m_startupTimer += deltaTime;
        m_startupFade = std::max(0.f, 1.f - m_startupTimer / m_startupDuration);
        if (m_startupTimer >= m_startupDuration) {
            m_state = MainMenu;
            m_audio.startMenuMusic();
        }
        return;
    }

    if (m_diverDamageCooldown > 0.f)
        m_diverDamageCooldown -= deltaTime;

    const float targetOverlay = (m_state == Playing) ? 0.f : (m_state == ShopScreen ? 0.f : 160.f);
    const float blendSpeed = 7.f;
    m_menuOverlayAlpha +=
        (targetOverlay - m_menuOverlayAlpha) * std::min(1.f, deltaTime * blendSpeed);

    if (m_state == Playing) {
        m_diver.handleInput();
        m_diver.update(deltaTime, m_world.getSeabedY(m_diver.getPosition().x));
        const sf::Vector2f diverPos = m_diver.getPosition();
        for (const auto &rock : m_world.getRocks()) {
            if (rock.checkCollision(diverPos, PLAYER_RADIUS)) {
                sf::Vector2f diff = diverPos - rock.getPosition();
                const float len = std::sqrt(diff.x * diff.x + diff.y * diff.y);
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
                m_audio.playTakingDamage();
                m_damageVignetteAlpha = 0.5f;
                m_diverDamageCooldown = 0.8f;
                if (m_diver.isDead()) {
                    m_audio.playDeath();
                    m_state = GameOver;
                    break;
                }
            }
        }

        if (m_damageVignetteAlpha > 0.f)
            m_damageVignetteAlpha = std::max(0.f, m_damageVignetteAlpha - deltaTime * 5.f);

#if defined(SFML_VERSION_MAJOR) && (SFML_VERSION_MAJOR >= 3)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H)) {
            const int healCost = 5;
            if (m_world.getCoinCount() >= healCost && m_diver.getHp() < 10) {
                m_world.setCoinCount(m_world.getCoinCount() - healCost);
                m_diver.heal(5);
                m_audio.playRegeneration();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            m_state = Paused;
#else
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            m_state = Paused;
#endif
    } else if (m_state == Paused) {
        m_camera.update(m_diver.getPosition());
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
    } else if (m_state == ShopScreen) {
#if defined(SFML_VERSION_MAJOR) && (SFML_VERSION_MAJOR >= 3)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            m_state = MainMenu;
#else
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            m_state = MainMenu;
#endif
    }
}

void Game::renderStartup() {
    m_window.clear(sf::Color::Black);

    if (m_hasStartupTexture) {
        sf::Sprite splash(m_startupTexture);
        const sf::Vector2f windowSize(static_cast<float>(m_window.getSize().x),
                                      static_cast<float>(m_window.getSize().y));
        const sf::Vector2u texSize = m_startupTexture.getSize();
        const float scale = std::min(windowSize.x / static_cast<float>(texSize.x),
                                     windowSize.y / static_cast<float>(texSize.y));
        splash.setOrigin(sf::Vector2f(static_cast<float>(texSize.x) * 0.5f,
                                      static_cast<float>(texSize.y) * 0.5f));
        splash.setPosition(windowSize * 0.5f);
        splash.setScale({scale, scale});
        m_window.draw(splash);
    }

    sf::RectangleShape fade;
    fade.setSize(sf::Vector2f(static_cast<float>(m_window.getSize().x),
                              static_cast<float>(m_window.getSize().y)));
    fade.setFillColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>(m_startupFade * 255.f)));
    m_window.draw(fade);
}

void Game::renderMenuOverlay() {
    if (m_menuOverlayAlpha <= 1.f)
        return;

    const sf::Vector2f windowSize(static_cast<float>(m_window.getSize().x),
                                  static_cast<float>(m_window.getSize().y));

    // draw menu background for main menu / game over (static or animated frames)
    if ((m_state == MainMenu || m_state == GameOver) && (m_hasMenuFrames || m_hasMenuBg)) {
        if (m_hasMenuFrames) {
            const sf::Texture &tex = m_menuFrames[m_menuCurrentFrame];
            const sf::Vector2u texSize = tex.getSize();
            if (texSize.x > 0 && texSize.y > 0) {
                sf::Sprite bg(tex);
                bg.setPosition({0.f, 0.f});
                bg.setScale({windowSize.x / static_cast<float>(texSize.x),
                             windowSize.y / static_cast<float>(texSize.y)});
                m_window.draw(bg);
            }
        } else {
            const sf::Vector2u texSize = m_menuBgTexture.getSize();
            if (texSize.x > 0 && texSize.y > 0) {
                sf::Sprite bg(m_menuBgTexture);
                bg.setPosition({0.f, 0.f});
                bg.setScale({windowSize.x / static_cast<float>(texSize.x),
                             windowSize.y / static_cast<float>(texSize.y)});
                m_window.draw(bg);
            }
        }
    }

    sf::RectangleShape overlay;
    overlay.setSize(windowSize);
    overlay.setFillColor(sf::Color(42, 46, 68, static_cast<std::uint8_t>(m_menuOverlayAlpha)));
    m_window.draw(overlay);

    const sf::Vector2f buttonSize{240.f, 72.f};
    const sf::Vector2f center(windowSize.x * 0.5f, windowSize.y * 0.5f);
    const sf::Vector2f playCenter = center + sf::Vector2f(0.f, -20.f);
    const sf::Vector2f secondCenter = center + sf::Vector2f(0.f, 80.f);
    const sf::Vector2f exitCenter = center + sf::Vector2f(0.f, 180.f);

    if (m_hasButtonTexture) {
        const sf::Vector2u texSize = m_buttonTexture.getSize();
        const int cols = 2;
        const int rows = 4;
        const int cellW = texSize.x / cols;
        const int cellH = texSize.y / rows;
        const sf::IntRect blueRect(sf::Vector2i(cellW * 1, 0), sf::Vector2i(cellW, cellH));

        auto drawButtonSprite = [&](const sf::Vector2f &pos) {
            sf::Sprite spr(m_buttonTexture);
            spr.setTextureRect(blueRect);
            spr.setOrigin({static_cast<float>(cellW) * 0.5f, static_cast<float>(cellH) * 0.5f});
            spr.setPosition(pos);
            spr.setScale({buttonSize.x / static_cast<float>(cellW),
                          buttonSize.y / static_cast<float>(cellH)});
            m_window.draw(spr);
        };

        if (m_state == MainMenu || m_state == GameOver) {
            drawButtonSprite(playCenter);
            drawButtonSprite(secondCenter);
            drawButtonSprite(exitCenter);
        } else if (m_state == Paused) {
            drawButtonSprite(playCenter);
            drawButtonSprite(secondCenter);
            drawButtonSprite(exitCenter);
        }
    } else {
        sf::RectangleShape playButton;
        sf::RectangleShape secondButton;
        sf::RectangleShape exitButton;
        playButton.setSize(buttonSize);
        secondButton.setSize(buttonSize);
        exitButton.setSize(buttonSize);
        playButton.setOrigin(buttonSize * 0.5f);
        secondButton.setOrigin(buttonSize * 0.5f);
        exitButton.setOrigin(buttonSize * 0.5f);
        playButton.setPosition(playCenter);
        secondButton.setPosition(secondCenter);
        exitButton.setPosition(exitCenter);
        // unified button color to match menu background
        const sf::Color btnColor(42, 46, 68, 230);
        playButton.setFillColor(btnColor);
        secondButton.setFillColor(btnColor);
        exitButton.setFillColor(btnColor);
        if (m_state == MainMenu || m_state == GameOver || m_state == Paused) {
            m_window.draw(playButton);
            m_window.draw(secondButton);
            m_window.draw(exitButton);
        }
    }

    if (m_hasUiFont && (m_state == MainMenu || m_state == GameOver || m_state == Paused)) {
        sf::Text playText(m_uiFont, "Play", 34);
        playText.setFillColor(sf::Color::White);
        auto bounds = playText.getLocalBounds();
        playText.setOrigin(
            {bounds.position.x + bounds.size.x * 0.5f, bounds.position.y + bounds.size.y * 0.5f});
        playText.setPosition(playCenter);
        m_window.draw(playText);

        sf::Text secondText(m_uiFont, m_state == Paused ? "Menu" : "Shop", 30);
        secondText.setFillColor(sf::Color::White);
        bounds = secondText.getLocalBounds();
        secondText.setOrigin(
            {bounds.position.x + bounds.size.x * 0.5f, bounds.position.y + bounds.size.y * 0.5f});
        secondText.setPosition(secondCenter);
        m_window.draw(secondText);

        sf::Text exitText(m_uiFont, "Exit", 26);
        exitText.setFillColor(sf::Color::White);
        bounds = exitText.getLocalBounds();
        exitText.setOrigin(
            {bounds.position.x + bounds.size.x * 0.5f, bounds.position.y + bounds.size.y * 0.5f});
        exitText.setPosition(exitCenter);
        m_window.draw(exitText);
    } else if (m_state == MainMenu || m_state == GameOver || m_state == Paused) {
        sf::CircleShape marker(10.f, 3);
        marker.setFillColor(sf::Color(235, 241, 255));
        marker.setOrigin({10.f, 10.f});
#if defined(SFML_VERSION_MAJOR) && (SFML_VERSION_MAJOR >= 3)
        marker.setRotation(sf::degrees(90.f));
#else
        marker.setRotation(90.f);
#endif
        marker.setPosition(playCenter);
        m_window.draw(marker);
    }
}

void Game::renderShop() {
    m_window.clear(sf::Color::Black);
    m_window.setView(m_window.getDefaultView());
    if (m_hasMenuFrames || m_hasMenuBg) {
        const sf::Vector2f windowSize(static_cast<float>(m_window.getSize().x),
                                      static_cast<float>(m_window.getSize().y));
        if (m_hasMenuFrames) {
            const sf::Texture &tex = m_menuFrames[m_menuCurrentFrame];
            const sf::Vector2u texSize = tex.getSize();
            if (texSize.x > 0 && texSize.y > 0) {
                sf::Sprite bg(tex);
                bg.setPosition({0.f, 0.f});
                bg.setScale({windowSize.x / static_cast<float>(texSize.x),
                             windowSize.y / static_cast<float>(texSize.y)});
                m_window.draw(bg);
            }
        } else {
            const sf::Vector2u texSize = m_menuBgTexture.getSize();
            if (texSize.x > 0 && texSize.y > 0) {
                sf::Sprite bg(m_menuBgTexture);
                bg.setPosition({0.f, 0.f});
                bg.setScale({windowSize.x / static_cast<float>(texSize.x),
                             windowSize.y / static_cast<float>(texSize.y)});
                m_window.draw(bg);
            }
        }
    }
    m_shop.draw(m_window, m_hasUiFont ? &m_uiFont : nullptr, m_world.getCoinCount(),
                m_cooldownReduction, m_damageBonus);
}

void Game::render() {
    if (m_state == Startup) {
        renderStartup();
        m_window.display();
        return;
    }

    if (m_state == ShopScreen) {
        renderShop();
        m_window.display();
        return;
    }

    m_window.clear(sf::Color(30, 30, 30));

    m_window.setView(m_camera.getView());
    const sf::View view = m_camera.getView();
    const sf::Vector2f viewCenter = view.getCenter();
    const sf::Vector2f viewSize = view.getSize();
    if (m_bgTexture.getSize().x > 0 && m_bgTexture.getSize().y > 0) {
        sf::Sprite bgSprite(m_bgTexture);
        float scaleY = viewSize.y / m_bgTexture.getSize().y;
        float scaleX = viewSize.x / m_bgTexture.getSize().x;
        float scale = std::max(scaleX, scaleY);
        bgSprite.setScale({scale, scale});
        sf::Vector2f spriteSize(m_bgTexture.getSize().x * scale, m_bgTexture.getSize().y * scale);
        bgSprite.setPosition(viewCenter - spriteSize / 2.f);
        m_window.draw(bgSprite);
    }
    drawParallaxLayer(m_cloudTexture, view, m_cloudParallax, sf::Color(255, 255, 255, 200), false);

    if (m_state == Playing || m_state == Paused || m_state == GameOver) {
        m_world.draw(m_window, view);
        m_diver.draw(m_window);
    }

    m_window.setView(m_window.getDefaultView());
    renderMenuOverlay();

    m_vignette.setSize(sf::Vector2f(static_cast<float>(m_window.getSize().x),
                                    static_cast<float>(m_window.getSize().y)));
    m_vignette.setFillColor(
        sf::Color(255, 0, 0, static_cast<std::uint8_t>(m_damageVignetteAlpha * 255)));
    m_window.draw(m_vignette);

    if (m_coinText && m_state != MainMenu) {
        m_coinText->setString("Points: " + std::to_string(m_world.getCoinCount()));
        m_window.draw(*m_coinText);
    }

    if (m_hpText && (m_state == Playing || m_state == Paused)) {
        m_hpText->setString("HP: " + std::to_string(m_diver.getHp()));
        m_window.draw(*m_hpText);
    }

    m_window.display();
}

} // namespace Game
