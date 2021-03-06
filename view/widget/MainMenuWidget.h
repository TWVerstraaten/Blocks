//
// Created by pc on 01-02-21.
//

#ifndef BLOCKS_MAINMENUWIDGET_H
#define BLOCKS_MAINMENUWIDGET_H

#include "LevelSelectWidget.h"

#include <QTabWidget>

namespace view::widget {

    class Window;

    class MainMenuWidget : public QTabWidget {
      public:
        explicit MainMenuWidget(Window* parent);

        [[nodiscard]] LevelSelectWidget* levelSelectWidget() const;

      private:
        LevelSelectWidget* m_levelSelectWidget = nullptr;
    };

} // namespace view::widget

#endif // BLOCKS_MAINMENUWIDGET_H
