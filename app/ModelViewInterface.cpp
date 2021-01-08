//
// Created by pc on 28-12-20.
//

#include "ModelViewInterface.h"

#include "../action/GenericCommandEditBoxAction.h"
#include "../action/GenericModelAction.h"
#include "../action/RemoveBlockFromClusterAction.h"
#include "../action/RemoveClusterAction.h"
#include "../global/defines.h"
#include "../global/overloaded.h"
#include "../view/View.h"
#include "../view/widget/ScrollArea.h"
#include "Application_Edit.h"

#include <cassert>

using namespace model;
using namespace view;
using namespace widget;

static std::list<Cluster>::iterator findCluster(const CommandEditBox& commandEditBox, std::list<Cluster>& clusters) {
    return std::find_if(_IT_(clusters), _FUNC_(cluster, cluster.index() == commandEditBox.index()));
}

static std::list<Cluster>::const_iterator findCluster(const CommandEditBox& commandEditBox, const std::list<Cluster>& clusters) {
    return std::find_if(_CIT_(clusters), _FUNC_(cluster, cluster.index() == commandEditBox.index()));
}

static std::list<CommandEditBox>::iterator findCommandEditBox(const Cluster& cluster, std::list<CommandEditBox>& commandEditBoxes) {
    return std::find_if(_IT_(commandEditBoxes), _FUNC_(box, cluster.index() == box.index()));
}

static void removeActionBoxesOfRemovedClusters(const std::list<Cluster>& clusters, ScrollArea& scrollArea) {
    scrollArea.children().remove_if(_FUNC_(box, findCluster(box, clusters) == clusters.end()));
}

static void updateExistingCommandEditBoxes(const std::list<Cluster>& clusters, ScrollArea& scrollArea) {
    for (auto& commandEditBox : scrollArea.children()) {
        auto it = findCluster(commandEditBox, clusters);
        assert(it != clusters.end());
        if (it->alive()) {
            commandEditBox.update(it->commandVector());
        }
        commandEditBox.setActive(it->alive());
    }
}

static void addCommandEditBoxesOfNewClusters(const std::list<Cluster>& clusters, ScrollArea& scrollArea) {
    auto it = std::find_if(_IT_(clusters), _FUNC_(cluster, findCommandEditBox(cluster, scrollArea.children()) == scrollArea.children().end()));
    while (it != clusters.end()) {
        scrollArea.addCommandEditBox(*it);
        it = std::find_if(_IT_(clusters), _FUNC_(cluster, findCommandEditBox(cluster, scrollArea.children()) == scrollArea.children().end()));
    }
}

void app::ModelViewInterface::leftMouseDrag(
    Model& model, View& view, ScrollArea& scrollArea, const GridXY& point, const GridXY& previousPoint, const BlockType& selectedBlockType) {
    std::visit(overloaded{[&](const CLUSTER_TYPE type) { clusterDrag(model, view, scrollArea, point, previousPoint); },
                          [&](const auto type) { addAction(model.level().addBlock(point, type)); }},
               selectedBlockType);
}

void app::ModelViewInterface::leftClickControl(
    Model& model, View& view, ScrollArea& scrollArea, const GridXY& point, const BlockType& selectedBlockType) {
    std::visit(overloaded{[&](const CLUSTER_TYPE type) { clearBlockFromCluster(model, view, scrollArea, point); },
                          [&](const auto type) {
                              if (model.noClusterOnBlock(point)) {
                                  addAction(model.level().removeBlock(point));
                              } else {
                                  clearBlockFromCluster(model, view, scrollArea, point);
                              }
                          }},
               selectedBlockType);
}

void app::ModelViewInterface::leftMouseClick(
    Model& model, View& view, ScrollArea& scrollArea, const GridXY& point, const BlockType& selectedBlockType) {
    std::visit(overloaded{[&](const CLUSTER_TYPE type) { leftMouseClick(model, view, scrollArea, point); },
                          [&](const auto type) { addAction(model.level().addBlock(point, type)); }},
               selectedBlockType);
}

void app::ModelViewInterface::addAction(Action_u_ptr&& action) {
    if (action) {
        m_undoStack.push(std::move(action));
        m_redoStack = {};
    }
}

