#include "audio_manager.hpp"

namespace Game {

bool AudioManager::loadBuffer(sf::SoundBuffer &buffer, const char *path) {
    return buffer.loadFromFile(path);
}

bool AudioManager::load() {
    const bool startupOk = loadBuffer(m_startupBuffer, "assets/audio/sfx/startup.wav");
    const bool beatOk = loadBuffer(m_beatBuffer, "assets/audio/sfx/beat.wav");
    const bool deathOk = loadBuffer(m_deathBuffer, "assets/audio/sfx/death.wav");
    const bool buyOk = loadBuffer(m_buyItemBuffer, "assets/audio/sfx/buy_item.wav");
    const bool successOk = loadBuffer(m_successBuffer, "assets/audio/sfx/success.wav");

    if (startupOk)
        m_startupSound.emplace(m_startupBuffer);
    if (beatOk)
        m_beatSound.emplace(m_beatBuffer);
    if (deathOk)
        m_deathSound.emplace(m_deathBuffer);
    if (buyOk)
        m_buyItemSound.emplace(m_buyItemBuffer);
    if (successOk)
        m_successSound.emplace(m_successBuffer);

    m_backgroundReady = m_backgroundMusic.openFromFile("assets/audio/music/background.ogg");
    if (m_backgroundReady) {
        m_backgroundMusic.setLooping(true);
        m_backgroundMusic.setVolume(45.f);
    }

    return startupOk || beatOk || deathOk || buyOk || successOk || m_backgroundReady;
}

void AudioManager::update(float deltaTime) {
    if (!m_purchaseSequenceActive)
        return;

    if (m_waitingForBuyItemEnd) {
        const bool buyPlaying =
            m_buyItemSound && m_buyItemSound->getStatus() == sf::SoundSource::Status::Playing;
        if (!buyPlaying) {
            m_waitingForBuyItemEnd = false;
            m_successDelayTimer = 0.5f;
        }
        return;
    }

    m_successDelayTimer -= deltaTime;
    if (m_successDelayTimer <= 0.f) {
        if (m_successSound) {
            m_successSound->stop();
            m_successSound->play();
        }
        m_purchaseSequenceActive = false;
    }
}

void AudioManager::playStartup() {
    if (!m_startupSound)
        return;
    m_startupSound->stop();
    m_startupSound->play();
}

void AudioManager::startBackgroundMusic() {
    if (!m_backgroundReady)
        return;
    if (m_backgroundMusic.getStatus() != sf::SoundSource::Status::Playing)
        m_backgroundMusic.play();
}

void AudioManager::playBeat() {
    if (!m_beatSound)
        return;
    m_beatSound->stop();
    m_beatSound->play();
}

void AudioManager::playDeath() {
    if (!m_deathSound)
        return;
    m_deathSound->stop();
    m_deathSound->play();
}

void AudioManager::playPurchaseSequence() {
    m_purchaseSequenceActive = true;
    m_waitingForBuyItemEnd = true;
    m_successDelayTimer = 0.f;

    if (!m_buyItemSound) {
        m_waitingForBuyItemEnd = false;
        m_successDelayTimer = 0.5f;
        return;
    }

    m_buyItemSound->stop();
    m_buyItemSound->play();
}

} // namespace Game
