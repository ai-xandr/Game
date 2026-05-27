#include "camera.hpp"
#include <algorithm>

namespace Game {

Camera::Camera(sf::FloatRect worldBounds, sf::Vector2f viewSize) : m_bounds(worldBounds) {
    m_view = sf::View(sf::FloatRect({0.f, 0.f}, viewSize));
}

void Camera::update(sf::Vector2f target) {
    sf::Vector2f center = target;

    float halfW = m_view.getSize().x / 2.f;
    float halfH = m_view.getSize().y / 2.f;

    center.x = std::clamp(center.x, m_bounds.position.x + halfW,
                          m_bounds.position.x + m_bounds.size.x - halfW);
    center.y = std::clamp(center.y, m_bounds.position.y + halfH,
                          m_bounds.position.y + m_bounds.size.y - halfH);

    m_view.setCenter(center);
}

sf::View Camera::getView() const {
    return m_view;
}

} // namespace Game