//
// Created by pc on 31-01-21.
//

#include "AudioManager.h"

namespace audio {
    bool                            AudioManager::s_isInitialized = false;
    AudioSettings                   AudioManager::s_audioSettings;
    QMediaPlayer*                   AudioManager::s_musicPlayer = nullptr;
    std::map<SOUNDS, QSoundEffect*> AudioManager::s_soundEffects;

    void AudioManager::init(QObject* parent) {
        assert(not s_isInitialized);
        s_musicPlayer   = new QMediaPlayer(parent);
        s_isInitialized = true;

        addSoundEffect(SOUNDS::CLICK);
        s_musicPlayer->setMedia(QUrl("qrc:/assets/song2.mp3"));
        s_musicPlayer->setVolume(s_audioSettings.musicVolume());
        s_musicPlayer->play();
    }

    void AudioManager::play(SOUNDS sound) {
        if (s_soundEffects.find(sound) == s_soundEffects.end()) {
            addSoundEffect(sound);
        }
        s_soundEffects[sound]->play();
    }

    void AudioManager::addSoundEffect(SOUNDS sound) {
        assert(s_isInitialized);
        assert(s_soundEffects.find(sound) == s_soundEffects.end());

        QUrl url;
        switch (sound) {
            case SOUNDS::CLICK:
                url = QUrl("qrc:/assets/click.wav");
                break;
        }

        s_soundEffects[sound] = new QSoundEffect{s_musicPlayer->parent()};
        s_soundEffects[sound]->setSource(url);
        s_soundEffects[sound]->setVolume(s_audioSettings.soundEffectsVolume());
        s_soundEffects[sound]->setLoopCount(1);
    }

    void AudioManager::setSoundEffectVolume(float soundEffectVolume) {
        assert(s_isInitialized);
        for (auto& [_, soundEffect] : s_soundEffects) {
            soundEffect->setVolume(soundEffectVolume);
        }
    }

    void AudioManager::setMusicVolume(int musicVolume) {
        assert(s_isInitialized);
        s_musicPlayer->setVolume(musicVolume);
    }
} // namespace audio
