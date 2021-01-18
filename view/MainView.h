#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "MainViewMouseManager.h"
#include "MainViewPainter.h"
#include "ViewPort.h"
#include "model/Model.h"

#include <QWidget>
#include <memory>

namespace model {
    class Model;
}

namespace view {
    class CommandScrollArea;
    class CentralWidget;

    class MainView : public QWidget {
        Q_OBJECT
      public:
        explicit MainView(CentralWidget* centralWidget);

        [[nodiscard]] model::Model* model() const;

      protected:
        void paintEvent(QPaintEvent* event) override;
        void wheelEvent(QWheelEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;

      private:
        friend class MainViewPainter;
        friend class MainViewMouseManager;

        CentralWidget*                m_centralWidget;
        view::ViewPort                m_viewPort;
        std::unique_ptr<model::Model> m_model;
        CommandScrollArea*            m_commandScrollArea;
        MainViewPainter               m_mainViewPainter;
        MainViewMouseManager          m_mainViewMouseManager;
    };
} // namespace view

#endif // MAINVIEW_H
