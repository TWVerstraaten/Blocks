//
// Created by pc on 01-02-21.
//

#include "LevelSelectWidget.h"

#include "../color.h"

#include <QDebug>
#include <QDirIterator>
#include <QHBoxLayout>
#include <QLabel>
#include <QPaintEvent>
#include <QPushButton>

view::widget::LevelSelectWidget::LevelSelectWidget(QWidget* parent) : QWidget(parent) {
    auto* hBoxLayout = new QHBoxLayout(this);
    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    auto* scrollWidget = new QWidget(this);
    scrollArea->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);
    auto* scrollLayout = new QVBoxLayout(scrollWidget);

    QDir dir("levels");
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    const auto e = dir.entryList();
    for (const auto& l : e) {
        auto*    p = new QPushButton(l, scrollWidget);
        QPalette pal;
        pal.setColor(QPalette::Base, view::color::WIDGET_LIGHT);
        p->setAutoFillBackground(true);
        p->setPalette(pal);
        //        connect(p, &QPushButton::pressed, [this, l] { populatePreviewWidget(l); });
        connect(p, &QPushButton::pressed, [this, l] { emit levelSelected(l.toStdString()); });
        scrollLayout->addWidget(p);
    }

    scrollLayout->addStretch();
    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);

    hBoxLayout->addWidget(scrollArea);
    hBoxLayout->addWidget(new QLabel("asdsasad", this));
}

void view::widget::LevelSelectWidget::populatePreviewWidget(const QString& path) {
    qDebug() << path;
    QDir dir("levels");
    dir.setFilter(QDir::Files);
    const auto e = dir.entryList();
    for (const auto& l : e) {
        qDebug() << l;
    }

    QRect rect{0, 0, 200, 200};
    auto* paintEvent = new QPaintEvent{rect};
}
