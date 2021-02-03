//
// Created by pc on 31-01-21.
//

#ifndef BLOCKS_AUDIOMANAGER_H
#define BLOCKS_AUDIOMANAGER_H

#include "AudioSettings.h"
#include "Audio_enums.h"
#include "SoundEffect.h"

#include <map>

namespace audio {

    class AudioManager {

      public:
        static void init();
        static void play(SOUNDS sound);

      private:
        friend class AudioSettings;

        static void setSoundEffectVolume(double soundEffectVolume);
        static void setMusicVolume(int musicVolume);
        static void addSoundEffect(SOUNDS sound);

        static bool                          s_isInitialized;
        static AudioSettings                 s_audioSettings;
        static std::map<SOUNDS, SoundEffect> s_soundEffects;
    };
} // namespace audio

#endif // BLOCKS_AUDIOMANAGER_H
