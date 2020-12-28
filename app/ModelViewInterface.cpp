//
// Created by pc on 28-12-20.
//

#include "ModelViewInterface.h"

#include "../action/GenericCommandEditBoxAction.h"
#include "../action/GenericModelAction.h"
#include "../action/RemoveBlockFromClusterAction.h"
#include "../action/RemoveClusterAction.h"
#include "../view/View.h"
#include "../view/widget/ScrollArea.h"
#include "Application_Edit.h"
#include "Application_enums.h"

#include <algorithm>
#include <cassert>

static std::list<model::Cluster>::iterator findCluster(const view::widget::CommandEditBox& commandEditBox,
                                                       std::list<model::Cluster>&          clusters) {
    return std::find_if(clusters.begin(), clusters.end(), [&](const model::Cluster& cluster) {
        return cluster.index() == commandEditBox.index();
    });
}

static std::list<model::Cluster>::const_iterator findCluster(const view::widget::CommandEditBox& commandEditBox,
                                                             const std::list<model::Cluster>&    clusters) {
    return std::find_if(clusters.begin(), clusters.end(), [&](const model::Cluster& cluster) {
        return cluster.index() == commandEditBox.index();
    });
}

static std::list<view::widget::CommandEditBox>::iterator findCommandEditBox(
    const model::Cluster& cluster, std::list<view::widget::CommandEditBox>& commandEditBoxes) {
    return std::find_if(
        commandEditBoxes.begin(), commandEditBoxes.end(), [&](const view::widget::CommandEditBox& commandEditBox) {
            return cluster.index() == commandEditBox.index();
        });
}

static void removeActionBoxesOfRemovedClusters(const std::list<model::Cluster>& clusters,
                                               view::widget::ScrollArea&        scrollArea) {
    scrollArea.children().remove_if(
        [&](const view::widget::CommandEditBox& box) { return findCluster(box, clusters) == clusters.end(); });
}

static void highlightActionBoxLines(const std::list<model::Cluster>& clusters, view::widget::ScrollArea& scrollArea) {
    for (auto& commandEditBox : scrollArea.children()) {
        auto it = findCluster(commandEditBox, clusters);
        assert(it != clusters.end());
        if (it->isAlive()) {
            commandEditBox.setHighLightedLine(it->commandIndex());
        }
        commandEditBox.setActive(it->isAlive());
    }
}

static void addActionBoxesOfNewClusters(const std::list<model::Cluster>& clusters,
                                        view::widget::ScrollArea&        scrollArea) {
    auto it = std::find_if(clusters.begin(), clusters.end(), [&](const model::Cluster& cluster) {
        return findCommandEditBox(cluster, scrollArea.children()) == scrollArea.children().end();
    });
    while (it != clusters.end()) {
        scrollArea.addCommandEditBox(*it);
        it = std::find_if(clusters.begin(), clusters.end(), [&](const model::Cluster& cluster) {
            return findCommandEditBox(cluster, scrollArea.children()) == scrollArea.children().end();
        });
    }
}

void app::ModelViewInterface::leftMouseDrag(model::Model&             model,
                                            view::View&               view,
                                            view::widget::ScrollArea& scrollArea,
                                            const model::GridXY&      point,
                                            const model::GridXY&      previousPoint) {
    if (not model.level().isFreeStartBlock(point)) {
        return;
    }
    if (model.level().isFreeStartBlock(previousPoint)) {
        addAction(model.linkBlocks(previousPoint, point));
    } else {
        addAction(model.addCluster(point));
    }
    updateCommandScrollArea(model.clusters(), scrollArea, APP_MODE::EDITING);
}

void app::ModelViewInterface::clearBlock(model::Model&             model,
                                         view::View&               view,
                                         view::widget::ScrollArea& scrollArea,
                                         const model::GridXY&      point) {
    if (model.level().isFreeStartBlock(point)) {
        addAction(clearBlockStatic(model, scrollArea, point));
    }
    removeActionBoxesOfRemovedClusters(model.clusters(), scrollArea);
    scrollArea.setHeightAndPositions();
}

