#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "../view/ViewPort.h"
#include "MainWindow.h"

#include <QWidget>

namespace model {
    class Model;
}

namespace view2 {

    class MainView : public QWidget {
        Q_OBJECT
      public:
        explicit MainView(QWidget* parent);
        void setCurrentModel(const model::Model* model);

      protected:
        void paintEvent(QPaintEvent* event) override;
        void wheelEvent(QWheelEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;

      private:
        void drawConnected(const model::GridXYSet& blocks, const QColor& color, QPainter& painter) const;

        QPoint              m_previousMousePosition;
        view::ViewPort      m_viewPort;
        const model::Model* m_currentModel = nullptr;
    };
} // namespace view2
#endif // MAINVIEW_H
