//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_MAINVIEWPAINTER_H
#define BLOCKS_MAINVIEWPAINTER_H

#include "../../model/Model_typedefs.h"
#include "../../model/PhaseTransformation.h"

#include <QPaintEvent>
#include <QPainter>

namespace model {
    class Model;
    class Cluster;
} // namespace model

namespace view {

    class MainView;

    class MainViewPainter {

      public:
        static void paint(QPainter& painter, const QRect& rect, const model::Model& model, const view::ViewPort& viewPort, bool paintNames = true);

      private:
        friend class MainView;

        explicit MainViewPainter(const MainView* mainView);

        void        paint(QPainter& painter, QPaintEvent* event);
        static void drawConnected(const model::GridXyVector& blocks, const QColor& color, QPainter& painter, const view::ViewPort& viewPort);
        static void drawConnected(const model::GridXyVector&        blocks,
                                  const QColor&                     color,
                                  QPainter&                         painter,
                                  double                            angle,
                                  const model::PhaseTransformation& f,
                                  const view::ViewPort&             viewPort);
        static void drawCluster(const model::Cluster& cluster, QPainter& painter, const view::ViewPort& viewPort, bool paintNames);
        [[nodiscard]] static QPixmap connectedPixmap(const model::GridXyVector& blocks, const QColor& color, const view::ViewPort& viewPort);

        const MainView* m_mainView;
        const ViewPort* m_viewPort;
    };

} // namespace view

#endif // BLOCKS_MAINVIEWPAINTER_H
