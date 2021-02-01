//
// Created by pc on 01-02-21.
//

#include "LevelSelectWidget.h"

#include <QDebug>
#include <QDirIterator>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>

view::widget::LevelSelectWidget::LevelSelectWidget(QWidget* parent) : QScrollArea(parent) {
    auto* t      = new QGroupBox(this);
    auto* layout = new QVBoxLayout(t);
    t->setLayout(layout);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    setWidgetResizable(true);

    QPalette pal = t->palette();
    pal.setColor(QPalette::ColorRole::Window, Qt::white);
    t->setPalette(pal);

    QDir dir("levels");
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    const auto e = dir.entryList();
    for (const auto& l : e) {
        layout->addWidget(new QPushButton(l, t));
    }

    layout->addStretch();
    t->setTitle("Levels");
    setWidget(t);
}
