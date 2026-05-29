#ifndef AUDIO_MANAGER_HPP
#define AUDIO_MANAGER_HPP

#include <SFML/Audio.hpp>
#include <optional>

namespace Game {

class AudioManager {
  public:
    bool load();
    void update(float deltaTime);

    void playStartup();
    void startBackgroundMusic();
    void startMenuMusic();

    void playBeat();
    void playDeath();
    void playTakingDamage();
    void playRegeneration();
    void playWhoosh();
    void playPurchaseSequence();

  private:
    bool loadBuffer(sf::SoundBuffer &buffer, const char *path);

    sf::SoundBuffer m_startupBuffer;
    sf::SoundBuffer m_beatBuffer;
    sf::SoundBuffer m_deathBuffer;
    sf::SoundBuffer m_takingDamageBuffer;
    sf::SoundBuffer m_regenerationBuffer;
    sf::SoundBuffer m_whooshBuffer;
    sf::SoundBuffer m_buyItemBuffer;
    sf::SoundBuffer m_successBuffer;

    std::optional<sf::Sound> m_startupSound;
    std::optional<sf::Sound> m_beatSound;
    std::optional<sf::Sound> m_deathSound;
    std::optional<sf::Sound> m_takingDamageSound;
    std::optional<sf::Sound> m_regenerationSound;
    std::optional<sf::Sound> m_whooshSound;
    std::optional<sf::Sound> m_buyItemSound;
    std::optional<sf::Sound> m_successSound;

    sf::Music m_backgroundMusic;
    sf::Music m_menuMusic;
    bool m_backgroundReady = false;
    bool m_menuReady = false;

    bool m_purchaseSequenceActive = false;
    bool m_waitingForBuyItemEnd = false;
    float m_successDelayTimer = 0.f;
};

} // namespace Game

#endif
