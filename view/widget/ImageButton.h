//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_IMAGEBUTTON_H
#define BLOCKS_IMAGEBUTTON_H

#include "../BlockType.h"

#include <QCheckBox>
#include <QPushButton>

namespace view::widget {

    class ImageButton : public QPushButton {

        Q_OBJECT

      public:
        ImageButton(QWidget* parent, const view::BlockType& type, const QSize& size);

        void setSelected();
        void setUnSelected();


      private:
        QPixmap m_pixmap;
    };

} // namespace view::widget

#endif // BLOCKS_IMAGEBUTTON_H
