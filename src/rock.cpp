#include "rock.hpp"

#include <algorithm>
#include <cmath>
#include <random>

namespace Game {

Rock::Rock(sf::Vector2f position, float radius)
    : m_position(position), m_radius(radius), m_vertices(sf::PrimitiveType::TriangleFan) {
    if (m_texture.loadFromFile("assets/leha_vodnik/rock.jpeg")) {
        m_texture.setRepeated(true);
        m_hasTexture = true;
    }

    generateRandomShape();
}

void Rock::generateRandomShape() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> vertexCountDist(7, 12);
    std::uniform_real_distribution<float> angleDist(0.f, 2.f * 3.14159265f);
    std::uniform_real_distribution<float> radiusDist(m_radius * 0.75f, m_radius * 1.15f);
    int numVertices = vertexCountDist(gen);
    std::vector<float> angles(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        angles[i] = angleDist(gen);
    }

    std::sort(angles.begin(), angles.end());

    for (int i = 0; i < numVertices; ++i) {
        float angle = angles[i];
        float r = radiusDist(gen);
        float x = std::cos(angle) * r;
        float y = std::sin(angle) * r;

        m_points.push_back({x, y});
    }
    sf::Vector2f center(0.f, 0.f);
    for (const auto &p : m_points) {
        center += p;
    }

    center /= static_cast<float>(m_points.size());

    float textureScale = m_radius * 2.5f;

    sf::Vertex centerVertex;

    centerVertex.position = m_position;
    centerVertex.texCoords = {0.f, 0.f};
    centerVertex.color = sf::Color::White;

    m_vertices.append(centerVertex);

    for (size_t i = 0; i <= m_points.size(); ++i) {
        sf::Vector2f p = m_points[i % m_points.size()];
        sf::Vertex vertex;
        vertex.position = m_position + p;
        vertex.texCoords = {p.x / textureScale * static_cast<float>(m_texture.getSize().x),
                            p.y / textureScale * static_cast<float>(m_texture.getSize().y)};
        vertex.color = sf::Color::White;
        m_vertices.append(vertex);
    }
}

void Rock::draw(sf::RenderWindow &window) const {
    sf::RenderStates states;
    if (m_hasTexture) {
        states.texture = &m_texture;
    }
    window.draw(m_vertices, states);
}

bool Rock::checkCollision(const sf::Vector2f &point, float radius) const {
    float dx = m_position.x - point.x;
    float dy = m_position.y - point.y;
    float r = m_radius + radius;

    return (dx * dx + dy * dy) <= (r * r);
}

sf::Vector2f Rock::getPosition() const {
    return m_position;
}

float Rock::getRadius() const {
    return m_radius;
}

} // namespace Game