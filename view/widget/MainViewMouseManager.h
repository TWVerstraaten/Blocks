//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_MAINVIEWMOUSEMANAGER_H
#define BLOCKS_MAINVIEWMOUSEMANAGER_H

#include "model/GridXy.h"
#include "view/BlockType.h"
#include "view/ScreenXy.h"

#include <QMouseEvent>

namespace model {
    class Model;
}

namespace view {

    class MainView;
    class CentralWidget;

    class MainViewMouseManager {

      public:
        void setBlockEditing(bool blockEditing);

      private:
        friend class MainView;

        explicit MainViewMouseManager(MainView* mainView);

        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void mouseLeftPressEvent();

        void mouseLeftDragEvent(const model::GridXy& currentGridXy, CLUSTER_BLOCK type);
        void mouseLeftDragEvent(const model::GridXy& currentGridXy, model::FLOOR_BLOCK_TYPE type);
        void mouseLeftDragEvent(const model::GridXy& currentGridXy, model::DYNAMIC_BLOCK_TYPE type);
        void mouseLeftDragEvent(const model::GridXy& currentGridXy, model::INSTANT_BLOCK_TYPE type);
        void removeBlock(const model::GridXy& gridXy, CLUSTER_BLOCK type);
        void removeBlock(const model::GridXy& gridXy, model::FLOOR_BLOCK_TYPE type);
        void removeBlock(const model::GridXy& gridXy, model::DYNAMIC_BLOCK_TYPE type);
        void removeBlock(const model::GridXy& gridXy, model::INSTANT_BLOCK_TYPE type);
        void removeTopBlockFromLevel(const model::GridXy& gridXy);
        void addBlock(const model::GridXy& gridXy, CLUSTER_BLOCK type);
        void addBlock(const model::GridXy& gridXy, model::FLOOR_BLOCK_TYPE type);
        void addBlock(const model::GridXy& gridXy, model::DYNAMIC_BLOCK_TYPE type);
        void addBlock(const model::GridXy& gridXy, model::INSTANT_BLOCK_TYPE type);

        MainView*      m_mainView;
        bool           m_blockEditing  = false;
        CentralWidget* m_centralWidget = nullptr;
        model::Model*  m_model         = nullptr;
        view::ScreenXy m_previousMousePosition;
        model::GridXy  m_previousGridPosition{0, 0};
    };

} // namespace view

#endif // BLOCKS_MAINVIEWMOUSEMANAGER_H
