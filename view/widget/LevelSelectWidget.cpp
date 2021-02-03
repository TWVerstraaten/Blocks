//
// Created by pc on 01-02-21.
//

#include "LevelSelectWidget.h"

#include "../../misc/geom.h"
#include "../FontManager.h"
#include "LevelPreviewWidget.h"
#include "MainViewPainter.h"

#include <QDebug>
#include <QDirIterator>

view::widget::LevelSelectWidget::LevelSelectWidget(QWidget* parent) : QWidget(parent) {
    m_hBoxLayout = new QHBoxLayout(this);
    buildScrollArea();
    m_rightWidget = new QWidget(this);
    m_hBoxLayout->addWidget(m_rightWidget);
}

void view::widget::LevelSelectWidget::populatePreviewWidget(const QString& path) {
    const auto prefix = "dat/levels/" + path;

    m_hBoxLayout->removeWidget(m_rightWidget);
    delete m_rightWidget;
    m_rightWidget = new LevelPreviewWidget(this, prefix);
    m_hBoxLayout->addWidget(m_rightWidget);

    connect(qobject_cast<LevelPreviewWidget*>(m_rightWidget), &LevelPreviewWidget::levelSelected, [this](const std::string& path) {
        emit levelSelected(path);
    });
}

void view::widget::LevelSelectWidget::buildScrollArea() {
    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    auto* scrollWidget = new QWidget(this);
    scrollArea->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);
    auto* scrollLayout = new QVBoxLayout(scrollWidget);

    QDir dir("dat/levels");
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    const auto directory = dir.entryList();
    for (const auto& path : directory) {
        auto* directoryPushButton = new QPushButton(path, scrollWidget);
        directoryPushButton->setFont(FontManager::font(FONT_ENUM::ANON_PRO_BOLD, 12));
        connect(directoryPushButton, &QPushButton::pressed, [this, path] { populatePreviewWidget(path); });
        scrollLayout->addWidget(directoryPushButton);
    }

    scrollLayout->addStretch();
    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);
    m_hBoxLayout->addWidget(scrollArea);
}
