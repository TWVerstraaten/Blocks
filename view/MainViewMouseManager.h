//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_MAINVIEWMOUSEMANAGER_H
#define BLOCKS_MAINVIEWMOUSEMANAGER_H

#include "BlockSelectWidget.h"
#include "ScreenXY.h"
#include "model/GridXY.h"

#include <QMouseEvent>

namespace model {
    class Model;
}

namespace view {

    class MainView;
    class CentralWidget;

    class MainViewMouseManager {

        friend class MainView;

        explicit MainViewMouseManager(MainView* mainView);

        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void mouseLeftPressEvent();

        void mouseLeftDragEvent(const model::GridXY& currentGridXY, CLUSTER_BLOCK type);
        void mouseLeftDragEvent(const model::GridXY& currentGridXY, model::FLOOR_BLOCK_TYPE type);
        void mouseLeftDragEvent(const model::GridXY& currentGridXY, model::DYNAMIC_BLOCK_TYPE type);
        void mouseLeftDragEvent(const model::GridXY& currentGridXY, model::INSTANT_BLOCK_TYPE type);
        void removeBlock(const model::GridXY& gridXy, CLUSTER_BLOCK type);
        void removeBlock(const model::GridXY& gridXy, model::FLOOR_BLOCK_TYPE type);
        void removeBlock(const model::GridXY& gridXy, model::DYNAMIC_BLOCK_TYPE type);
        void removeBlock(const model::GridXY& gridXy, model::INSTANT_BLOCK_TYPE type);
        void addBlock(const model::GridXY& gridXy, CLUSTER_BLOCK type);
        void addBlock(const model::GridXY& gridXy, model::FLOOR_BLOCK_TYPE type);
        void addBlock(const model::GridXY& gridXy, model::DYNAMIC_BLOCK_TYPE type);
        void addBlock(const model::GridXY& gridXy, model::INSTANT_BLOCK_TYPE type);

        MainView*      m_mainView;
        CentralWidget* m_centralWidget;
        model::Model*  m_model;
        view::ScreenXY m_previousMousePosition;
        model::GridXY  m_previousGridPosition{0, 0};
    };

} // namespace view

#endif // BLOCKS_MAINVIEWMOUSEMANAGER_H
