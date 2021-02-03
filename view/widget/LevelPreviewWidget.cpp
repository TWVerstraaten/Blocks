//
// Created by teunv on 2/2/2021.
//

#include "LevelPreviewWidget.h"

#include "../FontManager.h"
#include "../ModelToPixmap.h"

#include <QDebug>
#include <QDir>
#include <QEvent>
#include <QPushButton>
#include <QScrollArea>

view::widget::LevelPreviewWidget::LevelPreviewWidget(QWidget* parent, const QString& path) : QWidget(parent), m_path(path) {
    m_layout = new QHBoxLayout(this);
    qDebug() << "Path:\t" << path;

    buildScrollArea();
    m_layout->addStretch();
    buildPreviewLabel();
    setLayout(m_layout);
    m_layout->addStretch();
}

bool view::widget::LevelPreviewWidget::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::Enter) {
        QSize previewSize(250, 250);
        m_label->setPixmap(view::modelToPixmap(m_path.toStdString() + "/" + qobject_cast<QPushButton*>(watched)->text().toStdString(), previewSize));
        qDebug() << "Hovered " << qobject_cast<QPushButton*>(watched)->text();
    } else if (event->type() == QEvent::MouseButtonDblClick) {
        emit levelSelected(m_path.toStdString() + "/" + qobject_cast<QPushButton*>(watched)->text().toStdString());
    }
    return false;
}

void view::widget::LevelPreviewWidget::buildScrollArea() {
    QDir dir(m_path, {"*.lev"}, QDir::Time, QDir::Files);

    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    auto* scrollWidget = new QWidget(this);
    scrollArea->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);
    auto* scrollLayout = new QVBoxLayout(scrollWidget);

    const auto files = dir.entryList();
    for (const auto& file : files) {
        auto* levelPushButton = new QPushButton(file, scrollWidget);
        levelPushButton->setFont(FontManager::font(FONT_ENUM::ANON_PRO_BOLD, 12));
        levelPushButton->installEventFilter(this);
        scrollLayout->addWidget(levelPushButton);
    }

    scrollLayout->addStretch();
    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);
    m_layout->addWidget(scrollArea);
}

void view::widget::LevelPreviewWidget::buildPreviewLabel() {
    m_label = new QLabel(this);
    QSize previewSize(250, 250);
    m_label->setFixedSize(previewSize);
    m_layout->addWidget(m_label);
}
