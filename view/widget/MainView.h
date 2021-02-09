#ifndef BLOCKS_MAINVIEW_H
#define BLOCKS_MAINVIEW_H

#include "../../view/ViewPort.h"

#include <QWidget>
#include <memory>

namespace model {
    class Model;
}

namespace contr {
    class MainViewMouseManager;
}

namespace view::widget {

    class MainViewPainter;
    class CommandScroll;
    class CentralWidget;

    class MainView : public QWidget {
        Q_OBJECT

      public:
        explicit MainView(CentralWidget* centralWidget);

        ~MainView() override;

        void init(const std::string& path);
        void init(const model::Model& model);

        void setCommandScrollArea(CommandScroll* commandScrollArea);
        void setViewPort(const ViewPort& viewPort);

        [[nodiscard]] CommandScroll*               commandScrollArea();
        [[nodiscard]] model::Model*                model() const;
        [[nodiscard]] contr::MainViewMouseManager& mainViewMouseManager();
        [[nodiscard]] const ViewPort&              viewPort() const;

        [[nodiscard]] ViewPort& viewPort();

      protected:
        void paintEvent(QPaintEvent* event) override;
        void wheelEvent(QWheelEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;

      private:
        friend class MainViewPainter;

        CentralWidget*               m_centralWidget = nullptr;
        view::ViewPort               m_viewPort;
        model::Model*                m_model;
        CommandScroll*               m_commandScrollArea = nullptr;
        MainViewPainter*             m_mainViewPainter   = nullptr;
        contr::MainViewMouseManager* m_mainViewMouseManager;
    };
} // namespace view::widget

#endif // BLOCKS_MAINVIEW_H