void app::ModelViewInterface::updateCommandScrollArea(Model& model, ScrollArea& scrollArea, app::APP_MODE mode) {
    removeActionBoxesOfRemovedClusters(model.clusters(), scrollArea);
    if (mode == APP_MODE::RUNNING) {
        updateExistingCommandEditBoxes(model.clusters(), scrollArea);
    }
    addCommandEditBoxesOfNewClusters(model.clusters(), scrollArea);
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
    updateCommandScrollArea(*applicationEdit.model(), *applicationEdit.scrollArea(), APP_MODE::EDITING);
}

void app::ModelViewInterface::redo(app::Application_Edit& applicationEdit) {
    if (m_redoStack.empty()) {
        return;
    }
    m_redoStack.top()->redoAction(applicationEdit);
    m_undoStack.push(std::move(m_redoStack.top()));
    assert(m_redoStack.top() == nullptr);
    m_redoStack.pop();
    updateCommandScrollArea(*applicationEdit.model(), *applicationEdit.scrollArea(), APP_MODE::EDITING);
}

std::unique_ptr<action::Action> app::ModelViewInterface::clearBlockFromCluster_static(Model& model, ScrollArea& scrollArea, const GridXY& point) {
    auto&      clusters = model.clusters();
    const auto it       = model.clusterContaining(point);
    if (it == clusters.end()) {
        return nullptr;
    }
    if (it->size() > 1) {
        return clearBlockFromCluster(model, scrollArea, point, *it);
    }
    auto result = std::make_unique<action::RemoveClusterAction>(*it);
    clusters.erase(it);
    return result;
}

void app::ModelViewInterface::handleKeyEvent(const SDL_Event& event, ScrollArea& scrollArea, Model& model) {
    const auto w = scrollArea.focusedWidget();
    if (w) {
        CommandEditBox copy(*w);
        w->keyEvent(event);
        if (w->clusterShouldBeUpdated()) {
            auto clusterIt = findCluster(*w, model.clusters());
            w->updateClusterCommands(*clusterIt);
        }
        addAction(std::make_unique<action::GenericCommandEditBoxAction>(copy, *w));
    }
}

void app::ModelViewInterface::interactWithInstantBlocks(Model& model, ScrollArea& scrollArea) {
    for (const auto& [point, type] : model.level().instantBlocks()) {
        switch (type) {
            case INSTANT_BLOCK_TYPE::KILL:
                clearBlockFromCluster_static(model, scrollArea, point);
                break;
        }
    }
}

void app::ModelViewInterface::stopSpliceOrKillIfNeeded(Level& level, Cluster& cluster) {
    if (not cluster.alive()) {
        return;
    }
    std::vector<std::pair<const GridXY, DYNAMIC_BLOCK_TYPE>> pendingOperations;
    for (const auto& [point, type] : level.dynamicBlocks()) {
        if (cluster.contains(point)) {
            pendingOperations.emplace_back(point, type);
        }
    }
    if (pendingOperations.size() > 1 && cluster.currentModifier() != COMMAND_MODIFIER::IGNORE) {
        cluster.kill();
    } else {
        if (pendingOperations.size() == 1 && cluster.currentModifier() != COMMAND_MODIFIER::IGNORE) {
            return;
        }
        if (cluster.commandVector().currentType() == COMMAND_TYPE::STP) {
            cluster.setState(CLUSTER_STATE::STOPPED);
        }
    }
}

bool app::ModelViewInterface::interactWithDynamicBlocks(Level& level, Cluster& cluster) {
    if (not cluster.alive() || cluster.currentModifier() == COMMAND_MODIFIER::IGNORE) {
        return false;
    }
    std::vector<std::pair<const GridXY, DYNAMIC_BLOCK_TYPE>> pendingOperations;
    for (const auto& [point, type] : level.dynamicBlocks()) {
        if (cluster.contains(point)) {
            pendingOperations.emplace_back(point, type);
        }
    }
    if (pendingOperations.empty()) {
        return false;
    }
    assert(pendingOperations.size() == 1);
    cluster.doOperation(pendingOperations.front().first, pendingOperations.front().second);
    return true;
}

std::unique_ptr<action::Action> app::ModelViewInterface::clearBlockFromCluster(Model&        model,
                                                                               ScrollArea&   scrollArea,
                                                                               const GridXY& point,
                                                                               Cluster&      cluster) {
    assert(cluster.contains(point));
    cluster.removeBLock(point);
    if (cluster.connected()) {
        return std::make_unique<action::RemoveBlockFromClusterAction>(cluster.index(), point);
    }
    Model copy = model;
    split(model, scrollArea, cluster);
    return std::make_unique<action::GenericModelAction>(copy, model);
}

