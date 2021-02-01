//
// Created by pc on 31-01-21.
//

#ifndef BLOCKS_AUDIOMANAGER_H
#define BLOCKS_AUDIOMANAGER_H

#include "AudioSettings.h"
#include "Audio_enums.h"

#include <QMediaPlayer>
#include <QSoundEffect>

namespace audio {

    class AudioManager {

      public:
        static void init(QObject* parent);

        static void play(SOUNDS sound);

      private:
        friend class AudioSettings;

        static void setSoundEffectVolume(float soundEffectVolume);
        static void setMusicVolume(int musicVolume);

        static void addSoundEffect(SOUNDS sound);

        static bool                            s_isInitialized;
        static AudioSettings                   s_audioSettings;
        static QMediaPlayer*                   s_musicPlayer;
        static std::map<SOUNDS, QSoundEffect*> s_soundEffects;
    };
} // namespace audio

#endif // BLOCKS_AUDIOMANAGER_H
