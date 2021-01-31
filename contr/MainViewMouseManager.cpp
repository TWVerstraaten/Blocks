//
// Created by teunv on 1/18/2021.
//

#include "MainViewMouseManager.h"

#include "../action/AddFloorBlockAction.h"
#include "../action/AddLevelBlockAction.h"
#include "../action/ChangeFloorBlockAction.h"
#include "../action/ChangeLevelBlockAction.h"
#include "../action/DeleteClusterAction.h"
#include "../action/MergeClusterAction.h"
#include "../action/RemoveBlockFromClusterAction.h"
#include "../action/RemoveFloorBlockAction.h"
#include "../action/RemoveLevelBlockAction.h"
#include "../action/SplitDisconnectedAction.h"
#include "../model/Model.h"
#include "../view/widget/BlockSelectWidget.h"
#include "../view/widget/CentralWidget.h"

#include <QApplication>

namespace contr {

    using namespace model;
    using namespace action;

    MainViewMouseManager::MainViewMouseManager(view::MainView* mainView) : m_mainView(mainView) {
    }

    void MainViewMouseManager::mousePressEvent(QMouseEvent* event) {
        m_previousMousePosition = event->pos();
        m_previousGridPosition  = GridXy::fromScreenXy(m_previousMousePosition, m_mainView->viewPort());
        if (not m_blockEditing) {
            if (event->button() == Qt::MouseButton::LeftButton) {
                mouseLeftPressEvent();
            }
        }
    }

    void MainViewMouseManager::mouseMoveEvent(QMouseEvent* event) {
        const view::ScreenXy currentMousePosition = event->pos();
        const GridXy         currentGridPosition  = GridXy::fromScreenXy(currentMousePosition, m_mainView->viewPort());
        switch (event->buttons()) {
            case Qt::RightButton:
                m_mainView->viewPort().translate((event->x() - m_previousMousePosition.x()), event->y() - m_previousMousePosition.y());
                m_previousMousePosition = event->pos();
                break;
            case Qt::LeftButton:
                if (m_previousGridPosition != currentGridPosition && (not m_blockEditing)) {
                    std::visit([this, currentGridPosition](const auto& a) { mouseLeftDragEvent(currentGridPosition, a); },
                               m_centralWidget->blockSelectWidget()->selectedBlockType());
                }
            default:
                break;
        }
        m_previousMousePosition = currentMousePosition;
        m_previousGridPosition  = currentGridPosition;
    }

