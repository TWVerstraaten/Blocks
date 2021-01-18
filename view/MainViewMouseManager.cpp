//
// Created by teunv on 1/18/2021.
//

#include "MainViewMouseManager.h"

#include "CentralWidget.h"
#include "MainView.h"

#include <QApplication>
#include <action/DeleteClusterAction.h>
#include <action/MergeClusterAction.h>
#include <action/RemoveBlockAction.h>
#include <action/SplitDisconnectedAction.h>

namespace view {

    MainViewMouseManager::MainViewMouseManager(MainView* mainView) : m_mainView(mainView) {
    }

    void MainViewMouseManager::mousePressEvent(QMouseEvent* event) {
        m_previousMousePosition = event->pos();
        m_previousGridPosition  = model::GridXY::fromScreenXY(m_previousMousePosition, m_mainView->m_viewPort);
        if (event->button() == Qt::MouseButton::LeftButton) {
            mouseLeftPressEvent();
        }
    }

    void MainViewMouseManager::mouseMoveEvent(QMouseEvent* event) {
        const view::ScreenXY currentMousePosition = event->pos();
        const model::GridXY  currentGridPosition  = model::GridXY::fromScreenXY(currentMousePosition, m_mainView->m_viewPort);
        switch (event->buttons()) {
            case Qt::RightButton:
                m_mainView->m_viewPort.translate((event->x() - m_previousMousePosition.x()), event->y() - m_previousMousePosition.y());
                m_previousMousePosition = event->pos();
                break;
            case Qt::NoButton:
            case Qt::LeftButton:
                if (m_previousGridPosition != currentGridPosition) {
                    std::visit([this, currentGridPosition](const auto& a) { mouseLeftDragEvent(currentGridPosition, a); },
                               m_mainView->m_centralWidget->blockSelectWidget()->selectedBlockType());
                }
            case Qt::MiddleButton:
            default:
                break;
        }
        m_previousMousePosition = currentMousePosition;
        m_previousGridPosition  = currentGridPosition;
    }

    void MainViewMouseManager::mouseLeftPressEvent() {
        const auto type = m_mainView->m_centralWidget->blockSelectWidget()->selectedBlockType();
        if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
            std::visit([this](const auto& a) { removeBlock(m_previousGridPosition, a); }, type);
        } else {
            std::visit([this](const auto& a) { addBlock(m_previousGridPosition, a); }, type);
        }
    }

    void MainViewMouseManager::mouseLeftDragEvent(const model::GridXY& currentGridXY, CLUSTER_BLOCK type) {
        if (not m_mainView->m_model->level().isFreeStartBlock(currentGridXY)) {
            return;
        }
        if (m_mainView->m_model->noLiveOrStoppedClusterOnBlock(m_previousGridPosition) || not currentGridXY.isAdjacent(m_previousGridPosition)) {
            m_previousGridPosition = currentGridXY;
            mouseLeftPressEvent();
        } else {
            auto baseIt      = m_mainView->m_model->clusterContaining(m_previousGridPosition);
            auto extensionIt = m_mainView->m_model->clusterContaining(currentGridXY);
            if (extensionIt != m_mainView->m_model->clusters().end() && baseIt->index() == extensionIt->index()) {
                return;
            }
            m_mainView->m_centralWidget->startActionGlob();
            addBlock(currentGridXY, CLUSTER_BLOCK::CLUSTER);
            extensionIt = m_mainView->m_model->clusterContaining(currentGridXY);
            if (baseIt->index() != extensionIt->index()) {
                assert(baseIt != m_mainView->m_model->clusters().end());
                assert(extensionIt != m_mainView->m_model->clusters().end());
                m_mainView->m_centralWidget->addAction(
                    new action::MergeClusterAction(m_mainView->m_model.get(), *baseIt, *extensionIt, m_mainView->m_commandScrollArea));
            }
            m_mainView->m_centralWidget->stopActionGlob();
        }
    }

    void MainViewMouseManager::removeBlock(const model::GridXY& gridXy, CLUSTER_BLOCK type) {
        auto it = m_mainView->m_model->clusterContaining(gridXy);
        if (it == m_mainView->m_model->clusters().end()) {
            return;
        }

        if (it->size() == 1) {
            m_mainView->m_centralWidget->addAction(new action::DeleteClusterAction(m_mainView->m_centralWidget, *it));
        } else {
            m_mainView->m_centralWidget->startActionGlob();
            m_mainView->m_centralWidget->addAction(new action::RemoveBlockAction(m_mainView->m_model.get(), it->index(), gridXy));
            if (not it->isConnected()) {
                m_mainView->m_centralWidget->addAction(
                    new action::SplitDisconnectedAction(m_mainView->m_model.get(), *it, m_mainView->m_commandScrollArea));
            }
            m_mainView->m_centralWidget->stopActionGlob();
        }
    }

    void MainViewMouseManager::addBlock(const model::GridXY& gridXy, CLUSTER_BLOCK type) {
        if (m_mainView->m_model->noLiveOrStoppedClusterOnBlock(gridXy) && m_mainView->m_model->level().isFreeStartBlock(gridXy)) {
            m_mainView->m_model->clusters().emplace_back(gridXy, "CL" + std::to_string(m_mainView->m_model->clusters().size()));
            m_mainView->m_commandScrollArea->add(m_mainView->m_model->clusters().back());
            m_mainView->m_centralWidget->addAction(new action::NewClusterAction(m_mainView->m_centralWidget, m_mainView->m_model->clusters().back()));
        }
    }

    void MainViewMouseManager::addBlock(const model::GridXY& gridXy, model::FLOOR_BLOCK_TYPE type) {
    }

    void MainViewMouseManager::addBlock(const model::GridXY& gridXy, model::DYNAMIC_BLOCK_TYPE type) {
    }

    void MainViewMouseManager::addBlock(const model::GridXY& gridXy, model::INSTANT_BLOCK_TYPE type) {
    }

    void MainViewMouseManager::removeBlock(const model::GridXY& gridXy, model::FLOOR_BLOCK_TYPE type) {
    }

    void MainViewMouseManager::removeBlock(const model::GridXY& gridXy, model::DYNAMIC_BLOCK_TYPE type) {
    }

    void MainViewMouseManager::removeBlock(const model::GridXY& gridXy, model::INSTANT_BLOCK_TYPE type) {
    }

    void MainViewMouseManager::mouseLeftDragEvent(const model::GridXY& currentGridXY, model::FLOOR_BLOCK_TYPE type) {
    }

    void MainViewMouseManager::mouseLeftDragEvent(const model::GridXY& currentGridXY, model::DYNAMIC_BLOCK_TYPE type) {
    }

    void MainViewMouseManager::mouseLeftDragEvent(const model::GridXY& currentGridXY, model::INSTANT_BLOCK_TYPE type) {
    }

} // namespace view
