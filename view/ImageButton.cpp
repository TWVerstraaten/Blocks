//
// Created by teunv on 1/18/2021.
//

#include "ImageButton.h"

#include "toPixmap.h"

view::ImageButton::ImageButton(QWidget* parent, const view::BlockType& type, const QSize& size)
    : QPushButton(parent), m_pixmap(toPixmapVariant(type, size)) {
    setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    setIconSize(size);
    setIcon(m_pixmap);
    setFixedSize(size);
}
