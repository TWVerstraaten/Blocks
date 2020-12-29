//
// Created by pc on 28-12-20.
//

#include "ModelViewInterface.h"

#include "../action/GenericCommandEditBoxAction.h"
#include "../action/GenericModelAction.h"
#include "../action/RemoveBlockFromClusterAction.h"
#include "../action/RemoveClusterAction.h"
#include "../global/overloaded.h"
#include "../view/View.h"
#include "../view/widget/ScrollArea.h"
#include "Application_Edit.h"

#include <algorithm>
#include <cassert>

static std::list<model::Cluster>::iterator findCluster(const view::widget::CommandEditBox& commandEditBox,
                                                       std::list<model::Cluster>&          clusters) {
    return std::find_if(
        clusters.begin(), clusters.end(), [&](const model::Cluster& cluster) { return cluster.index() == commandEditBox.index(); });
}

static std::list<model::Cluster>::const_iterator findCluster(const view::widget::CommandEditBox& commandEditBox,
                                                             const std::list<model::Cluster>&    clusters) {
    return std::find_if(
        clusters.begin(), clusters.end(), [&](const model::Cluster& cluster) { return cluster.index() == commandEditBox.index(); });
}

static std::list<view::widget::CommandEditBox>::iterator findCommandEditBox(const model::Cluster&                    cluster,
                                                                            std::list<view::widget::CommandEditBox>& commandEditBoxes) {
    return std::find_if(commandEditBoxes.begin(), commandEditBoxes.end(), [&](const view::widget::CommandEditBox& commandEditBox) {
        return cluster.index() == commandEditBox.index();
    });
}

