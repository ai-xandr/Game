#ifndef SHOP_HPP
#define SHOP_HPP

#include "common.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

namespace Game {

class Shop {
  public:
    Shop();
    void open();
    void close();
    bool isOpen() const;
    void draw(sf::RenderWindow &window) const;
    void addItem(const Upgrade &u);

  private:
    std::vector<Upgrade> m_items;
    bool m_open = false;
};

} // namespace Game

#endif
