#include "MainWidget.h"

#include "../model/Model.h"
#include "MainView.h"

#include <QKeyEvent>

namespace view2 {

    MainWidget::MainWidget() {
        auto* layout = new QGridLayout;

        setGeometry(0, 0, 1000, 800);
        setLayout(layout);

        m_mainView = new MainView(this);
        layout->addWidget(m_mainView, 0, 0);
        m_scrollArea = new CommandScrollArea(this);
        layout->addWidget(m_scrollArea, 0, 1);
    }

    CommandScrollArea* MainWidget::scrollArea() const {
        return m_scrollArea;
    }

    MainView* MainWidget::mainView() const {
        return m_mainView;
    }

    void MainWidget::keyPressEvent(QKeyEvent* event) {
        switch (event->key()) {
            case Qt::Key_Space:
                qDebug() << "Space pressed";
                break;
        }
    }

} // namespace view2