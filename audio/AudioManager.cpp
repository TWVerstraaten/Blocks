//
// Created by pc on 31-01-21.
//

#include "AudioManager.h"

#include <QMediaPlayer>
#include <QSoundEffect>
#include <memory>

namespace audio {
    bool                            AudioManager::s_isInitialized = false;
    QMediaPlayer*                   AudioManager::s_mediaPlayer   = nullptr;
    std::map<SOUNDS, QSoundEffect*> AudioManager::s_soundEffects;

    void AudioManager::init(QObject* parent) {
        assert(not s_isInitialized);
        s_mediaPlayer   = new QMediaPlayer(parent);
        s_isInitialized = true;
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

        s_soundEffects[sound] = new QSoundEffect{s_mediaPlayer->parent()};
        s_soundEffects[sound]->setSource(url);
        s_soundEffects[sound]->setVolume(0.5f);
        s_soundEffects[sound]->setLoopCount(1);
    }
} // namespace audio
