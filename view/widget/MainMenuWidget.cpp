//
// Created by pc on 01-02-21.
//

#include "MainMenuWidget.h"

#include "../FontManager.h"

view::widget::MainMenuWidget::MainMenuWidget(QWidget* parent) : QTabWidget(parent) {
    setFont(FontManager::font(FONT_ENUM::ANON_PRO_BOLD, 15));

    m_levelSelectWidget = new LevelSelectWidget(this);
    auto* p             = new QLabel("Temp", this);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    addTab(m_levelSelectWidget, "Level Select");
    addTab(p, "Settings");
}

view::widget::LevelSelectWidget* view::widget::MainMenuWidget::levelSelectWidget() const {
    assert(m_levelSelectWidget != nullptr);
    return m_levelSelectWidget;
}