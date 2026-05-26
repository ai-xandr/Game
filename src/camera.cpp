#include "camera.hpp"

namespace Game {

Camera::Camera(sf::FloatRect, sf::Vector2f viewSize) {
    m_view = sf::View(sf::FloatRect({0.f, 0.f}, viewSize));
}
void Camera::update(sf::Vector2f) {
}
sf::View Camera::getView() const {
    return m_view;
}

} // namespace Game