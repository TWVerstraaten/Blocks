//
// Created by teunv on 2/3/2021.
//

#include "SoundEffect.h"

#include <cassert>

audio::SoundEffect::SoundEffect(const std::string& path) {
    assert(m_buffer.loadFromFile(path));
}

void audio::SoundEffect::play() {
    if (m_sound.getStatus() != sf::SoundSource::Playing) {
        m_sound.setBuffer(m_buffer);
        m_sound.setVolume(m_volume);
        m_sound.play();
    }
}

void audio::SoundEffect::setVolume(int volume) {
    m_volume = volume;
}
