//
// Created by pc on 01-02-21.
//

#ifndef BLOCKS_LEVELSELECTWIDGET_H
#define BLOCKS_LEVELSELECTWIDGET_H

#include <QScrollArea>

namespace view::widget {

    class LevelSelectWidget : public QScrollArea {
      public:
        explicit LevelSelectWidget(QWidget* parent);
    };

} // namespace view::widget

#endif // BLOCKS_LEVELSELECTWIDGET_H
