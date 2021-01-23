#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "view/ViewPort.h"

#include <QWidget>
#include <memory>

namespace model {
    class Model;
}

namespace cont {
    class MainViewMouseManager;
}

namespace view {

    class MainViewPainter;
    class CommandScrollArea;
    class CentralWidget;

    class MainView : public QWidget {
        Q_OBJECT

      public:
        explicit MainView(CentralWidget* centralWidget);

        ~MainView() override;

        void init();
        void init(const model::Model& model);

        void setCommandScrollArea(CommandScrollArea* commandScrollArea);
        void setViewPort(const ViewPort& viewPort);

        [[nodiscard]] CommandScrollArea*          commandScrollArea();
        [[nodiscard]] model::Model*               model() const;
        [[nodiscard]] cont::MainViewMouseManager& mainViewMouseManager();
        [[nodiscard]] const ViewPort&             viewPort() const;

        [[nodiscard]] ViewPort& viewPort();

      protected:
        void paintEvent(QPaintEvent* event) override;
        void wheelEvent(QWheelEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;

      private:
        friend class MainViewPainter;

        CentralWidget*              m_centralWidget;
        view::ViewPort              m_viewPort;
        model::Model*               m_model;
        CommandScrollArea*          m_commandScrollArea;
        MainViewPainter*            m_mainViewPainter;
        cont::MainViewMouseManager* m_mainViewMouseManager;
    };
} // namespace view

#endif // MAINVIEW_H