void app::ModelViewInterface::leftMouseClick(model::Model&             model,
                                             view::View&               view,
                                             view::widget::ScrollArea& scrollArea,
                                             const model::GridXY&      point) {
    if (not model.level().isFreeStartBlock(point)) {
        return;
    }
    addAction(model.addCluster(point));
    updateCommandScrollArea(model.clusters(), scrollArea, APP_MODE::EDITING);
}

void app::ModelViewInterface::addAction(std::unique_ptr<action::Action> action) {
    if (action) {
        m_undoStack.push(std::move(action));
        m_redoStack = {};
    }
}

void app::ModelViewInterface::updateCommandScrollArea(const std::list<model::Cluster>& clusters,
                                                      view::widget::ScrollArea&        scrollArea,
                                                      app::APP_MODE                    mode) {
    removeActionBoxesOfRemovedClusters(clusters, scrollArea);
    if (mode == APP_MODE::RUNNING) {
        highlightActionBoxLines(clusters, scrollArea);
    }
    addActionBoxesOfNewClusters(clusters, scrollArea);
    scrollArea.setHeightAndPositions();
}

void app::ModelViewInterface::undo(app::Application_Edit& applicationEdit) {
    if (m_undoStack.empty()) {
        return;
    }
    m_undoStack.top()->undoAction(applicationEdit);
    m_redoStack.push(std::move(m_undoStack.top()));
    assert(m_undoStack.top() == nullptr);
    m_undoStack.pop();
    updateCommandScrollArea(applicationEdit.model()->clusters(), *applicationEdit.scrollArea(), APP_MODE::EDITING);
}

void app::ModelViewInterface::redo(app::Application_Edit& applicationEdit) {
    if (m_redoStack.empty()) {
        return;
    }
    m_redoStack.top()->redoAction(applicationEdit);
    m_undoStack.push(std::move(m_redoStack.top()));
    assert(m_redoStack.top() == nullptr);
    m_redoStack.pop();
    updateCommandScrollArea(applicationEdit.model()->clusters(), *applicationEdit.scrollArea(), APP_MODE::EDITING);
}

std::unique_ptr<action::Action> app::ModelViewInterface::clearBlockStatic(model::Model&             model,
                                                                          view::widget::ScrollArea& scrollArea,
                                                                          const model::GridXY&      point) {
    auto&      clusters = model.clusters();
    const auto it =
        std::find_if(clusters.begin(), clusters.end(), [&](const auto& cluster) { return cluster.contains(point); });
    if (it == clusters.end()) {
        return nullptr;
    }
    if (it->size() == 1) {
        auto result = std::unique_ptr<action::Action>(new action::RemoveClusterAction(*it));
        clusters.erase(it);
        return result;
    }
    model::Model copy = model;
    it->removeBLock(point);
    if (not it->isConnected()) {
        auto newClusters   = it->collectAllButFirstComponent();
        auto commandEditIt = findCommandEditBox(*it, scrollArea.children());
        assert(commandEditIt != scrollArea.children().end());
        addActionBoxesOfNewClusters(newClusters, scrollArea);

        for (const auto& cluster : newClusters) {
            auto newCommandEditIt = findCommandEditBox(cluster, scrollArea.children());
            assert(newCommandEditIt != scrollArea.children().end());
            newCommandEditIt->setStrings(commandEditIt->strings());
        }

        clusters.splice(clusters.end(), newClusters);
        assert(it->isConnected());
        return std::unique_ptr<action::Action>(new action::GenericModelAction(copy, model));
    }
    return std::unique_ptr<action::Action>(new action::RemoveBlockFromClusterAction(it->index(), point));
}

void app::ModelViewInterface::handleKeyEvent(const SDL_Event&          event,
                                             view::widget::ScrollArea& scrollArea,
                                             model::Model&             model) {

    const auto w = scrollArea.focusedWidget();
    if (w) {
        view::widget::CommandEditBox copy(*w);
        w->keyEvent(event);
        auto clusterIt = findCluster(*w, model.clusters());
        w->updateClusterCommands(*clusterIt);
        if (w->strings() != copy.strings()) {
            addAction(std::unique_ptr<action::Action>(new action::GenericCommandEditBoxAction(copy, *w)));
        }
    }
}
