//
// Created by teunv on 1/18/2021.
//

#include "MainViewMouseManager.h"

#include "CentralWidget.h"
#include "MainView.h"
#include "action/AddFloorBlockAction.h"
#include "action/AddLevelBlockAction.h"
#include "action/ChangeFloorBlockAction.h"
#include "action/ChangeLevelBlockAction.h"
#include "action/DeleteClusterAction.h"
#include "action/MergeClusterAction.h"
#include "action/RemoveBlockFromClusterAction.h"
#include "action/SplitDisconnectedAction.h"

#include <QApplication>
#include <action/RemoveFloorBlockAction.h>
#include <action/RemoveLevelBlockAction.h>

namespace view {

    MainViewMouseManager::MainViewMouseManager(MainView* mainView) : m_mainView(mainView) {
    }

    void MainViewMouseManager::mousePressEvent(QMouseEvent* event) {
        m_previousMousePosition = event->pos();
        m_previousGridPosition  = model::GridXy::fromScreenXy(m_previousMousePosition, m_mainView->m_viewPort);
        if (not m_blockEditing) {
            if (event->button() == Qt::MouseButton::LeftButton) {
                mouseLeftPressEvent();
            }
        }
    }

    void MainViewMouseManager::mouseMoveEvent(QMouseEvent* event) {
        const view::ScreenXy currentMousePosition = event->pos();
        const model::GridXy  currentGridPosition  = model::GridXy::fromScreenXy(currentMousePosition, m_mainView->m_viewPort);
        switch (event->buttons()) {
            case Qt::RightButton:
                m_mainView->m_viewPort.translate((event->x() - m_previousMousePosition.x()), event->y() - m_previousMousePosition.y());
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

    void MainViewMouseManager::mouseLeftDragEvent(const model::GridXy& currentGridXy, CLUSTER_BLOCK type) {
        if (not m_model->level().isFreeStartBlock(currentGridXy)) {
            return;
        }
        if (m_model->noLiveOrStoppedClusterOnBlock(m_previousGridPosition) || not currentGridXy.isAdjacent(m_previousGridPosition)) {
            m_previousGridPosition = currentGridXy;
            mouseLeftPressEvent();
        } else {
            auto baseIt      = m_model->clusterContaining(m_previousGridPosition);
            auto extensionIt = m_model->clusterContaining(currentGridXy);
            if (extensionIt != m_model->clusters().end() && baseIt->index() == extensionIt->index()) {
                return;
            }
            m_centralWidget->startActionGlob();
            addBlock(currentGridXy, type);
            extensionIt = m_model->clusterContaining(currentGridXy);
            if (baseIt->index() != extensionIt->index()) {
                assert(baseIt != m_model->clusters().end());
                assert(extensionIt != m_model->clusters().end());
                m_centralWidget->addAction(new action::MergeClusterAction(m_model, *baseIt, *extensionIt, m_mainView->m_commandScrollArea));
            }
            m_centralWidget->stopActionGlob();
        }
    }

    void MainViewMouseManager::removeBlock(const model::GridXy& gridXy, [[maybe_unused]] CLUSTER_BLOCK type) {
        auto it = m_model->clusterContaining(gridXy);
        if (it == m_model->clusters().end()) {
            return;
        }
        if (it->size() == 1) {
            m_centralWidget->addAction(new action::DeleteClusterAction(m_centralWidget, *it));
        } else {
            m_centralWidget->startActionGlob();
            m_centralWidget->addAction(new action::RemoveBlockFromClusterAction(m_model, it->index(), gridXy));
            if (not it->isConnected()) {
                m_centralWidget->addAction(new action::SplitDisconnectedAction(m_model, *it, m_mainView->m_commandScrollArea));
            }
            m_centralWidget->stopActionGlob();
        }
    }

    void MainViewMouseManager::addBlock(const model::GridXy& gridXy, [[maybe_unused]] CLUSTER_BLOCK type) {
        if (m_model->noLiveOrStoppedClusterOnBlock(gridXy) && m_model->level().isFreeStartBlock(gridXy)) {
            m_model->clusters().emplace_back(gridXy, "CL" + std::to_string(m_model->clusters().size()));
            m_mainView->m_commandScrollArea->add(m_model->clusters().back());
            m_centralWidget->addAction(new action::NewClusterAction(m_centralWidget, m_model->clusters().back()));
        }
    }

    void MainViewMouseManager::addBlock(const model::GridXy& gridXy, model::FLOOR_BLOCK_TYPE type) {
        if (m_model->level().floorBlocks().find(gridXy) == m_model->level().floorBlocks().end()) {
            m_centralWidget->addAction(new action::AddFloorBlockAction(m_model, type, gridXy));
        } else if (m_model->level().floorBlocks().at(gridXy) != type) {
            m_centralWidget->addAction(new action::ChangeFloorBlockAction(m_model, type, m_model->level().floorBlocks().at(gridXy), gridXy));
        }
    }

    void MainViewMouseManager::addBlock(const model::GridXy& gridXy, model::DYNAMIC_BLOCK_TYPE type) {
        const auto& level         = m_model->level();
        const auto& dynamicBlocks = level.dynamicBlocks();
        const auto& instantBlocks = level.instantBlocks();
        bool        shouldGlob    = level.floorBlocks().find(gridXy) == level.floorBlocks().end();
        if (shouldGlob) {
            m_centralWidget->startActionGlob();
            addBlock(gridXy, model::FLOOR_BLOCK_TYPE::LEVEL);
        }
        if (dynamicBlocks.find(gridXy) == dynamicBlocks.end() && instantBlocks.find(gridXy) == instantBlocks.end()) {
            m_centralWidget->addAction(new action::AddLevelBlockAction(m_model, type, gridXy));
        } else {
            if (dynamicBlocks.find(gridXy) != dynamicBlocks.end() && dynamicBlocks.at(gridXy) != type) {
                m_centralWidget->addAction(new action::ChangeLevelBlockAction(m_model, type, dynamicBlocks.at(gridXy), gridXy));
            } else if (instantBlocks.find(gridXy) != instantBlocks.end()) {
                m_centralWidget->addAction(new action::ChangeLevelBlockAction(m_model, type, instantBlocks.at(gridXy), gridXy));
            }
        }
        if (shouldGlob) {
            m_centralWidget->stopActionGlob();
        }
    }

    void MainViewMouseManager::addBlock(const model::GridXy& gridXy, model::INSTANT_BLOCK_TYPE type) {
        const auto& level         = m_model->level();
        const auto& dynamicBlocks = level.dynamicBlocks();
        const auto& instantBlocks = level.instantBlocks();
        bool        shouldGlob    = level.floorBlocks().find(gridXy) == level.floorBlocks().end();
        if (shouldGlob) {
            m_centralWidget->startActionGlob();
            addBlock(gridXy, model::FLOOR_BLOCK_TYPE::LEVEL);
        }
        if (dynamicBlocks.find(gridXy) == dynamicBlocks.end() && instantBlocks.find(gridXy) == instantBlocks.end()) {
            m_centralWidget->addAction(new action::AddLevelBlockAction(m_model, type, gridXy));
        } else {
            if (dynamicBlocks.find(gridXy) != dynamicBlocks.end()) {
                m_centralWidget->addAction(new action::ChangeLevelBlockAction(m_model, type, dynamicBlocks.at(gridXy), gridXy));
            } else if (instantBlocks.find(gridXy) != instantBlocks.end() && instantBlocks.at(gridXy) != type) {
                m_centralWidget->addAction(new action::ChangeLevelBlockAction(m_model, type, instantBlocks.at(gridXy), gridXy));
            }
        }
        if (shouldGlob) {
            m_centralWidget->stopActionGlob();
        }
    }

    void MainViewMouseManager::removeBlock(const model::GridXy& gridXy, [[maybe_unused]] model::FLOOR_BLOCK_TYPE type) {
        removeTopBlockFromLevel(gridXy);
    }

    void MainViewMouseManager::removeBlock(const model::GridXy& gridXy, [[maybe_unused]] model::DYNAMIC_BLOCK_TYPE type) {
        removeTopBlockFromLevel(gridXy);
    }

    void MainViewMouseManager::removeBlock(const model::GridXy& gridXy, [[maybe_unused]] model::INSTANT_BLOCK_TYPE type) {
        removeTopBlockFromLevel(gridXy);
    }

    void MainViewMouseManager::mouseLeftDragEvent(const model::GridXy& gridXy, model::FLOOR_BLOCK_TYPE type) {
        addBlock(gridXy, type);
    }

    void MainViewMouseManager::mouseLeftDragEvent([[maybe_unused]] const model::GridXy& gridXy, [[maybe_unused]] model::DYNAMIC_BLOCK_TYPE type) {
    }

    void MainViewMouseManager::mouseLeftDragEvent([[maybe_unused]] const model::GridXy& gridXy, [[maybe_unused]] model::INSTANT_BLOCK_TYPE type) {
    }

    void MainViewMouseManager::removeTopBlockFromLevel(const model::GridXy& gridXy) {
        const auto& level         = m_model->level();
        const auto& dynamicBlocks = level.dynamicBlocks();
        const auto& instantBlocks = level.instantBlocks();
        const auto& floorBlocks   = level.floorBlocks();
        if (dynamicBlocks.find(gridXy) != dynamicBlocks.end()) {
            m_centralWidget->addAction(new action::RemoveLevelBlockAction(m_model, dynamicBlocks.at(gridXy), gridXy));
        } else if (instantBlocks.find(gridXy) != instantBlocks.end()) {
            m_centralWidget->addAction(new action::RemoveLevelBlockAction(m_model, instantBlocks.at(gridXy), gridXy));
        } else if (floorBlocks.find(gridXy) != floorBlocks.end()) {
            if (floorBlocks.at(gridXy) != model::FLOOR_BLOCK_TYPE::LEVEL) {
                m_centralWidget->addAction(
                    new action::ChangeFloorBlockAction(m_model, model::FLOOR_BLOCK_TYPE::LEVEL, floorBlocks.at(gridXy), gridXy));
            } else if (m_model->noLiveOrStoppedClusterOnBlock(gridXy)) {
                m_centralWidget->addAction(new action::RemoveFloorBlockAction(m_model, model::FLOOR_BLOCK_TYPE::LEVEL, gridXy));
            }
        }
    }

    void MainViewMouseManager::setBlockEditing(bool blockEditing) {
        m_blockEditing = blockEditing;
    }

} // namespace view