static void removeActionBoxesOfRemovedClusters(const std::list<model::Cluster>& clusters, view::widget::ScrollArea& scrollArea) {
    scrollArea.children().remove_if([&](const view::widget::CommandEditBox& box) { return findCluster(box, clusters) == clusters.end(); });
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

static void addActionBoxesOfNewClusters(const std::list<model::Cluster>& clusters, view::widget::ScrollArea& scrollArea) {
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
                                            const model::GridXY&      previousPoint,
                                            const model::BlockType&   selectedBlockType) {
    std::visit(overloaded{[&](const model::CLUSTER_TYPE type) { clusterDrag(model, view, scrollArea, point, previousPoint); },
                          [&](const auto type) { addAction(model.level().addBlock(point, type)); }},
               selectedBlockType);
}

void app::ModelViewInterface::leftClickControl(model::Model&             model,
                                               view::View&               view,
                                               view::widget::ScrollArea& scrollArea,
                                               const model::GridXY&      point,
                                               const model::BlockType&   selectedBlockType) {
    std::visit(overloaded{[&](const model::CLUSTER_TYPE type) { clearBlock(model, view, scrollArea, point); },
                          [&](const auto type) {
                              if (model.noClusterOnBlock(point)) {
                                  addAction(model.level().removeBlock(point));
                              } else {
                                  clearBlock(model, view, scrollArea, point);
                              }
                          }},
               selectedBlockType);
}

void app::ModelViewInterface::leftMouseClick(model::Model&             model,
                                             view::View&               view,
                                             view::widget::ScrollArea& scrollArea,
                                             const model::GridXY&      point,
                                             const model::BlockType&   selectedBlockType) {
    std::visit(overloaded{[&](const model::CLUSTER_TYPE type) { leftMouseClick(model, view, scrollArea, point); },
                          [&](const auto type) { addAction(model.level().addBlock(point, type)); }},
               selectedBlockType);
}

void app::ModelViewInterface::addAction(Action_u_ptr&& action) {
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
    const auto it       = std::find_if(clusters.begin(), clusters.end(), [&](const auto& cluster) { return cluster.contains(point); });
    if (it == clusters.end()) {
        return nullptr;
    }
    if (it->size() == 1) {
        auto commandEditBoxIt = findCommandEditBox(*it, scrollArea.children());
        auto result           = std::make_unique<action::RemoveClusterAction>(*it, *commandEditBoxIt);
        clusters.erase(it);
        return result;
    }
    return clearBlockFromCluster(model, scrollArea, point, it);
}

void app::ModelViewInterface::handleKeyEvent(const SDL_Event& event, view::widget::ScrollArea& scrollArea, model::Model& model) {
    const auto w = scrollArea.focusedWidget();
    if (w) {
        view::widget::CommandEditBox copy(*w);
        w->keyEvent(event);
        if (w->clusterShouldBeUpdated()) {
            auto clusterIt = findCluster(*w, model.clusters());
            w->updateClusterCommands(*clusterIt);
            addAction(std::make_unique<action::GenericCommandEditBoxAction>(copy, *w));
        }
    }
}

void app::ModelViewInterface::interactWithInstantBlocks(model::Model& model, view::widget::ScrollArea& scrollArea) {
    for (const auto& block : model.level().instantBlocks()) {
        switch (block.second) {
            case model::INSTANT_BLOCK_TYPE::NONE:
                break;
            case model::INSTANT_BLOCK_TYPE::KILL:
                clearBlockStatic(model, scrollArea, block.first);
                break;
        }
    }
}

void app::ModelViewInterface::interactWithDynamicBlocks(model::Model& model, view::widget::ScrollArea& scrollArea) {
    for (auto& cluster : model.clusters()) {
        if (not cluster.isAlive()) {
            continue;
        }
        for (auto it = cluster.gridXY().begin(); it != cluster.gridXY().end(); ++it) {
            cluster.addPendingOperation(*it, model.level().dynamicBlockAt(*it));
        }
        cluster.performPendingOperationOrNextCommand();
    }
}

std::unique_ptr<action::Action> app::ModelViewInterface::clearBlockFromCluster(model::Model&                       model,
                                                                               view::widget::ScrollArea&           scrollArea,
                                                                               const model::GridXY&                point,
                                                                               std::list<model::Cluster>::iterator clusterIt) {
    assert(clusterIt->contains(point));

    model::Model copy = model;
    clusterIt->removeBLock(point);
    if (not clusterIt->isConnected()) {
        auto newClusters   = clusterIt->collectAllButFirstComponent();
        auto commandEditIt = findCommandEditBox(*clusterIt, scrollArea.children());
        assert(commandEditIt != scrollArea.children().end());
        addActionBoxesOfNewClusters(newClusters, scrollArea);
        for (const auto& cluster : newClusters) {
            auto newCommandEditIt = findCommandEditBox(cluster, scrollArea.children());
            assert(newCommandEditIt != scrollArea.children().end());
            newCommandEditIt->setStrings(commandEditIt->strings());
        }
        assert(clusterIt->isConnected());
        model.clusters().splice(model.clusters().end(), newClusters);
        return std::make_unique<action::GenericModelAction>(copy, model);
    }
    return std::make_unique<action::RemoveBlockFromClusterAction>(clusterIt->index(), point);
}

std::unique_ptr<action::Action> app::ModelViewInterface::addCluster(model::Model&             model,
                                                                    view::widget::ScrollArea& scrollArea,
                                                                    const model::GridXY&      point) {
    assert(model.level().isFreeStartBlock(point));
    auto& clusters = model.clusters();
    if (std::find_if(clusters.begin(), clusters.end(), [&](const auto& cluster) { return cluster.contains(point); }) == clusters.end()) {
        clusters.push_back(model::Cluster({point}, "CL" + std::to_string(clusters.size())));
        addActionBoxesOfNewClusters(clusters, scrollArea);
        auto commandEditBoxIt = findCommandEditBox(clusters.back(), scrollArea.children());
        assert(commandEditBoxIt != scrollArea.children().end());
        return std::make_unique<action::AddClusterAction>(clusters.back(), *commandEditBoxIt);
    }
    return nullptr;
}

std::unique_ptr<action::Action> app::ModelViewInterface::linkBlocks(model::Model&             model,
                                                                    view::widget::ScrollArea& scrollArea,
                                                                    const model::GridXY&      point,
                                                                    const model::GridXY&      previousPoint) {

    auto&      clusters = model.clusters();
    const auto baseIt =
        std::find_if(clusters.begin(), clusters.end(), [&](const auto& cluster) { return cluster.contains(previousPoint); });
    if (baseIt == clusters.end() || (not previousPoint.isAdjacent(point))) {
        return addCluster(model, scrollArea, point);
    }
    auto extensionIt = std::find_if(clusters.begin(), clusters.end(), [&](const auto& cluster) { return cluster.contains(point); });
    if (baseIt == extensionIt) {
        return nullptr;
    }
    if (extensionIt == clusters.end()) {
        baseIt->addGridXY(point);
        assert(baseIt->isConnected());
        return std::make_unique<action::AddBlockToClusterAction>(baseIt->index(), point);
    }
    auto copyOfModel = model;
    baseIt->gridXY().insert(extensionIt->gridXY().begin(), extensionIt->gridXY().end());
    clusters.erase(extensionIt);
    return std::make_unique<action::GenericModelAction>(copyOfModel, model);
}

void app::ModelViewInterface::leftMouseClick(model::Model&             model,
                                             view::View&               view,
                                             view::widget::ScrollArea& scrollArea,
                                             const model::GridXY&      point) {
    if (not model.level().isFreeStartBlock(point)) {
        return;
    }
    addAction(addCluster(model, scrollArea, point));
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

void app::ModelViewInterface::clusterDrag(model::Model&             model,
                                          view::View&               view,
                                          view::widget::ScrollArea& scrollArea,
                                          const model::GridXY&      point,
                                          const model::GridXY&      previousPoint) {
    if (not model.level().isFreeStartBlock(point)) {
        return;
    }
    if (model.level().isFreeStartBlock(previousPoint)) {
        addAction(linkBlocks(model, scrollArea, point, previousPoint));
    } else {
        addAction(addCluster(model, scrollArea, point));
    }
    updateCommandScrollArea(model.clusters(), scrollArea, APP_MODE::EDITING);
}
