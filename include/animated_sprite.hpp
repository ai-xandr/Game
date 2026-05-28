#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace Game {

class AnimatedSprite {
  public:
    AnimatedSprite();

    bool loadFromFile(const std::string &filename);
    void setFrameCount(int count);
    void setFrameSize(sf::Vector2f size);
    void setFrameTime(float time);
    void setPosition(sf::Vector2f pos);
    void setScale(sf::Vector2f scale);
    void setRotation(float angle);
    void setOrigin(sf::Vector2f origin);

    void update(float deltaTime);
    void draw(sf::RenderWindow &window) const;

    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    sf::Vector2u getTextureSize() const;
    int getCurrentFrame() const {
        return m_currentFrame;
    }

  private:
    sf::Sprite m_sprite;
    sf::Texture m_texture;
    sf::Vector2f m_frameSize;
    float m_frameTime;
    float m_elapsed;
    int m_frameCount;
    int m_currentFrame;
};

} // namespace Game
