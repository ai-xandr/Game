#ifndef COIN_HPP
#define COIN_HPP

#include <SFML/Graphics.hpp>

namespace Game {

struct Coin {
    sf::Vector2f pos{0.f, 0.f};
    float radius = 8.f;
    int value = 1;
    float lifetime = 10.f;
    void update(float dt) {
        lifetime -= dt;
    }
    void draw(sf::RenderWindow &window) const {
        sf::CircleShape shape(radius);
        shape.setOrigin(sf::Vector2f(radius, radius));
        shape.setPosition(sf::Vector2f(pos.x, pos.y));
        shape.setFillColor(sf::Color::Yellow);
        window.draw(shape);
    }
};

} // namespace Game

#endif
