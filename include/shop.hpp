#ifndef SHOP_HPP
#define SHOP_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

namespace Game {

class Shop {
  public:
    enum class OfferKind { CooldownReduction, DamageBoost };

    struct Offer {
        std::string label;
        int cost = 0;
        float percent = 0.f;
        OfferKind kind = OfferKind::CooldownReduction;
    };

    Shop();
    bool loadAssets();
    void draw(sf::RenderWindow &window, const sf::Font *font, int coins, float cooldownReduction,
              float damageMultiplier) const;
    enum class ClickResult { None, Purchased, Back };

    ClickResult handleClick(sf::Vector2f click, sf::Vector2f windowSize, int &coins,
                            float &cooldownReduction, float &damageMultiplier);

  private:
    sf::FloatRect getBackButtonRect(sf::Vector2f windowSize) const;
    sf::FloatRect getOfferButtonRect(std::size_t index, sf::Vector2f windowSize) const;
    bool isPointInside(sf::Vector2f point, const sf::FloatRect &rect) const;
    bool tryBuy(std::size_t index, int &coins, float &cooldownReduction, float &damageMultiplier);

    std::vector<Offer> m_offers;
    std::vector<bool> m_purchased;
    sf::Texture m_shopTexture;
    bool m_hasShopTexture = false;
};

} // namespace Game

#endif
