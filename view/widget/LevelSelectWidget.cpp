//
// Created by pc on 01-02-21.
//

#include "LevelSelectWidget.h"

#include "../../misc/geom.h"
#include "../FontManager.h"
#include "../ModelToPixmap.h"
#include "LevelPreviewWidget.h"
#include "MainViewPainter.h"

#include <QDebug>
#include <QDirIterator>

view::widget::LevelSelectWidget::LevelSelectWidget(QWidget* parent) : QWidget(parent) {
    m_hBoxLayout     = new QHBoxLayout(this);
    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    auto* scrollWidget = new QWidget(this);
    scrollArea->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);
    auto* scrollLayout = new QVBoxLayout(scrollWidget);

    QDir dir("levels");
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    const auto e = dir.entryList();
    for (const auto& path : e) {
        auto* p = new QPushButton(path, scrollWidget);
        p->setFont(FontManager::font(FONT_ENUM::ANON_PRO_BOLD, 12));
        connect(p, &QPushButton::pressed, [this, path] { populatePreviewWidget(path); });
        //        connect(p, &QPushButton::pressed, [this, m_path] { emit levelSelected("levels/" + m_path.toStdString() + "/level1.lev"); });
        scrollLayout->addWidget(p);
    }

    scrollLayout->addStretch();
    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);

    m_hBoxLayout->addWidget(scrollArea);
    m_rightWidget = new QWidget(this);
    m_hBoxLayout->addWidget(m_rightWidget);
}

void view::widget::LevelSelectWidget::populatePreviewWidget(const QString& path) {
    const auto prefix = "levels/" + path;

    m_hBoxLayout->removeWidget(m_rightWidget);
    delete m_rightWidget;
    m_rightWidget = new LevelPreviewWidget(this, prefix);
    m_hBoxLayout->addWidget(m_rightWidget);

    connect(qobject_cast<LevelPreviewWidget*>(m_rightWidget), &LevelPreviewWidget::levelSelected, [this](const std::string& path) {
        emit levelSelected(path);
    });
}
