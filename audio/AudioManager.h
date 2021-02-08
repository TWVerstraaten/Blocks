//
// Created by pc on 31-01-21.
//

#ifndef BLOCKS_AUDIOMANAGER_H
#define BLOCKS_AUDIOMANAGER_H

#include "AudioSettings.h"
#include "Audio_enums.h"
#include "Music.h"
#include "SoundEffect.h"

#include <map>
#include <memory>

namespace io {
    class SettingsManager;
}

namespace audio {

    class AudioManager {

      public:
        static void play(SOUNDS sound);

      private:
        friend class AudioSettings;
        friend class io::SettingsManager;

        static void init();

        static void setSoundEffectVolume(double soundEffectVolume);
        static void setMusicVolume(int musicVolume);
        static void addSoundEffect(SOUNDS sound);

        static bool                          s_isInitialized;
        static std::map<SOUNDS, SoundEffect> s_soundEffects;
        static std::unique_ptr<Music>        s_music;
    };
} // namespace audio

#endif // BLOCKS_AUDIOMANAGER_H
