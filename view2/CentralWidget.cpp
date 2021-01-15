#include "CentralWidget.h"

#include "../model/Model.h"
#include "MainView.h"

#include <QKeyEvent>

namespace view2 {

    CentralWidget::CentralWidget() {
        auto* layout = new QGridLayout;

        setGeometry(0, 0, 1000, 800);
        setLayout(layout);

        m_mainView = new MainView(this);
        layout->addWidget(m_mainView, 0, 0);
        m_commandScrollArea = new CommandScrollArea(this);
        layout->addWidget(m_commandScrollArea, 0, 1);
    }

    CommandScrollArea* CentralWidget::commandScrollArea() const {
        return m_commandScrollArea;
    }

    MainView* CentralWidget::mainView() const {
        return m_mainView;
    }

    void CentralWidget::keyPressEvent(QKeyEvent* event) {
        switch (event->key()) {
            case Qt::Key_Space:
                qDebug() << "Space pressed";
                break;
        }
    }

} // namespace view2