void app::ModelViewInterface::splitIfDisconnected(Model& model, ScrollArea& scrollArea, Cluster& cluster) {
    if (not cluster.connected()) {
        split(model, scrollArea, cluster);
    }
}

void app::ModelViewInterface::split(Model& model, ScrollArea& scrollArea, Cluster& cluster) {
    auto newClusters   = cluster.collectAllButFirstComponent();
    auto commandEditIt = findCommandEditBox(cluster, scrollArea.children());
    assert(commandEditIt != scrollArea.children().end());
    addCommandEditBoxesOfNewClusters(newClusters, scrollArea);
    for (const auto& newCluster : newClusters) {
        auto newCommandEditIt = findCommandEditBox(newCluster, scrollArea.children());
        assert(newCommandEditIt != scrollArea.children().end());
        newCommandEditIt->setStrings(commandEditIt->strings());
    }
    assert(cluster.connected());
    model.clusters().splice(model.clusters().end(), newClusters);
}

std::unique_ptr<action::Action> app::ModelViewInterface::addSingleBlockCluster(Model& model, ScrollArea& scrollArea, const GridXY& point) {
    assert(model.level().isFreeStartBlock(point));
    auto& clusters = model.clusters();
    if (model.clusterContaining(point) != clusters.end()) {
        return nullptr;
    }
    clusters.push_back(Cluster({point}, "CL" + std::to_string(clusters.size())));
    addCommandEditBoxesOfNewClusters(clusters, scrollArea);
    return std::make_unique<action::AddClusterAction>(clusters.back());
}

std::unique_ptr<action::Action> app::ModelViewInterface::linkBlocks(Model&        model,
                                                                    ScrollArea&   scrollArea,
                                                                    const GridXY& point,
                                                                    const GridXY& previousPoint) {
    auto&      clusters = model.clusters();
    const auto baseIt   = std::find_if(_IT_(clusters), _FUNC_(cluster, cluster.contains(previousPoint)));
    if (baseIt == clusters.end() || (not previousPoint.isAdjacent(point))) {
        return addSingleBlockCluster(model, scrollArea, point);
    }
    auto extensionIt = std::find_if(_IT_(clusters), _FUNC_(cluster, cluster.contains(point)));
    if (baseIt == extensionIt) {
        return nullptr;
    }
    if (extensionIt == clusters.end()) {
        baseIt->addGridXY(point);
        assert(baseIt->connected());
        return std::make_unique<action::AddBlockToClusterAction>(baseIt->index(), point);
    }
    auto copyOfModel = model;
    baseIt->gridXY().merge(extensionIt->gridXY());
    clusters.erase(extensionIt);
    return std::make_unique<action::GenericModelAction>(copyOfModel, model);
}

void app::ModelViewInterface::leftMouseClick(Model& model, View& view, ScrollArea& scrollArea, const GridXY& point) {
    if (not model.level().isFreeStartBlock(point)) {
        return;
    }
    addAction(addSingleBlockCluster(model, scrollArea, point));
    updateCommandScrollArea(model, scrollArea, APP_MODE::EDITING);
}

void app::ModelViewInterface::clearBlockFromCluster(Model& model, View& view, ScrollArea& scrollArea, const GridXY& point) {
    if (model.level().isFreeStartBlock(point)) {
        addAction(clearBlockFromCluster_static(model, scrollArea, point));
        removeActionBoxesOfRemovedClusters(model.clusters(), scrollArea);
        scrollArea.setHeightAndPositions();
    }
}

void app::ModelViewInterface::clusterDrag(Model& model, View& view, ScrollArea& scrollArea, const GridXY& point, const GridXY& previousPoint) {
    if (not model.level().isFreeStartBlock(point)) {
        return;
    }
    if (model.level().isFreeStartBlock(previousPoint)) {
        addAction(linkBlocks(model, scrollArea, point, previousPoint));
    } else {
        addAction(addSingleBlockCluster(model, scrollArea, point));
    }
    updateCommandScrollArea(model, scrollArea, APP_MODE::EDITING);
}
