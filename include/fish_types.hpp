#ifndef FISH_TYPES_HPP
#define FISH_TYPES_HPP

#include "animated_sprite.hpp"
#include "common.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

namespace Game {

class GoldFish : public Fish {
  public:
    GoldFish(sf::Vector2f startPos);
    void update(float deltaTime, sf::Vector2f diverPos,
                const std::vector<Fish *> &allFishes) override;
    void draw(sf::RenderWindow &window) override;
    bool isColliding(sf::Vector2f point, float radius) const override;

  private:
    AnimatedSprite m_sprite;
};

class Tuna : public Fish {
  public:
    Tuna(sf::Vector2f startPos);
    void update(float deltaTime, sf::Vector2f diverPos,
                const std::vector<Fish *> &allFishes) override;
    void draw(sf::RenderWindow &window) override;
    bool isColliding(sf::Vector2f point, float radius) const override;

  private:
    AnimatedSprite m_sprite;
};

class Shark : public Fish {
  public:
    Shark(sf::Vector2f startPos);
    void update(float deltaTime, sf::Vector2f diverPos,
                const std::vector<Fish *> &allFishes) override;
    void draw(sf::RenderWindow &window) override;
    bool isColliding(sf::Vector2f point, float radius) const override;

  private:
    AnimatedSprite m_sprite;
};

class Crab : public Fish {
  public:
    Crab(sf::Vector2f startPos);
    void update(float deltaTime, sf::Vector2f diverPos,
                const std::vector<Fish *> &allFishes) override;
    void draw(sf::RenderWindow &window) override;
    bool isColliding(sf::Vector2f point, float radius) const override;

  private:
    AnimatedSprite m_sprite;
};

class Shell : public Fish {
  public:
    Shell(sf::Vector2f startPos);
    void update(float deltaTime, sf::Vector2f diverPos,
                const std::vector<Fish *> &allFishes) override;
    void draw(sf::RenderWindow &window) override;
    bool isColliding(sf::Vector2f point, float radius) const override;

  private:
    AnimatedSprite m_sprite;
};

class Algae : public Fish {
  public:
    Algae(sf::Vector2f startPos);
    void update(float deltaTime, sf::Vector2f diverPos,
                const std::vector<Fish *> &allFishes) override;
    void draw(sf::RenderWindow &window) override;
    bool isColliding(sf::Vector2f point, float radius) const override;

  private:
    AnimatedSprite m_sprite;
};

} // namespace Game

#endif
