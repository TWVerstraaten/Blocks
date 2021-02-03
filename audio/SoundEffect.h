//
// Created by teunv on 2/3/2021.
//

#ifndef BLOCKS_SOUNDEFFECT_H
#define BLOCKS_SOUNDEFFECT_H

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <string>

namespace audio {

    class SoundEffect {

      public:
        SoundEffect() = default;
        explicit SoundEffect(const std::string& path);

        void play();

        void setVolume(int volume);

      private:
        int             m_volume = 40;
        sf::SoundBuffer m_buffer;
        sf::Sound       m_sound;
    };

} // namespace audio

#endif // BLOCKS_SOUNDEFFECT_H
