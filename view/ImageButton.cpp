//
// Created by teunv on 1/18/2021.
//

#include "ImageButton.h"

#include "toPixmap.h"

view::ImageButton::ImageButton(QWidget* parent, const view::BlockType& type, const QSize& size)
    : QPushButton(parent), m_pixmap(toPixmapVariant(type, size)) {
    setIconSize(size - QSize{6, 6});
    setIcon(m_pixmap);
    setFixedSize(size);
    update();
    setStyleSheet(QString("QPushButton {\n"
                          "    background-color: rgba(255,255,255,0);\n"
                          "}\n"
                          "QPushButton:hover {\n"
                          "    background-color: rgb(%1,%2,%3);\n"
                          "}\n")
                      .arg(view::color::WIDGET_LIGHT.red())
                      .arg(view::color::WIDGET_LIGHT.green())
                      .arg(view::color::WIDGET_LIGHT.blue()));
}

void view::ImageButton::setSelected() {
    setStyleSheet(QString("QPushButton {\n"
                          "    background-color: rgb(%1,%2,%3);\n"
                          "border: 3px solid red;"
                          "}\n")
                      .arg(view::color::WIDGET_LIGHT.red())
                      .arg(view::color::WIDGET_LIGHT.green())
                      .arg(view::color::WIDGET_LIGHT.blue()));
}

void view::ImageButton::setUnselected() {
    setStyleSheet(QString("QPushButton {\n"
                          "    background-color: rgba(255,255,255,0);\n"
                          "}\n"
                          "QPushButton:hover {\n"
                          "    background-color: rgb(%1,%2,%3);\n"
                          "}\n")
                      .arg(view::color::WIDGET_LIGHT.red())
                      .arg(view::color::WIDGET_LIGHT.green())
                      .arg(view::color::WIDGET_LIGHT.blue()));
}
