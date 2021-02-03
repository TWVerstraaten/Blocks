//
// Created by pc on 01-02-21.
//

#include "AudioSettings.h"

#include "AudioManager.h"

namespace audio {

    double AudioSettings::soundEffectsVolume() {
        return s_soundEffectVolume;
    }

    int AudioSettings::musicVolume() {
        return s_musicVolume;
    }

    void AudioSettings::setSoundEffectVolume(double soundEffectVolume) {
        s_soundEffectVolume = soundEffectVolume;
        AudioManager::setSoundEffectVolume(s_soundEffectVolume);
    }

    void AudioSettings::setMusicVolume(int musicVolume) {
        AudioManager::setMusicVolume(musicVolume);
        s_musicVolume = musicVolume;
    }
} // namespace audio