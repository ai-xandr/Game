#include "camera.hpp"

namespace Game {

Camera::Camera(sf::FloatRect worldBounds, sf::Vector2f viewSize) : m_bounds(worldBounds) {
    m_view = sf::View(sf::FloatRect({0.f, 0.f}, viewSize));
}

void Camera::update(sf::Vector2f target) {
    m_view.setCenter(target);
}

sf::View Camera::getView() const {
    return m_view;
}

void Camera::setViewSize(sf::Vector2f size) {
    m_view.setSize(size);
}

} // namespace Game