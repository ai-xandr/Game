#include "world.hpp"

namespace Game {

World::World(sf::FloatRect) {
}
World::~World() {
}
void World::update(float, sf::Vector2f) {
}
void World::draw(sf::RenderWindow &) const {
}
std::vector<Fish *> &World::getFishes() {
    return m_fishes;
}

} // namespace Game