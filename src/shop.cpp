#include "shop.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>

namespace Game {

Shop::Shop() {
    m_offers = {
        {"Attack CD -10%", 50, 0.10f, OfferKind::CooldownReduction},
        {"Attack CD -20%", 100, 0.20f, OfferKind::CooldownReduction},
        {"Attack CD -30%", 150, 0.30f, OfferKind::CooldownReduction},
        {"Damage +1", 40, 1.0f, OfferKind::DamageBoost},
        {"Damage +2", 80, 2.0f, OfferKind::DamageBoost},
        {"Damage +3", 120, 3.0f, OfferKind::DamageBoost},
    };
    m_purchased.assign(m_offers.size(), false);
}

bool Shop::loadAssets() {
    // Do not use shop artwork or button sprites; use unified Background.png and simple rectangle
    // buttons
    m_hasShopTexture = false;
    m_hasButtonTexture = false;
    return false;
}

sf::FloatRect Shop::getBackButtonRect(sf::Vector2f windowSize) const {
    const sf::Vector2f center(windowSize.x * 0.5f, windowSize.y * 0.5f + 220.f);
    const sf::Vector2f size(240.f, 72.f);
    return sf::FloatRect(center - size * 0.5f, size);
}

sf::FloatRect Shop::getOfferButtonRect(std::size_t index, sf::Vector2f windowSize) const {
    const int col = static_cast<int>(index % 2);
    const int row = static_cast<int>(index / 2);
    const float panelWidth = 520.f;
    const float panelHeight = 360.f;
    const sf::Vector2f panelCenter(windowSize.x * 0.5f, windowSize.y * 0.5f - 10.f);
    const sf::Vector2f panelTopLeft(panelCenter.x - panelWidth * 0.5f,
                                    panelCenter.y - panelHeight * 0.5f);
    const sf::Vector2f buttonSize(230.f, 52.f);
    const float gapX = 20.f;
    const float gapY = 14.f;
    const float startX = panelTopLeft.x + 20.f + static_cast<float>(col) * (buttonSize.x + gapX);
    const float startY = panelTopLeft.y + 70.f + static_cast<float>(row) * (buttonSize.y + gapY);
    return sf::FloatRect({startX, startY}, buttonSize);
}

bool Shop::isPointInside(sf::Vector2f point, const sf::FloatRect &rect) const {
    return rect.contains(point);
}

bool Shop::tryBuy(std::size_t index, int &coins, float &cooldownReduction, float &damageBonus) {
    if (index >= m_offers.size() || m_purchased[index])
        return false;

    const Offer &offer = m_offers[index];
    if (coins < offer.cost)
        return false;

    coins -= offer.cost;
    m_purchased[index] = true;

    if (offer.kind == OfferKind::CooldownReduction)
        cooldownReduction = std::max(cooldownReduction, offer.percent);
    else
        damageBonus += static_cast<int>(offer.percent);

    return true;
}

Shop::ClickResult Shop::handleClick(sf::Vector2f click, sf::Vector2f windowSize, int &coins,
                                    float &cooldownReduction, float &damageBonus) {

    if (isPointInside(click, getBackButtonRect(windowSize)))
        return ClickResult::Back;

    for (std::size_t i = 0; i < m_offers.size(); ++i) {
        if (!isPointInside(click, getOfferButtonRect(i, windowSize)))
            continue;
        if (tryBuy(i, coins, cooldownReduction, damageBonus))
            return ClickResult::Purchased;
        return ClickResult::None;
    }

    return ClickResult::None;
}

void Shop::draw(sf::RenderWindow &window, const sf::Font *font, int coins, float cooldownReduction,
                float damageMultiplier) const {
    const sf::Vector2f windowSize(static_cast<float>(window.getSize().x),
                                  static_cast<float>(window.getSize().y));

    sf::RectangleShape overlay;
    overlay.setSize(windowSize);
    overlay.setFillColor(sf::Color(42, 46, 68, 210));
    window.draw(overlay);

    const sf::Vector2f panelSize(520.f, 360.f);
    const sf::Vector2f panelCenter(windowSize.x * 0.5f, windowSize.y * 0.5f - 10.f);
    sf::RectangleShape panel;
    panel.setSize(panelSize);
    panel.setOrigin(panelSize * 0.5f);
    panel.setPosition(panelCenter);
    panel.setFillColor(sf::Color(58, 68, 98, 240));
    window.draw(panel);

    if (m_hasShopTexture) {
        sf::Sprite shopArt(m_shopTexture);
        const sf::Vector2u texSize = m_shopTexture.getSize();
        const float scale = std::min((panelSize.x - 40.f) / static_cast<float>(texSize.x),
                                     (panelSize.y - 120.f) / static_cast<float>(texSize.y));
        shopArt.setOrigin(sf::Vector2f(static_cast<float>(texSize.x) * 0.5f,
                                       static_cast<float>(texSize.y) * 0.5f));
        shopArt.setPosition(panelCenter + sf::Vector2f(0.f, -40.f));
        shopArt.setScale({scale, scale});
        shopArt.setColor(sf::Color(255, 255, 255, 180));
        window.draw(shopArt);
    }

    if (font) {
        sf::Text title(*font, "Shop", 36);
        title.setFillColor(sf::Color(235, 241, 255));
        title.setPosition({panelCenter.x - 52.f, panelCenter.y - panelSize.y * 0.5f + 16.f});
        window.draw(title);

        sf::Text coinsText(*font, "Points: " + std::to_string(coins), 22);
        coinsText.setFillColor(sf::Color::Yellow);
        coinsText.setPosition(
            {panelCenter.x - panelSize.x * 0.5f + 20.f, panelCenter.y - panelSize.y * 0.5f + 24.f});
        window.draw(coinsText);

        char stats[96];
        std::snprintf(stats, sizeof(stats), "CD bonus: -%.0f%%   Damage: x%.2f",
                      cooldownReduction * 100.f, damageMultiplier);
        sf::Text statsText(*font, stats, 18);
        statsText.setFillColor(sf::Color(200, 210, 235));
        statsText.setPosition({panelCenter.x - panelSize.x * 0.5f + 20.f,
                               panelCenter.y - panelSize.y * 0.5f + 280.f});
        window.draw(statsText);
    }

    for (std::size_t i = 0; i < m_offers.size(); ++i) {
        const sf::FloatRect rect = getOfferButtonRect(i, windowSize);
        const bool owned = m_purchased[i];
        const bool affordable = coins >= m_offers[i].cost;
        if (m_hasButtonTexture) {
            const sf::Vector2u texSize = m_buttonTexture.getSize();
            const int cols = 2;
            const int rows = 4;
            const int cellW = texSize.x / cols;
            const int cellH = texSize.y / rows;
            const sf::IntRect blueRect(sf::Vector2i(cellW * 1, 0), sf::Vector2i(cellW, cellH));

            sf::Sprite spr(m_buttonTexture);
            spr.setTextureRect(blueRect);
            spr.setOrigin({static_cast<float>(cellW) * 0.5f, static_cast<float>(cellH) * 0.5f});
            spr.setPosition(
                {rect.position.x + rect.size.x * 0.5f, rect.position.y + rect.size.y * 0.5f});
            spr.setScale(
                {rect.size.x / static_cast<float>(cellW), rect.size.y / static_cast<float>(cellH)});
            if (owned)
                spr.setColor(sf::Color(160, 180, 160, 230));
            else if (!affordable)
                spr.setColor(sf::Color(140, 150, 160, 200));
            window.draw(spr);
        } else {
            sf::RectangleShape button;
            button.setSize(rect.size);
            button.setPosition(rect.position);
            const sf::Color btnColor(42, 46, 68, 230);
            if (owned)
                button.setFillColor(sf::Color(60, 70, 90, 230));
            else
                button.setFillColor(btnColor);
            window.draw(button);
        }

        if (font) {
            std::string label = m_offers[i].label + " (" + std::to_string(m_offers[i].cost) + ")";
            if (owned)
                label += " [owned]";
            sf::Text itemText(*font, label, 16);
            itemText.setFillColor(sf::Color(235, 241, 255));
            itemText.setPosition({rect.position.x + 10.f, rect.position.y + 14.f});
            window.draw(itemText);
        }
    }

    const sf::FloatRect backRect = getBackButtonRect(windowSize);
    sf::RectangleShape backButton;
    backButton.setSize(backRect.size);
    backButton.setPosition(backRect.position);
    backButton.setFillColor(sf::Color(42, 46, 68, 230));
    window.draw(backButton);

    if (font) {
        sf::Text backText(*font, "Back", 30);
        backText.setFillColor(sf::Color(223, 232, 255));
        backText.setPosition({backRect.position.x + 78.f, backRect.position.y + 16.f});
        window.draw(backText);
    }
}

} // namespace Game
