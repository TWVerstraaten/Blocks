#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "../view/ViewPort.h"
#include "MainWindow.h"

#include <QWidget>
#include <memory>

namespace model {
    class Model;
}

namespace view2 {
    class MainView : public QWidget {
        Q_OBJECT
      public:
        explicit MainView(QWidget* parent);
        void init(CommandScrollArea* commandScrollArea);

      protected:
        void paintEvent(QPaintEvent* event) override;
        void wheelEvent(QWheelEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;

      private:
        void                                   drawConnected(const model::GridXYSet& blocks, const QColor& color, QPainter& painter) const;
        [[nodiscard]] std::unique_ptr<QPixmap> connectedPixmap(const model::GridXYSet& blocks, const QColor& color) const;

        void mouseLeftPressEvent();
        void mouseLeftDragEvent(const model::GridXY& currentGridXY);
        void removeBlock(const model::GridXY& gridXy);
        void addBlock(const model::GridXY& gridXy);

        bool                          m_isInitialized = false;
        view::ScreenXY                m_previousMousePosition;
        model::GridXY                 m_previousGridPosition{0, 0};
        view::ViewPort                m_viewPort;
        std::unique_ptr<model::Model> m_model;
        CommandScrollArea*            m_commandScrollArea = nullptr;
    };
} // namespace view2
#endif // MAINVIEW_H