    void MainViewMouseManager::mouseLeftPressEvent() {
        if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
            std::visit([this](const auto& a) { removeBlock(m_previousGridPosition, a); }, m_centralWidget->blockSelectWidget()->selectedBlockType());
        } else {
            std::visit([this](const auto& a) { addBlock(m_previousGridPosition, a); }, m_centralWidget->blockSelectWidget()->selectedBlockType());
        }
    }

    void MainViewMouseManager::mouseLeftDragEvent(const GridXy& currentGridXy, view::CLUSTER_BLOCK type) {
        if (not m_model->level().isFreeStartBlock(currentGridXy)) {
            return;
        }
        if (m_model->noLiveOrStoppedClusterOnBlock(m_previousGridPosition) || not currentGridXy.isAdjacent(m_previousGridPosition)) {
            m_previousGridPosition = currentGridXy;
            mouseLeftPressEvent();
        } else {
            m_centralWidget->startActionGlob();
            addBlock(currentGridXy, type);
            auto baseIt      = m_model->clusterContaining(m_previousGridPosition);
            auto extensionIt = m_model->clusterContaining(currentGridXy);
            if (extensionIt != m_model->clusters().end() && baseIt->index() == extensionIt->index()) {
                m_centralWidget->stopActionGlob();
                return;
            }
            extensionIt = m_model->clusterContaining(currentGridXy);
            baseIt      = m_model->clusterContaining(m_previousGridPosition);
            if (baseIt->index() != extensionIt->index()) {

                assert(baseIt != m_model->clusters().end());
                assert(extensionIt != m_model->clusters().end());
                m_centralWidget->addAction(new MergeClusterAction(m_model, *baseIt, *extensionIt, m_mainView->commandScrollArea()));
            }
            m_centralWidget->stopActionGlob();
        }
    }

    void MainViewMouseManager::removeBlock(const GridXy& gridXy, [[maybe_unused]] view::CLUSTER_BLOCK type) {
        auto it = m_model->clusterContaining(gridXy);
        if (it == m_model->clusters().end()) {
            return;
        }
        if (it->size() == 1) {
            m_centralWidget->addAction(new DeleteClusterAction(m_centralWidget, *it));
        } else {
            m_centralWidget->startActionGlob();
            m_centralWidget->addAction(new RemoveBlockFromClusterAction(m_model, it->index(), gridXy));
            if (not it->isConnected()) {
                m_centralWidget->addAction(new SplitDisconnectedAction(m_model, *it, m_mainView->commandScrollArea()));
            }
            m_centralWidget->stopActionGlob();
        }
    }

    void MainViewMouseManager::addBlock(const GridXy& gridXy, [[maybe_unused]] view::CLUSTER_BLOCK type) {
        if (m_model->noLiveOrStoppedClusterOnBlock(gridXy) && m_model->level().isFreeStartBlock(gridXy)) {
            m_model->clusters().emplace_back(gridXy, "CL" + std::to_string(m_model->clusters().size()));
            m_mainView->commandScrollArea()->add(m_model->clusters().back());
            m_centralWidget->addAction(new NewClusterAction(m_centralWidget, m_model->clusters().back()));
        }
    }

    void MainViewMouseManager::addBlock(const GridXy& gridXy, FLOOR_BLOCK_TYPE type) {
        if (m_model->level().floorBlocks().find(gridXy) == m_model->level().floorBlocks().end()) {
            m_centralWidget->addAction(new AddFloorBlockAction(m_model, type, gridXy));
        } else if (m_model->level().floorBlocks().at(gridXy) != type) {
            m_centralWidget->addAction(new ChangeFloorBlockAction(m_model, type, m_model->level().floorBlocks().at(gridXy), gridXy));
        }
    }

    void MainViewMouseManager::addBlock(const GridXy& gridXy, DYNAMIC_BLOCK_TYPE type) {
        const auto& level         = m_model->level();
        const auto& dynamicBlocks = level.dynamicBlocks();
        const auto& instantBlocks = level.instantBlocks();
        bool        shouldGlob    = level.floorBlocks().find(gridXy) == level.floorBlocks().end();
        if (shouldGlob) {
            m_centralWidget->startActionGlob();
            addBlock(gridXy, FLOOR_BLOCK_TYPE::LEVEL);
        }
        if (dynamicBlocks.find(gridXy) == dynamicBlocks.end() && instantBlocks.find(gridXy) == instantBlocks.end()) {
            m_centralWidget->addAction(new AddLevelBlockAction(m_model, type, gridXy));
        } else {
            if (dynamicBlocks.find(gridXy) != dynamicBlocks.end() && dynamicBlocks.at(gridXy) != type) {
                m_centralWidget->addAction(new ChangeLevelBlockAction(m_model, type, dynamicBlocks.at(gridXy), gridXy));
            } else if (instantBlocks.find(gridXy) != instantBlocks.end()) {
                m_centralWidget->addAction(new ChangeLevelBlockAction(m_model, type, instantBlocks.at(gridXy), gridXy));
            }
        }
        if (shouldGlob) {
            m_centralWidget->stopActionGlob();
        }
    }

    void MainViewMouseManager::addBlock(const GridXy& gridXy, INSTANT_BLOCK_TYPE type) {
        const auto& level         = m_model->level();
        const auto& dynamicBlocks = level.dynamicBlocks();
        const auto& instantBlocks = level.instantBlocks();
        bool        shouldGlob    = level.floorBlocks().find(gridXy) == level.floorBlocks().end();
        if (shouldGlob) {
            m_centralWidget->startActionGlob();
            addBlock(gridXy, FLOOR_BLOCK_TYPE::LEVEL);
        }
        if (dynamicBlocks.find(gridXy) == dynamicBlocks.end() && instantBlocks.find(gridXy) == instantBlocks.end()) {
            m_centralWidget->addAction(new AddLevelBlockAction(m_model, type, gridXy));
        } else {
            if (dynamicBlocks.find(gridXy) != dynamicBlocks.end()) {
                m_centralWidget->addAction(new ChangeLevelBlockAction(m_model, type, dynamicBlocks.at(gridXy), gridXy));
            } else if (instantBlocks.find(gridXy) != instantBlocks.end() && instantBlocks.at(gridXy) != type) {
                m_centralWidget->addAction(new ChangeLevelBlockAction(m_model, type, instantBlocks.at(gridXy), gridXy));
            }
        }
        if (shouldGlob) {
            m_centralWidget->stopActionGlob();
        }
    }

    void MainViewMouseManager::removeBlock(const GridXy& gridXy, [[maybe_unused]] FLOOR_BLOCK_TYPE type) {
        removeTopBlockFromLevel(gridXy);
    }

    void MainViewMouseManager::removeBlock(const GridXy& gridXy, [[maybe_unused]] DYNAMIC_BLOCK_TYPE type) {
        removeTopBlockFromLevel(gridXy);
    }

    void MainViewMouseManager::removeBlock(const GridXy& gridXy, [[maybe_unused]] INSTANT_BLOCK_TYPE type) {
        removeTopBlockFromLevel(gridXy);
    }

    void MainViewMouseManager::mouseLeftDragEvent(const GridXy& gridXy, FLOOR_BLOCK_TYPE type) {
        addBlock(gridXy, type);
    }

    void MainViewMouseManager::mouseLeftDragEvent([[maybe_unused]] const GridXy& gridXy, [[maybe_unused]] DYNAMIC_BLOCK_TYPE type) {
    }

    void MainViewMouseManager::mouseLeftDragEvent([[maybe_unused]] const GridXy& gridXy, [[maybe_unused]] INSTANT_BLOCK_TYPE type) {
    }

    void MainViewMouseManager::removeTopBlockFromLevel(const GridXy& gridXy) {
        const auto& level         = m_model->level();
        const auto& dynamicBlocks = level.dynamicBlocks();
        const auto& instantBlocks = level.instantBlocks();
        const auto& floorBlocks   = level.floorBlocks();
        if (dynamicBlocks.find(gridXy) != dynamicBlocks.end()) {
            m_centralWidget->addAction(new RemoveLevelBlockAction(m_model, dynamicBlocks.at(gridXy), gridXy));
        } else if (instantBlocks.find(gridXy) != instantBlocks.end()) {
            m_centralWidget->addAction(new RemoveLevelBlockAction(m_model, instantBlocks.at(gridXy), gridXy));
        } else if (floorBlocks.find(gridXy) != floorBlocks.end()) {
            if (floorBlocks.at(gridXy) != FLOOR_BLOCK_TYPE::LEVEL) {
                m_centralWidget->addAction(new ChangeFloorBlockAction(m_model, FLOOR_BLOCK_TYPE::LEVEL, floorBlocks.at(gridXy), gridXy));
            } else if (m_model->noLiveOrStoppedClusterOnBlock(gridXy)) {
                m_centralWidget->addAction(new RemoveFloorBlockAction(m_model, FLOOR_BLOCK_TYPE::LEVEL, gridXy));
            }
        }
    }

    void MainViewMouseManager::setBlockEditing(bool blockEditing) {
        m_blockEditing = blockEditing;
    }

} // namespace contr