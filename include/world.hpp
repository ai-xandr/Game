#ifndef WORLD_HPP
#define WORLD_HPP

#include "common.hpp"
#include "rock.hpp"
#include <random>
#include <vector>

namespace Game {

class World {
  public:
    World(sf::FloatRect bounds);
    ~World();
    void update(float deltaTime, sf::Vector2f diverPos, const sf::View &cameraView);
    void draw(sf::RenderWindow &window, const sf::View &cameraView) const;
    std::vector<Fish *> &getFishes();
    int handleAttack(sf::Vector2f pos, float range, int damage);
    int getCoinCount() const;
    void setCoinCount(int amount);
    float getSeabedY(float x) const;
    void resetAround(sf::Vector2f center);
    const std::vector<Rock> &getRocks() const {
        return m_rocks;
    }

  private:
    std::vector<Fish *> m_fishes;
    sf::FloatRect m_bounds;
    float m_spawnTimer = 0.f;
    float m_spawnInterval = 0.3f;
    int m_targetFishMin = 24;
    std::mt19937 m_rng;
    std::uniform_real_distribution<float> m_dist01{0.f, 1.f};
    void spawnRandomFish(const sf::FloatRect &visibleRect);
    int m_collectedCoins = 0;
    std::vector<Rock> m_rocks;
    void spawnInitialRocks();
};
} // namespace Game
#endif