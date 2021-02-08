//
// Created by pc on 01-02-21.
//

#include "AudioSettings.h"

#include "AudioManager.h"

#include <cassert>

namespace audio {

    int AudioSettings::soundEffectsVolume() {
        return s_soundEffectVolume;
    }

    int AudioSettings::musicVolume() {
        return s_musicVolume;
    }

    void AudioSettings::setSoundEffectVolume(int soundEffectVolume) {
        assert(soundEffectVolume <= 100 && soundEffectVolume >= 0);
        s_soundEffectVolume = soundEffectVolume;
        AudioManager::setSoundEffectVolume(s_soundEffectVolume);
    }

    void AudioSettings::setMusicVolume(int musicVolume) {
        assert(musicVolume <= 100 && musicVolume >= 0);
        AudioManager::setMusicVolume(musicVolume);
        s_musicVolume = musicVolume;
    }

} // namespace audio