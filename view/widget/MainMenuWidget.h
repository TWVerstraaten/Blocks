//
// Created by pc on 01-02-21.
//

#ifndef BLOCKS_MAINMENUWIDGET_H
#define BLOCKS_MAINMENUWIDGET_H

#include <QTabWidget>


namespace view::widget {

    class MainMenuWidget : public QTabWidget {
      public:
        explicit MainMenuWidget(QWidget* parent);
    };

} // namespace view::widget

#endif // BLOCKS_MAINMENUWIDGET_H
