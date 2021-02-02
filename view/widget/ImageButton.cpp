//
// Created by teunv on 1/18/2021.
//

#include "ImageButton.h"

#include "../toPixmap.h"

#include <QStyle>
#include <QVariant>

namespace view::widget {

    ImageButton::ImageButton(QWidget* parent, const BlockType& type, const QSize& size) : QPushButton(parent), m_pixmap(toPixmapVariant(type, size)) {
        setIconSize(size - QSize{6, 6});
        setIcon(m_pixmap);
        setFixedSize(size);
    }

    void ImageButton::setSelected() {
        setProperty("selected", true);
        style()->polish(this);
    }

    void ImageButton::setUnSelected() {
        setProperty("selected", false);
        style()->polish(this);
    }

} // namespace view::widget
