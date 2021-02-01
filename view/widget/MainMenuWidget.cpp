//
// Created by pc on 01-02-21.
//

#include "MainMenuWidget.h"

#include "../FontManager.h"
#include "../color.h"
#include "LevelSelectWidget.h"

#include <QLabel>

view::widget::MainMenuWidget::MainMenuWidget(QWidget* parent) : QTabWidget(parent) {
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(view::color::BACKGROUND));
    setPalette(palette);
    setAutoFillBackground(true);

    setFont(FontManager::font(FONT_ENUM::ANON_PRO_BOLD, 15));

    auto* q = new LevelSelectWidget(this);
    auto* p = new QLabel("Haoi", this);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    addTab(p, "sdss");
    addTab(q, "AAASs");
}
