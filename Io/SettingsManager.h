//
// Created by teunv on 2/2/2021.
//

#ifndef BLOCKS_SETTINGSMANAGER_H
#define BLOCKS_SETTINGSMANAGER_H

#include <QSettings>

namespace Io {

    class SettingsManager {

      public:
        static QSettings& settings();

      private:
        inline static QSettings m_settings{"dat/settings/settings.ini", QSettings::IniFormat};
    };

} // namespace Io

#endif // BLOCKS_SETTINGSMANAGER_H
