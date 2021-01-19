//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_MAINVIEWPAINTER_H
#define BLOCKS_MAINVIEWPAINTER_H

#include "model/Cluster.h"
#include "view/ViewPort.h"

#include <QPaintEvent>
#include <QPainter>

namespace model {
    class Model;
}

namespace view {

    class MainView;

    class MainViewPainter {

        friend class MainView;

        explicit MainViewPainter(const MainView* mainView);

        void paint(QPainter& painter, QPaintEvent* event);
        void drawConnected(const model::GridXYSet& blocks, const QColor& color, QPainter& painter) const;
        void drawCluster(const model::Cluster& cluster, QPainter& painter);

        [[nodiscard]] QPixmap connectedPixmap(const model::GridXYSet& blocks, const QColor& color) const;

        const MainView* m_mainView;
        QFont           m_font;
    };

} // namespace view

#endif // BLOCKS_MAINVIEWPAINTER_H
