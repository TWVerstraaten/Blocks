//
// Created by pc on 01-02-21.
//

#ifndef BLOCKS_AUDIOSETTINGS_H
#define BLOCKS_AUDIOSETTINGS_H

namespace io {
    class SettingsManager;
}

namespace audio {

    class AudioSettings {

      public:
        [[nodiscard]] static int soundEffectsVolume();
        [[nodiscard]] static int musicVolume();

        static void setSoundEffectVolume(int soundEffectVolume);
        static void setMusicVolume(int musicVolume);

      private:
        friend class io::SettingsManager;

        inline static int s_soundEffectVolume = 40;
        inline static int s_musicVolume       = 40;
    };

} // namespace audio

#endif // BLOCKS_AUDIOSETTINGS_H
