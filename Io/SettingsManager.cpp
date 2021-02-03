//
// Created by teunv on 2/2/2021.
//

#include "SettingsManager.h"

QSettings& Io::SettingsManager::settings() {
    return m_settings;
}
