#include "audio_manager.hpp"

namespace Game {

bool AudioManager::loadBuffer(sf::SoundBuffer &buffer, const char *path) {
    return buffer.loadFromFile(path);
}

bool AudioManager::load() {
    const bool startupOk = loadBuffer(m_startupBuffer, "assets/audio/sfx/startup.wav");
    const bool beatOk = loadBuffer(m_beatBuffer, "assets/audio/sfx/beat.wav");
    const bool deathOk = loadBuffer(m_deathBuffer, "assets/audio/sfx/death.wav");
    const bool takingDamageOk =
        loadBuffer(m_takingDamageBuffer, "assets/audio/sfx/taking_damage.wav");
    const bool regenOk = loadBuffer(m_regenerationBuffer, "assets/audio/sfx/regeneration.wav");
    const bool whooshOk = loadBuffer(m_whooshBuffer, "assets/audio/sfx/whoosh.wav");
    const bool buyOk = loadBuffer(m_buyItemBuffer, "assets/audio/sfx/buy_item.wav");
    const bool successOk = loadBuffer(m_successBuffer, "assets/audio/sfx/success.wav");

    if (startupOk)
        m_startupSound.emplace(m_startupBuffer);
    if (beatOk)
        m_beatSound.emplace(m_beatBuffer);
    if (deathOk)
        m_deathSound.emplace(m_deathBuffer);
    if (takingDamageOk)
        m_takingDamageSound.emplace(m_takingDamageBuffer);
    else if (beatOk)
        m_takingDamageSound.emplace(m_beatBuffer);
    if (regenOk)
        m_regenerationSound.emplace(m_regenerationBuffer);
    if (whooshOk)
        m_whooshSound.emplace(m_whooshBuffer);
    if (buyOk)
        m_buyItemSound.emplace(m_buyItemBuffer);
    if (successOk)
        m_successSound.emplace(m_successBuffer);

    // adjust SFX volumes to balance mix
    if (m_startupSound)
        m_startupSound->setVolume(80.f);
    if (m_beatSound)
        m_beatSound->setVolume(75.f);
    if (m_deathSound)
        m_deathSound->setVolume(90.f);
    if (m_takingDamageSound)
        m_takingDamageSound->setVolume(80.f);
    if (m_regenerationSound)
        m_regenerationSound->setVolume(70.f);
    if (m_whooshSound)
        m_whooshSound->setVolume(60.f);
    if (m_buyItemSound)
        m_buyItemSound->setVolume(65.f);
    if (m_successSound)
        m_successSound->setVolume(70.f);

    m_backgroundReady = m_backgroundMusic.openFromFile("assets/audio/music/background.ogg");
    if (m_backgroundReady) {
        m_backgroundMusic.setLooping(true);
        m_backgroundMusic.setVolume(45.f);
    }

    m_menuReady = m_menuMusic.openFromFile("assets/audio/music/main_menu.ogg");
    if (m_menuReady) {
        m_menuMusic.setLooping(true);
        m_menuMusic.setVolume(45.f);
    }

    return startupOk || beatOk || deathOk || buyOk || successOk || m_backgroundReady || m_menuReady;
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
    if (m_menuReady && m_menuMusic.getStatus() == sf::SoundSource::Status::Playing)
        m_menuMusic.stop();
    if (m_backgroundMusic.getStatus() != sf::SoundSource::Status::Playing)
        m_backgroundMusic.play();
}

void AudioManager::startMenuMusic() {
    if (!m_menuReady)
        return;
    if (m_backgroundReady && m_backgroundMusic.getStatus() == sf::SoundSource::Status::Playing)
        m_backgroundMusic.stop();
    if (m_menuMusic.getStatus() != sf::SoundSource::Status::Playing)
        m_menuMusic.play();
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

void AudioManager::playTakingDamage() {
    if (!m_takingDamageSound)
        return;
    m_takingDamageSound->stop();
    m_takingDamageSound->play();
}

void AudioManager::playRegeneration() {
    if (!m_regenerationSound)
        return;
    m_regenerationSound->stop();
    m_regenerationSound->play();
}

void AudioManager::playWhoosh() {
    if (!m_whooshSound)
        return;
    m_whooshSound->stop();
    m_whooshSound->play();
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
