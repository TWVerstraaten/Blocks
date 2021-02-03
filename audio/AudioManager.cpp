//
// Created by pc on 31-01-21.
//

#include "AudioManager.h"

#include <cassert>

namespace audio {
    bool                          AudioManager::s_isInitialized = false;
    AudioSettings                 AudioManager::s_audioSettings;
    std::map<SOUNDS, SoundEffect> AudioManager::s_soundEffects;

    void AudioManager::init() {
        assert(not s_isInitialized);
        s_isInitialized = true;

        addSoundEffect(SOUNDS::CLICK);

        //        s_musicPlayer->setVolume(s_audioSettings.musicVolume());
    }

    void AudioManager::play(SOUNDS sound) {
        if (s_soundEffects.find(sound) == s_soundEffects.end()) {
            addSoundEffect(sound);
        }
        s_soundEffects[sound].play();
    }

    void AudioManager::addSoundEffect(SOUNDS sound) {
        assert(s_isInitialized);
        assert(s_soundEffects.find(sound) == s_soundEffects.end());

        std::string url;
        switch (sound) {
            case SOUNDS::CLICK:
                url = "dat/audio/fx/click.wav";
                break;
        }
        s_soundEffects.insert({sound, SoundEffect(url)});
        s_soundEffects[sound].setVolume(s_audioSettings.soundEffectsVolume());
    }

    void AudioManager::setSoundEffectVolume(double soundEffectVolume) {
        assert(s_isInitialized);
        for (auto& [_, soundEffect] : s_soundEffects) {
            soundEffect.setVolume(soundEffectVolume);
        }
    }

    void AudioManager::setMusicVolume(int musicVolume) {
        assert(s_isInitialized);
//        s_musicPlayer->setVolume(musicVolume);
    }
} // namespace audio
