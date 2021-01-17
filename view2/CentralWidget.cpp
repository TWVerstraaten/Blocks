#include "CentralWidget.h"

#include "../model/Model.h"
#include "MainView.h"

#include <QApplication>
#include <QKeyEvent>
#include <QTimer>

namespace view2 {

    CentralWidget::CentralWidget() {
        auto* layout = new QGridLayout;

        setGeometry(0, 0, 1000, 800);
        setLayout(layout);

        m_mainView = new MainView(this);
        layout->addWidget(m_mainView, 0, 0);
        m_commandScrollArea = new CommandScrollArea(this);
        layout->addWidget(m_commandScrollArea, 0, 1);
        m_mainView->init(m_commandScrollArea);
        QUndoGroup q;
    }

    void CentralWidget::keyPressEvent(QKeyEvent* event) {
        switch (event->key()) {
            case Qt::Key_Space:
                qDebug() << "Space pressed";
                break;
        }
    }

} // namespace view2