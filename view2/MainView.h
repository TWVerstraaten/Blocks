#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "../model/Model.h"
#include "../view/ViewPort.h"
#include "MainViewPainter.h"

#include <QWidget>
#include <memory>

namespace model {
    class Model;
}

namespace view2 {
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
        void mouseReleaseEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;

      private:
        friend class MainViewPainter;

        void mouseLeftPressEvent();
        void mouseLeftDragEvent(const model::GridXY& currentGridXY);
        void removeBlock(const model::GridXY& gridXy);
        void createCluster(const model::GridXY& gridXy);

        CentralWidget*                m_centralWidget;
        view::ScreenXY                m_previousMousePosition;
        model::GridXY                 m_previousGridPosition{0, 0};
        view::ViewPort                m_viewPort;
        std::unique_ptr<model::Model> m_model;
        CommandScrollArea*            m_commandScrollArea;
        MainViewPainter               m_mainViewPainter;
    };
} // namespace view2
#endif // MAINVIEW_H
