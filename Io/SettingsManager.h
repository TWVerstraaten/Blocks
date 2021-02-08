//
// Created by teunv on 2/2/2021.
//

#ifndef BLOCKS_SETTINGSMANAGER_H
#define BLOCKS_SETTINGSMANAGER_H

#include <QSettings>

namespace io {

    class SettingsManager {

      public:
        static QSettings& settings();

        static void initSettings();

        template <typename T>
        static void writeValueToFile(const T& value, const QString& nameInIni) {
            io::SettingsManager::settings().setValue(nameInIni, value);
        }

      private:
        inline static bool      s_isInitialized = false;
        inline static QSettings m_settings{"dat/settings/settings.ini", QSettings::IniFormat};
    };

} // namespace io

#endif // BLOCKS_SETTINGSMANAGER_H
