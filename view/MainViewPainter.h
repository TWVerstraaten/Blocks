//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_MAINVIEWPAINTER_H
#define BLOCKS_MAINVIEWPAINTER_H

#include "../model/Model_typedefs.h"
#include "../model/PhaseTransformation.h"

#include <QPaintEvent>
#include <QPainter>

namespace model {
    class Model;
    class Cluster;
} // namespace model

namespace view {

    class MainView;

    class MainViewPainter {

        friend class MainView;

        explicit MainViewPainter(const MainView* mainView);

        void paint(QPainter& painter, QPaintEvent* event);
        void drawConnected(const model::GridXySet& blocks, const QColor& color, QPainter& painter) const;
        void drawConnected(
            const model::GridXySet& blocks, const QColor& color, QPainter& painter, double angle, const model::PhaseTransformation& f) const;
        void drawCluster(const model::Cluster& cluster, QPainter& painter);

        [[nodiscard]] QPixmap connectedPixmap(const model::GridXySet& blocks, const QColor& color) const;

        const MainView* m_mainView;
        const ViewPort* m_viewPort;
        QFont           m_font;
    };

} // namespace view

#endif // BLOCKS_MAINVIEWPAINTER_H
