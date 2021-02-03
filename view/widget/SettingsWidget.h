//
// Created by teunv on 2/2/2021.
//

#ifndef BLOCKS_SETTINGSWIDGET_H
#define BLOCKS_SETTINGSWIDGET_H

#include "Window.h"
#include "Window_enums.h"

#include <QFormLayout>
#include <QWidget>

namespace view::widget {

    class SettingsWidget : public QWidget {

        Q_OBJECT

      public:
        explicit SettingsWidget(QWidget* parent, Window* window);

      signals:
        void windowModeChanged(WINDOW_MODE mode);

      private:
        void         addDivider(const QString& string);
        QFormLayout* m_formLayout;
    };

} // namespace view::widget

#endif // BLOCKS_SETTINGSWIDGET_H
