//
// Created by pc on 03-02-21.
//

#ifndef BLOCKS_MUSIC_H
#define BLOCKS_MUSIC_H

#include <SFML/Audio/Music.hpp>

namespace audio {

    class Music {

      public:
        Music() = default;
        explicit Music(const std::string& path);

        void play();
        void stop();
        void pause();

        void setVolume(int volume);

      private:
        sf::Music m_music;
    };

} // namespace audio

#endif // BLOCKS_MUSIC_H
