#include "world.hpp"
#include "fish_types.hpp"
#include "utility.hpp"
#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

namespace Game {

World::World(sf::FloatRect bounds) : m_bounds(bounds) {
    std::random_device rd;
    m_rng.seed(rd());
    m_spawnTimer = 0.f;
    m_spawnInterval = 0.3f;
    // spawn a few initial fishes near the center so player sees activity immediately
    sf::Vector2f center(m_bounds.position.x + m_bounds.size.x / 2.f,
                        m_bounds.position.y + m_bounds.size.y / 2.f);
    for (int i = 0; i < 60; ++i) {
        float rx = (m_dist01(m_rng) - 0.5f) * 400.f;
        float ry = (m_dist01(m_rng) - 0.5f) * 200.f;
        sf::Vector2f pos = center + sf::Vector2f(rx, ry);
        float r = m_dist01(m_rng);
        if (r < 0.5f)
            m_fishes.push_back(new GoldFish(pos));
        else if (r < 0.85f)
            m_fishes.push_back(new Tuna(pos));
        else
            m_fishes.push_back(new GoldFish(pos));
    }
    spawnInitialRocks();
}

World::~World() {
    for (auto f : m_fishes)
        delete f;
}

void World::spawnRandomFish(const sf::FloatRect &noSpawnRect) {
    const float spawnMargin = 150.f;
    sf::FloatRect spawnRect(noSpawnRect.position - sf::Vector2f(spawnMargin, spawnMargin),
                            noSpawnRect.size + sf::Vector2f(spawnMargin * 2.f, spawnMargin * 2.f));

    float r = m_dist01(m_rng);
    Fish *fish = nullptr;
    sf::Vector2f pos;
    bool valid = false;

    for (int attempt = 0; attempt < 50; ++attempt) {
        float x = spawnRect.position.x + m_dist01(m_rng) * spawnRect.size.x;
        float y = spawnRect.position.y + m_dist01(m_rng) * spawnRect.size.y;
        sf::Vector2f point(x, y);
        if (!noSpawnRect.contains(point)) {
            pos = point;
            valid = true;
            break;
        }
    }

    if (!valid)
        return;

    if (r < 0.4f)
        fish = new GoldFish(pos);
    else if (r < 0.8f)
        fish = new Tuna(pos);
    else if (r < 0.9f)
        fish = new Crab(sf::Vector2f(pos.x, getSeabedY(pos.x) - 20.f));
    else if (r < 0.98f)
        fish = new Shell(sf::Vector2f(pos.x, getSeabedY(pos.x) - 10.f));
    else
        fish = new Shark(pos);

    if (fish != nullptr) {
        float dirX = m_dist01(m_rng) < 0.5f ? -1.f : 1.f;
        fish->setHorizontalDirection(dirX);
        m_fishes.push_back(fish);
    }
}
void World::spawnInitialRocks() {
    const int numRocks = 15;
    for (int i = 0; i < numRocks; ++i) {
        float x = m_dist01(m_rng) * WORLD_WIDTH;
        float y = m_dist01(m_rng) * WORLD_HEIGHT;
        float radius = 40.f + m_dist01(m_rng) * 100.f;
        m_rocks.emplace_back(sf::Vector2f(x, y), radius);
    }
}

void World::update(float deltaTime, sf::Vector2f diverPos, const sf::View &cameraView) {
    const sf::Vector2f viewSize = cameraView.getSize();
    const sf::Vector2f viewCenter = cameraView.getCenter();
    sf::FloatRect visibleRect(viewCenter - viewSize / 2.f, viewSize);

    int fishesInActiveArea = 0;
    const float activeMargin = 220.f;
    sf::FloatRect activeRect(visibleRect.position - sf::Vector2f(activeMargin, activeMargin),
                             visibleRect.size +
                                 sf::Vector2f(activeMargin * 2.f, activeMargin * 2.f));
    for (const auto *f : m_fishes) {
        if (activeRect.contains(f->getPosition()))
            ++fishesInActiveArea;
    }

    m_spawnTimer -= deltaTime;
    if (m_spawnTimer <= 0.f || fishesInActiveArea < m_targetFishMin) {
        spawnRandomFish(visibleRect);
        if (fishesInActiveArea < m_targetFishMin / 2)
            spawnRandomFish(visibleRect);
        m_spawnTimer = m_spawnInterval * (0.45f + m_dist01(m_rng) * 0.35f);
    }

    const float viewMargin = 300.f;
    const float spawnMargin = 500.f;

    sf::FloatRect safeRect = visibleRect;
    safeRect.position -= sf::Vector2f(viewMargin, viewMargin);
    safeRect.size += sf::Vector2f(viewMargin * 2.f, viewMargin * 2.f);

    sf::FloatRect spawnRect = visibleRect;
    spawnRect.position -= sf::Vector2f(spawnMargin, spawnMargin);
    spawnRect.size += sf::Vector2f(spawnMargin * 2.f, spawnMargin * 2.f);

    const float removalMargin = 800.f;
    sf::FloatRect removalRect = visibleRect;
    removalRect.position -= sf::Vector2f(removalMargin, removalMargin);
    removalRect.size += sf::Vector2f(removalMargin * 2.f, removalMargin * 2.f);

    m_rocks.erase(std::remove_if(m_rocks.begin(), m_rocks.end(),
                                 [&removalRect](const Rock &r) {
                                     return !removalRect.contains(r.getPosition());
                                 }),
                  m_rocks.end());

    int activeRocks = 0;
    for (const auto &rock : m_rocks) {
        if (spawnRect.contains(rock.getPosition()))
            ++activeRocks;
    }

    const int minRocks = 20;
    int toSpawn = minRocks - activeRocks;
    for (int i = 0; i < toSpawn; ++i) {
        sf::Vector2f point;
        bool valid = false;
        for (int attempts = 0; attempts < 50; ++attempts) {
            float x = spawnRect.position.x + m_dist01(m_rng) * spawnRect.size.x;
            float y = spawnRect.position.y + m_dist01(m_rng) * spawnRect.size.y;
            point = {x, y};
            if (!safeRect.contains(point)) {
                valid = true;
                break;
            }
        }
        if (!valid)
            continue;
        float radius = 40.f + m_dist01(m_rng) * 100.f;
        m_rocks.emplace_back(point, radius);
    }
    for (auto f : m_fishes)
        f->update(deltaTime, diverPos, m_fishes);

    const float cullMargin = 400.f;
    sf::FloatRect cullRect(visibleRect.position - sf::Vector2f(cullMargin, cullMargin),
                           visibleRect.size + sf::Vector2f(cullMargin * 2.f, cullMargin * 2.f));

    for (auto it = m_fishes.begin(); it != m_fishes.end();) {
        Fish *f = *it;
        const sf::Vector2f p = f->getPosition();
        const bool outOfScreenSpace = !cullRect.contains(p);
        if (f->isDead()) {
            m_collectedCoins += f->getPoints();
            delete f;
            it = m_fishes.erase(it);
        } else if (outOfScreenSpace) {
            delete f;
            it = m_fishes.erase(it);
        } else {
            ++it;
        }
    }
}

void World::draw(sf::RenderWindow &window, const sf::View &cameraView) const {
    const sf::Vector2f viewSize = cameraView.getSize();
    const sf::Vector2f viewCenter = cameraView.getCenter();
    const float left = viewCenter.x - viewSize.x / 2.f - 64.f;
    const float right = viewCenter.x + viewSize.x / 2.f + 64.f;

    // Draw a strip-based seabed gradient that follows camera and gives infinite-bottom illusion.
    const float step = 64.f;
    const int segmentCount = std::max(2, static_cast<int>((right - left) / step) + 2);
    sf::VertexArray seabed(sf::PrimitiveType::TriangleStrip,
                           static_cast<std::size_t>(segmentCount) * 2);

    for (int i = 0; i < segmentCount; ++i) {
        const float x = left + static_cast<float>(i) * step;
        const float y = getSeabedY(x);
        const std::size_t idx = static_cast<std::size_t>(i) * 2;

        seabed[idx].position = sf::Vector2f(x, y);
        seabed[idx + 1].position = sf::Vector2f(x, y + 700.f);
        seabed[idx].color = sf::Color(69, 88, 95);
        seabed[idx + 1].color = sf::Color(42, 56, 64);
    }
    window.draw(seabed);

    for (const auto &rock : m_rocks) {
        rock.draw(window);
    }
    for (auto f : m_fishes)
        f->draw(window);
}

std::vector<Fish *> &World::getFishes() {
    return m_fishes;
}

int World::handleAttack(sf::Vector2f pos, float range, int damage) {
    int hits = 0;
    for (auto f : m_fishes) {
        if (f->isColliding(pos, range)) {
            f->takeDamage(damage);
            ++hits;
        }
    }
    return hits;
}

int World::getCoinCount() const {
    return m_collectedCoins;
}

void World::setCoinCount(int amount) {
    m_collectedCoins = std::max(0, amount);
}

float World::getSeabedY(float x) const {
    const float base = m_bounds.position.y + m_bounds.size.y - 240.f;
    const float waveA = std::sin(x * 0.0023f) * 50.f;
    const float waveB = std::sin(x * 0.0067f + 1.7f) * 20.f;
    return base + waveA + waveB;
}

void World::resetAround(sf::Vector2f center) {
    for (auto *f : m_fishes)
        delete f;
    m_fishes.clear();
    m_rocks.clear();
    m_spawnTimer = 0.f;

    const sf::FloatRect seedRect(center - sf::Vector2f(400.f, 300.f), sf::Vector2f(800.f, 600.f));
    const int initialCount = m_targetFishMin;
    for (int i = 0; i < initialCount; ++i)
        spawnRandomFish(seedRect);
}

} // namespace Game