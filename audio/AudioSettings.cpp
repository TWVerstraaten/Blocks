//
// Created by pc on 01-02-21.
//

#include "AudioSettings.h"

#include "AudioManager.h"

namespace audio {
    AudioSettings::AudioSettings() = default;

    float AudioSettings::soundEffectsVolume() const {
        return m_soundEffectVolume;
    }

    int AudioSettings::musicVolume() const {
        return m_musicVolume;
    }

    void AudioSettings::setSoundEffectVolume(float soundEffectVolume) {
        m_soundEffectVolume = soundEffectVolume;
        AudioManager::setSoundEffectVolume(m_soundEffectVolume);
    }

    void AudioSettings::setMusicVolume(int musicVolume) {
        AudioManager::setMusicVolume(musicVolume);
        m_musicVolume = musicVolume;
    }
} // namespace audio