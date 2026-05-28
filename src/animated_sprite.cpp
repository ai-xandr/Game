#include "animated_sprite.hpp"
#include <SFML/Config.hpp>
#include <algorithm>

namespace Game {

AnimatedSprite::AnimatedSprite()
    : m_sprite(m_texture), m_frameSize(50.f, 50.f), m_frameTime(0.1f), m_elapsed(0.f),
      m_frameCount(1), m_currentFrame(0) {
}

bool AnimatedSprite::loadFromFile(const std::string &filename) {
    return m_texture.loadFromFile(filename);
}

void AnimatedSprite::setFrameCount(int count) {
    m_frameCount = count;
    m_currentFrame = 0;
    m_elapsed = 0.f;
}

void AnimatedSprite::setFrameSize(sf::Vector2f size) {
    m_frameSize = size;
}

void AnimatedSprite::setFrameTime(float time) {
    m_frameTime = time;
}

void AnimatedSprite::setPosition(sf::Vector2f pos) {
    m_sprite.setPosition(pos);
}

void AnimatedSprite::setScale(sf::Vector2f scale) {
    m_sprite.setScale(scale);
}

void AnimatedSprite::setRotation(float angle) {
#if defined(SFML_VERSION_MAJOR) && (SFML_VERSION_MAJOR >= 3)
    m_sprite.setRotation(sf::degrees(angle));
#else
    m_sprite.setRotation(angle);
#endif
}

void AnimatedSprite::setOrigin(sf::Vector2f origin) {
    m_sprite.setOrigin(origin);
}

void AnimatedSprite::update(float deltaTime) {
    const sf::Vector2u texSize = m_texture.getSize();
    if (texSize.x == 0 || texSize.y == 0)
        return;

    m_elapsed += deltaTime;
    if (m_elapsed >= m_frameTime && m_frameCount > 1) {
        m_elapsed -= m_frameTime;
        m_currentFrame = (m_currentFrame + 1) % m_frameCount;
    }

    m_sprite.setTexture(m_texture, true);
    int frameX = 0;
    int frameY = 0;
    int frameW = static_cast<int>(texSize.x);
    int frameH = static_cast<int>(texSize.y);
    if (m_frameCount > 1) {
        frameW =
            std::max(1, std::min(static_cast<int>(m_frameSize.x), static_cast<int>(texSize.x)));
        frameH =
            std::max(1, std::min(static_cast<int>(m_frameSize.y), static_cast<int>(texSize.y)));
        const int maxFramesByWidth = std::max(1, static_cast<int>(texSize.x) / frameW);
        frameX = (m_currentFrame % maxFramesByWidth) * frameW;
    }
#if defined(SFML_VERSION_MAJOR) && (SFML_VERSION_MAJOR >= 3)
    sf::IntRect rect(sf::Vector2i(frameX, frameY), sf::Vector2i(frameW, frameH));
#else
    sf::IntRect rect(frameX, frameY, frameW, frameH);
#endif
    m_sprite.setTextureRect(rect);
}

void AnimatedSprite::draw(sf::RenderWindow &window) const {
    window.draw(m_sprite);
}

sf::Vector2f AnimatedSprite::getPosition() const {
    return m_sprite.getPosition();
}

sf::Vector2f AnimatedSprite::getSize() const {
    return m_frameSize;
}

sf::Vector2u AnimatedSprite::getTextureSize() const {
    return m_texture.getSize();
}

} // namespace Game
