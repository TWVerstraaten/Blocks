//
// Created by pc on 01-02-21.
//

#ifndef BLOCKS_AUDIOSETTINGS_H
#define BLOCKS_AUDIOSETTINGS_H

namespace audio {

    class AudioSettings {

      public:
        [[nodiscard]] static double soundEffectsVolume();
        [[nodiscard]] static int    musicVolume();

        static void setSoundEffectVolume(double soundEffectVolume);
        static void setMusicVolume(int musicVolume);

      private:
        inline static double s_soundEffectVolume = 0.3f;
        inline static int    s_musicVolume       = 40;
    };

} // namespace audio

#endif // BLOCKS_AUDIOSETTINGS_H
