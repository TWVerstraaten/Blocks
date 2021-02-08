//
// Created by teunv on 2/2/2021.
//

#include "SettingsManager.h"

#include "../audio/AudioManager.h"

static void readValue(const QString& name, double& value) {
    value = io::SettingsManager::settings().value(name).toDouble();
}

static void readValue(const QString& name, int& value) {
    value = io::SettingsManager::settings().value(name).toInt();
}

template <typename T>
static void readOrWrite(const QString& name, T& value) {
    if (io::SettingsManager::settings().contains(name)) {
        readValue(name, value);
    } else {
        io::SettingsManager::writeValueToFile(value, name);
    }
}

QSettings& io::SettingsManager::settings() {
    return m_settings;
}

void io::SettingsManager::initSettings() {
    assert(not s_isInitialized);
    s_isInitialized = true;
    readOrWrite("Sound fx volume", audio::AudioSettings::s_soundEffectVolume);
    readOrWrite("Music volume", audio::AudioSettings::s_musicVolume);

    //    addSettingWithSlider(
    //        layout, this, , audio::AudioSettings::soundEffectsVolume(), 0, 100, &audio::AudioSettings::setSoundEffectVolume);
    //    addSettingWithSlider(layout, this, "Music volume", audio::AudioSettings::musicVolume(), 0, 100, &audio::AudioSettings::setMusicVolume);
    //
    //    addDivider(layout, this, "window Settings");
    //    addSettingWithComboBox(layout,
    //                           this,
    //                           "Window mode",
    //                           {{"Maximized", D_THIS_FUNC(emit windowModeChanged(WINDOW_MODE::MAXIMIZED);)},
    //                            {"Fullscreen", D_THIS_FUNC(emit windowModeChanged(WINDOW_MODE::FULL_SCREEN);)},
    //                            {"Borderless", D_THIS_FUNC(emit windowModeChanged(WINDOW_MODE::BORDERLESS);)}});
    //
    //
    audio::AudioManager::init();
}
