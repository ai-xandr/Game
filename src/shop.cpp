#include "shop.hpp"

namespace Game {

Shop::Shop() {
}

void Shop::open() {
    m_open = true;
}

void Shop::close() {
    m_open = false;
}

bool Shop::isOpen() const {
    return m_open;
}

void Shop::draw(sf::RenderWindow &window) const {
    if (!m_open)
        return;
    sf::RectangleShape bg;
    bg.setSize(sf::Vector2f(400.f, 300.f));
    bg.setFillColor(sf::Color(0, 0, 0, 160));
    auto vsz = window.getView().getSize();
    bg.setPosition(sf::Vector2f(vsz.x / 2.f - 200.f, vsz.y / 2.f - 150.f));
    window.draw(bg);
    // Placeholder: font/text omitted for compatibility with different SFML versions
    sf::RectangleShape stripe;
    stripe.setSize(sf::Vector2f(360.f, 40.f));
    stripe.setFillColor(sf::Color(255, 255, 255, 60));
    stripe.setPosition(bg.getPosition() + sf::Vector2f(20.f, 20.f));
    window.draw(stripe);
}

void Shop::addItem(const Upgrade &u) {
    m_items.push_back(u);
}

} // namespace Game
