//
// Created by pc on 01-02-21.
//

#include "LevelSelectWidget.h"

#include "../../app/Application_constants.h"
#include "../../misc/geom.h"
#include "../../model/Model.h"
#include "../ModelToPixmap.h"
#include "../ViewPort.h"
#include "../color.h"
#include "MainViewPainter.h"

#include <QDebug>
#include <QDirIterator>
#include <QHBoxLayout>
#include <QPainter>
#include <QPushButton>

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
    for (const auto& l : e) {
        auto*    p = new QPushButton(l, scrollWidget);
        QPalette pal;
        pal.setColor(QPalette::Base, view::color::WIDGET_LIGHT);
        p->setAutoFillBackground(true);
        p->setPalette(pal);
        connect(p, &QPushButton::pressed, [this, l] { populatePreviewWidget(l); });
        //        connect(p, &QPushButton::pressed, [this, l] { emit levelSelected(l.toStdString()); });
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
    qDebug() << path;
    QDir dir("levels");
    dir.setFilter(QDir::Files);
    const auto e = dir.entryList();
    for (const auto& l : e) {
        qDebug() << l;
    }

    m_hBoxLayout->removeWidget(m_rightWidget);
    m_rightWidget = new QWidget(this);

    auto* g = new QGridLayout(m_rightWidget);
    auto* l = new QLabel(m_rightWidget);
    l->setFixedSize(350, 350);
    l->setPixmap(view::modelToPixmap("levels/" + path.toStdString() + "/level1.dat", QSize(350, 350)));
    g->addWidget(l);
    m_hBoxLayout->addWidget(m_rightWidget);
    update();
}
