//
// Created by teunv on 2/2/2021.
//

#include "LevelPreviewWidget.h"

#include "../FontManager.h"
#include "../ModelToPixmap.h"

#include <QDebug>
#include <QDir>
#include <QEvent>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>

view::widget::LevelPreviewWidget::LevelPreviewWidget(QWidget* parent, const QString& path) : QWidget(parent), m_path(path) {
    auto* layout = new QHBoxLayout(this);
    qDebug() << "Path:\t" << path;
    QDir dir(path);
    dir.setFilter(QDir::Files);
    QStringList filters;
    filters << "*.lev";
    dir.setNameFilters(filters);

    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    auto* scrollWidget = new QWidget(this);
    scrollArea->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);
    auto* scrollLayout = new QVBoxLayout(scrollWidget);

    const auto e = dir.entryList();
    for (const auto& file : e) {
        auto* p = new QPushButton(file, scrollWidget);
        p->setFont(FontManager::font(FONT_ENUM::ANON_PRO_BOLD, 12));
        p->installEventFilter(this);
        scrollLayout->addWidget(p);
    }

    scrollLayout->addStretch();
    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);
    layout->addWidget(scrollArea);
    layout->addStretch();
    m_label = new QLabel(this);
    QSize previewSize(250, 250);
    m_label->setFixedSize(previewSize);
    layout->addWidget(m_label);
    setLayout(layout);
    layout->addStretch();
}

void view::widget::LevelPreviewWidget::populatePreview(const QString& string) {
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
