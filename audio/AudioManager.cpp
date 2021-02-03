//
// Created by pc on 31-01-21.
//

#include "AudioManager.h"

#include <cassert>

namespace audio {
    bool                          AudioManager::s_isInitialized = false;
    std::map<SOUNDS, SoundEffect> AudioManager::s_soundEffects;
    std::unique_ptr<Music>        AudioManager::s_music;

    void AudioManager::init() {
        assert(not s_isInitialized);
        s_isInitialized = true;

        addSoundEffect(SOUNDS::CLICK);
        s_music = std::make_unique<Music>("dat/audio/music/song2.wav");
        s_music->setVolume(AudioSettings::musicVolume());
        s_music->play();
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
        s_soundEffects[sound].setVolume(AudioSettings::soundEffectsVolume());
    }

    void AudioManager::setSoundEffectVolume(double soundEffectVolume) {
        assert(s_isInitialized);
        for (auto& [_, soundEffect] : s_soundEffects) {
            soundEffect.setVolume(soundEffectVolume);
        }
    }

    void AudioManager::setMusicVolume(int musicVolume) {
        assert(s_isInitialized);
        s_music->setVolume(musicVolume);
    }
} // namespace audio
