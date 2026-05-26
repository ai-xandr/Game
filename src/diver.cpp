#include "diver.hpp"

namespace Game {

Diver::Diver(sf::Vector2f startPos) : m_position(startPos) {
}
void Diver::handleInput() {
}
void Diver::update(float) {
}
void Diver::attack() {
}
void Diver::draw(sf::RenderWindow &) const {
}
sf::Vector2f Diver::getPosition() const {
    return m_position;
}

} // namespace Game