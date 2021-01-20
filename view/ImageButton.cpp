//
// Created by teunv on 1/18/2021.
//

#include "ImageButton.h"

#include "toPixmap.h"

#include <QDebug>

namespace view {

    ImageButton::ImageButton(QWidget* parent, const BlockType& type, const QSize& size) : QPushButton(parent), m_pixmap(toPixmapVariant(type, size)) {
        setIconSize(size - QSize{6, 6});
        setIcon(m_pixmap);
        setFixedSize(size);
        update();
    }

    void ImageButton::setSelected() {
        setStyleSheet(QString("QPushButton {\n"
                              "    background-color: %1;\n"
                              "border: 3px solid red;"
                              "}\n")
                          .arg(color::WIDGET_LIGHT.name(QColor::HexRgb)));
    }

    void ImageButton::setUnselected() {
        setStyleSheet(QString("QPushButton {\n"
                              "    background-color: %1;\n"
                              "}\n"
                              "QPushButton:hover {\n"
                              "    background-color: %2;\n"
                              "}\n")                          .arg(color::WIDGET_DARK.name(QColor::HexRgb))
                          .arg(color::WIDGET_LIGHT.name(QColor::HexRgb)));
    }
} // namespace view
