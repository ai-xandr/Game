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
    void playBeat();
    void playDeath();
    void playPurchaseSequence();

  private:
    bool loadBuffer(sf::SoundBuffer &buffer, const char *path);

    sf::SoundBuffer m_startupBuffer;
    sf::SoundBuffer m_beatBuffer;
    sf::SoundBuffer m_deathBuffer;
    sf::SoundBuffer m_buyItemBuffer;
    sf::SoundBuffer m_successBuffer;

    std::optional<sf::Sound> m_startupSound;
    std::optional<sf::Sound> m_beatSound;
    std::optional<sf::Sound> m_deathSound;
    std::optional<sf::Sound> m_buyItemSound;
    std::optional<sf::Sound> m_successSound;

    sf::Music m_backgroundMusic;
    bool m_backgroundReady = false;

    bool m_purchaseSequenceActive = false;
    bool m_waitingForBuyItemEnd = false;
    float m_successDelayTimer = 0.f;
};

} // namespace Game

#endif
