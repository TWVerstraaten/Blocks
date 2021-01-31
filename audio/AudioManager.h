//
// Created by pc on 31-01-21.
//

#ifndef BLOCKS_AUDIOMANAGER_H
#define BLOCKS_AUDIOMANAGER_H

#include "Audio_enums.h"

#include <QMediaPlayer>
#include <QSoundEffect>

namespace audio {

    class AudioManager {

      public:
        static void init(QObject* parent);

        static void play(SOUNDS sound);

      private:
        static void addSoundEffect(SOUNDS sound);

        static bool                            s_isInitialized;
        static QMediaPlayer*                   s_mediaPlayer;
        static std::map<SOUNDS, QSoundEffect*> s_soundEffects;
    };
} // namespace audio

#endif // BLOCKS_AUDIOMANAGER_H
