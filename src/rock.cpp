#include "rock.hpp"
#include <cmath>
#include <random>

namespace Game {

Rock::Rock(sf::Vector2f position, float radius) : m_position(position), m_radius(radius) {
    generateRandomShape();
    m_shape.setPosition(m_position);
    m_shape.setFillColor(sf::Color(80, 80, 80, 220));

    if (m_texture.loadFromFile("assets/rock.png")) {
        m_sprite = std::make_unique<sf::Sprite>(m_texture);
        m_sprite->setOrigin({static_cast<float>(m_texture.getSize().x) / 2.f,
                             static_cast<float>(m_texture.getSize().y) / 2.f});
        m_sprite->setPosition(m_position);
        m_hasTexture = true;
    }
}

void Rock::generateRandomShape() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> vertexCountDist(6, 10);
    std::uniform_real_distribution<float> angleDist(0.f, 2.f * 3.14159f);
    std::uniform_real_distribution<float> radiusDist(m_radius * 0.6f, m_radius * 1.2f);

    int numVertices = vertexCountDist(gen);
    m_shape.setPointCount(numVertices);

    std::vector<float> angles(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        angles[i] = angleDist(gen);
    }
    std::sort(angles.begin(), angles.end());

    for (int i = 0; i < numVertices; ++i) {
        float angle = angles[i];
        float r = radiusDist(gen);
        float x = r * std::cos(angle);
        float y = r * std::sin(angle);
        m_shape.setPoint(i, sf::Vector2f(x, y));
    }

    sf::Vector2f centroid(0.f, 0.f);
    for (int i = 0; i < numVertices; ++i) {
        centroid += m_shape.getPoint(i);
    }
    centroid /= static_cast<float>(numVertices);
    m_shape.setOrigin(centroid);
}

void Rock::draw(sf::RenderWindow &window) const {
    if (m_hasTexture && m_sprite)
        window.draw(*m_sprite);
    else
        window.draw(m_shape);
}

bool Rock::checkCollision(const sf::Vector2f &point, float radius) const {
    float dx = m_position.x - point.x;
    float dy = m_position.y - point.y;
    float r = m_radius + radius;
    return (dx * dx + dy * dy) <= r * r;
}

sf::Vector2f Rock::getPosition() const {
    return m_position;
}
float Rock::getRadius() const {
    return m_radius;
}

} // namespace Game