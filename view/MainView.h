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

        void init();
        void init(const model::Model& model);

        void startRunning();
        void stopRunning();
        void setCommandScrollArea(CommandScrollArea* commandScrollArea);
        void setModel(const std::unique_ptr<model::Model>& model);
        void setViewPort(const ViewPort& viewPort);

        [[nodiscard]] CommandScrollArea*    commandScrollArea();
        [[nodiscard]] model::Model*         model() const;
        [[nodiscard]] MainViewMouseManager& mainViewMouseManager();
        [[nodiscard]] const ViewPort&       viewPort() const;

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
        std::unique_ptr<model::Model> m_modelStash;
        CommandScrollArea*            m_commandScrollArea;
        MainViewPainter               m_mainViewPainter;
        MainViewMouseManager          m_mainViewMouseManager;
    };
} // namespace view

#endif // MAINVIEW_H
