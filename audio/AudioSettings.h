//
// Created by pc on 01-02-21.
//

#ifndef BLOCKS_AUDIOSETTINGS_H
#define BLOCKS_AUDIOSETTINGS_H

namespace audio {

    class AudioSettings {

      public:
        AudioSettings();

        [[nodiscard]] float soundEffectsVolume() const;
        [[nodiscard]] int   musicVolume() const;

        void setSoundEffectVolume(float soundEffectVolume);
        void setMusicVolume(int musicVolume);

      private:
        float m_soundEffectVolume = 0.3f;
        int   m_musicVolume       = 0;
    };

} // namespace audio

#endif // BLOCKS_AUDIOSETTINGS_H
