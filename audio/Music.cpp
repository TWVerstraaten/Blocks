//
// Created by pc on 03-02-21.
//

#include "Music.h"

#include <cassert>

audio::Music::Music(const std::string& path) {
    assert(m_music.openFromFile(path));
}

void audio::Music::play() {
    if (m_music.getStatus() != sf::SoundSource::Playing) {
        m_music.play();
    }
}

void audio::Music::setVolume(int volume) {
    m_music.setVolume(volume);
}

void audio::Music::stop() {
    m_music.stop();
}

void audio::Music::pause() {
    m_music.pause();
}
