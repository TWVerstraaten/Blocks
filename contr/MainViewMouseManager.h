//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_MAINVIEWMOUSEMANAGER_H
#define BLOCKS_MAINVIEWMOUSEMANAGER_H

#include "../model/GridXy.h"
#include "../view/BlockType.h"
#include "../view/ScreenXy.h"

#include <QMouseEvent>

namespace model {
    class Model;
}

namespace view::widget {
    class MainView;
    class CentralWidget;
} // namespace view::widget

namespace contr {

    class MainViewMouseManager {

      public:
        void setBlockEditing(bool blockEditing);

        explicit MainViewMouseManager(view::widget::MainView* mainView);

        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void setCentralWidget(view::widget::CentralWidget* centralWidget);
        void setModel(model::Model* model);

      private:
        void mouseLeftPressEvent();

        void mouseLeftDragEvent(const model::GridXy& currentGridXy, view::CLUSTER_BLOCK type);
        void mouseLeftDragEvent(const model::GridXy& currentGridXy, model::FLOOR_BLOCK_TYPE type);
        void mouseLeftDragEvent(const model::GridXy& currentGridXy, model::DYNAMIC_BLOCK_TYPE type);
        void mouseLeftDragEvent(const model::GridXy& currentGridXy, model::INSTANT_BLOCK_TYPE type);
        void removeBlock(const model::GridXy& gridXy, view::CLUSTER_BLOCK type);
        void removeBlock(const model::GridXy& gridXy, model::FLOOR_BLOCK_TYPE type);
        void removeBlock(const model::GridXy& gridXy, model::DYNAMIC_BLOCK_TYPE type);
        void removeBlock(const model::GridXy& gridXy, model::INSTANT_BLOCK_TYPE type);
        void removeTopBlockFromLevel(const model::GridXy& gridXy);
        void addBlock(const model::GridXy& gridXy, view::CLUSTER_BLOCK type);
        void addBlock(const model::GridXy& gridXy, model::FLOOR_BLOCK_TYPE type);
        void addBlock(const model::GridXy& gridXy, model::DYNAMIC_BLOCK_TYPE type);
        void addBlock(const model::GridXy& gridXy, model::INSTANT_BLOCK_TYPE type);

        bool                         m_blockEditing = false;
        view::widget::MainView*      m_mainView;
        view::widget::CentralWidget* m_centralWidget = nullptr;
        model::Model*                m_model         = nullptr;
        view::ScreenXy               m_previousMousePosition;
        model::GridXy                m_previousGridPosition{0, 0};
    };

} // namespace contr

#endif // BLOCKS_MAINVIEWMOUSEMANAGER_H
