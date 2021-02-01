//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_IMAGEBUTTON_H
#define BLOCKS_IMAGEBUTTON_H

#include "../BlockType.h"

#include <QPushButton>

namespace view::widget {

    class ImageButton : public QPushButton {

      public:
        ImageButton(QWidget* parent, const view::BlockType& type, const QSize& size);

        void setSelected();
        void setUnselected();

      private:
        QPixmap m_pixmap;
    };

} // namespace view

#endif // BLOCKS_IMAGEBUTTON_H
