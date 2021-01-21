////
//// Created by pc on 28-12-20.
////
//
//#include "ModelViewInterface.h"
//
//#include "../misc/defines.h"
//#include "../misc/overloaded.h"
//#include "ApplicationEdit.h"
//
//#include <cassert>
//
//using namespace model;
//using namespace view;
//using namespace widget;
//
//static std::list<Cluster>::iterator findCluster(const CommandEditWidget& commandEditBox, std::list<Cluster>& clusters) {
//    return std::find_if(D_IT(clusters), D_FUNC(cluster, cluster.m_currentIndex() == commandEditBox.m_currentIndex()));
//}
//
//static std::list<Cluster>::const_iterator findCluster(const CommandEditWidget& commandEditBox, const std::list<Cluster>& clusters) {
//    return std::find_if(D_CIT(clusters), D_FUNC(cluster, cluster.m_currentIndex() == commandEditBox.m_currentIndex()));
//}
//
//static std::list<CommandEditWidget>::iterator findCommandEditBox(const Cluster& cluster, std::list<CommandEditWidget>& commandEditBoxes) {
//    return std::find_if(D_IT(commandEditBoxes), D_FUNC(box, cluster.m_currentIndex() == box.m_currentIndex()));
//}
//
//static void updateCommandEditBoxes(const std::list<Cluster>& clusters, ScrollArea& scrollArea) {
//    for (auto& commandEditBox : scrollArea.children()) {
//        auto it = findCluster(commandEditBox, clusters);
//        assert(it != clusters.end());
//        if (it->isAlive()) {
//            commandEditBox.updateComments(it->commandVector());
//            commandEditBox.updateSelected(it->commandVector());
//        }
//        commandEditBox.setActive(it->isAlive());
//    }
//}
//
//static void addCommandEditBoxesOfNewClusters(const std::list<Cluster>& clusters, ScrollArea& scrollArea) {
//    auto it = std::find_if(D_IT(clusters), D_FUNC(cluster, findCommandEditBox(cluster, scrollArea.children()) == scrollArea.children().end()));
//    while (it != clusters.end()) {
//        scrollArea.addCommandEditBox(*it);
//        it = std::find_if(D_IT(clusters), D_FUNC(cluster, findCommandEditBox(cluster, scrollArea.children()) == scrollArea.children().end()));
//    }
//}
//
//void app::ModelViewInterface::leftMouseDrag(model::Model&             model,
//                                            view::widget::ScrollArea& scrollArea,
//                                            const model::GridXy&      point,
//                                            const model::GridXy&      previousPoint,
//                                            const model::BlockType&   selectedBlockType) {
//    std::visit(overloaded{[&](const CLUSTER_TYPE) { clusterDrag(model, scrollArea, point, previousPoint); },
//                          [&](const auto type) { addAction(model.level().addBlock(point, type)); }},
//               selectedBlockType);
//}
//
//void app::ModelViewInterface::leftClickControl(model::Model&             model,
//                                               view::widget::ScrollArea& scrollArea,
//                                               const model::GridXy&      point,
//                                               const model::BlockType&   selectedBlockType) {
//    std::visit(overloaded{[&](const CLUSTER_TYPE) { clearBlockFromCluster(model, scrollArea, point); },
//                          [&](auto) {
//                              if (model.noLiveOrStoppedClusterOnBlock(point)) {
//                                  addAction(model.level().removeBlock(point));
//                              } else {
//                                  clearBlockFromCluster(model, scrollArea, point);
//                              }
//                          }},
//               selectedBlockType);
//}
//
//void app::ModelViewInterface::leftMouseClick(model::Model&             model,
//                                             view::widget::ScrollArea& scrollArea,
//                                             const model::GridXy&      point,
//                                             const model::BlockType&   selectedBlockType) {
//    std::visit(overloaded{[&](const CLUSTER_TYPE) { leftMouseClick(model, scrollArea, point); },
//                          [&](const auto type) { addAction(model.level().addBlock(point, type)); }},
//               selectedBlockType);
//}
//
//void app::ModelViewInterface::addAction(Action_u_ptr&& action) {
//    if (action) {
//        m_undoStack.push(std::move(action));
//        m_redoStack = {};
//    }
//}
//
//void app::ModelViewInterface::updateCommandScrollArea(Model& model, ScrollArea& scrollArea, app::APP_MODE mode) {
//    removeActionBoxesOfRemovedClusters(model.clusters(), scrollArea);
//    if (mode == APP_MODE::RUNNING) {
//        updateCommandEditBoxes(model.clusters(), scrollArea);
//    }
//    addCommandEditBoxesOfNewClusters(model.clusters(), scrollArea);
//    scrollArea.setHeightAndPositions();
//}
//
//void app::ModelViewInterface::undo(app::ApplicationEdit& applicationEdit) {
//    if (m_undoStack.empty()) {
//        return;
//    }
//    //    m_undoStack.top()->undo(applicationEdit);
//    m_redoStack.push(std::move(m_undoStack.top()));
//    assert(m_undoStack.top() == nullptr);
//    m_undoStack.pop();
//    updateCommandScrollArea(*applicationEdit.model(), *applicationEdit.scrollArea(), APP_MODE::EDITING);
//}
//
//void app::ModelViewInterface::redo(app::ApplicationEdit& applicationEdit) {
//    if (m_redoStack.empty()) {
//        return;
//    }
//    //    m_redoStack.top()->redo(applicationEdit);
//    m_undoStack.push(std::move(m_redoStack.top()));
//    assert(m_redoStack.top() == nullptr);
//    m_redoStack.pop();
//    updateCommandScrollArea(*applicationEdit.model(), *applicationEdit.scrollArea(), APP_MODE::EDITING);
//}
//
//std::unique_ptr<action::Action> app::ModelViewInterface::clearBlockFromCluster_static(Model& model, ScrollArea& scrollArea, const GridXy& point) {
//    auto&      clusters = model.clusters();
//    const auto it       = model.clusterContaining(point);
//    if (it == clusters.end()) {
//        return nullptr;
//    }
//    if (it->size() > 1) {
//        return clearBlockFromCluster(model, scrollArea, point, *it);
//    }
//    clusters.erase(it);
//    return nullptr;
//}
//
//void app::ModelViewInterface::handleKeyEvent(const SDL_Event& event, ScrollArea& scrollArea, Model& model) {
//    const auto w = scrollArea.focusedWidget();
//    if (w) {
//        CommandEditWidget copy(*w);
//        w->keyEvent(event);
//        if (w->clusterShouldBeUpdated()) {
//            auto clusterIt = findCluster(*w, model.clusters());
//            w->updateClusterCommands(*clusterIt);
//        }
//    }
//}
//
//void app::ModelViewInterface::interactWithInstantBlocks(Model& model, ScrollArea& scrollArea) {
//    for (const auto& [point, type] : model.level().instantBlocks()) {
//        switch (type) {
//            case INSTANT_BLOCK_TYPE::KILL:
//                clearBlockFromCluster_static(model, scrollArea, point);
//                break;
//            default:
//                assert(false);
//        }
//    }
//}
//
//void app::ModelViewInterface::stopSpliceOrKillIfNeeded(Level& level, Cluster& cluster) {
//    if (not cluster.isAlive()) {
//        return;
//    }
//    std::vector<std::pair<const GridXy, DYNAMIC_BLOCK_TYPE>> pendingOperations;
//    for (const auto& [point, type] : level.dynamicBlocks()) {
//        if (cluster.contains(point)) {
//            pendingOperations.emplace_back(point, type);
//        }
//    }
//    assert(cluster.pendingDynamicMoves() == model::PENDING_DYNAMIC_MOVES::ZERO);
//    if (pendingOperations.size() > 1 && cluster.currentModifier() != COMMAND_MODIFIER::IGNORE) {
//        cluster.kill();
//        cluster.setPendingDynamicMoves(model::PENDING_DYNAMIC_MOVES::TOO_MANY);
//    } else {
//        if (pendingOperations.size() == 1 && cluster.currentModifier() != COMMAND_MODIFIER::IGNORE) {
//            cluster.setPendingDynamicMoves(model::PENDING_DYNAMIC_MOVES::ONE);
//            return;
//        }
//        if (cluster.commandVector().currentType() == COMMAND_TYPE::STP) {
//            cluster.setState(CLUSTER_STATE::STOPPED);
//        }
//        if (cluster.commandVector().currentType() == COMMAND_TYPE::SPL) {
//            cluster.spliceCluster(level);
//        }
//    }
//}
//
//bool app::ModelViewInterface::interactWithDynamicBlocks(Level& level, Cluster& cluster) {
//    if (not cluster.isAlive() || cluster.currentModifier() == COMMAND_MODIFIER::IGNORE) {
//        return false;
//    }
//    std::vector<std::pair<const GridXy, DYNAMIC_BLOCK_TYPE>> pendingOperations;
//    for (const auto& [point, type] : level.dynamicBlocks()) {
//        if (cluster.contains(point)) {
//            pendingOperations.emplace_back(point, type);
//        }
//    }
//    if (pendingOperations.empty()) {
//        return false;
//    }
//    assert(pendingOperations.size() == 1);
//    cluster.handleDynamicBlock(pendingOperations.front().first, pendingOperations.front().second);
//    return true;
//}
//
//std::unique_ptr<action::Action> app::ModelViewInterface::clearBlockFromCluster(Model&        model,
//                                                                               ScrollArea&   scrollArea,
//                                                                               const GridXy& point,
//                                                                               Cluster&      cluster) {
//    assert(cluster.contains(point));
//    cluster.removeGridXy(point);
//    if (cluster.isConnected()) {
//        return nullptr;
//    }
//    Model copy = model;
//    split(model, scrollArea, cluster);
//    return nullptr;
//}
//
//void app::ModelViewInterface::splitIfDisconnected(Model& model, ScrollArea& scrollArea, Cluster& cluster) {
//    if (not cluster.isConnected()) {
//        split(model, scrollArea, cluster);
//    }
//}
//
//void app::ModelViewInterface::split(Model& model, ScrollArea& scrollArea, Cluster& cluster) {
//    auto newClusters   = cluster.collectAllButFirstComponent();
//    auto commandEditIt = findCommandEditBox(cluster, scrollArea.children());
//    assert(commandEditIt != scrollArea.children().end());
//    addCommandEditBoxesOfNewClusters(newClusters, scrollArea);
//    for (const auto& newCluster : newClusters) {
//        auto newCommandEditIt = findCommandEditBox(newCluster, scrollArea.children());
//        assert(newCommandEditIt != scrollArea.children().end());
//        newCommandEditIt->setStrings(commandEditIt->strings());
//    }
//    assert(cluster.isConnected());
//    model.clusters().splice(model.clusters().end(), newClusters);
//}
//
//std::unique_ptr<action::Action> app::ModelViewInterface::addSingleBlockToCluster(Model& model, ScrollArea& scrollArea, const GridXy& point) {
//    assert(model.level().isFreeStartBlock(point));
//    auto& clusters = model.clusters();
//    if (model.clusterContaining(point) != clusters.end()) {
//        return nullptr;
//    }
//    clusters.push_back(Cluster({point}, "CL" + std::to_string(clusters.size())));
//    addCommandEditBoxesOfNewClusters(clusters, scrollArea);
//    return nullptr;
//}
//
//std::unique_ptr<action::Action> app::ModelViewInterface::linkBlocks(Model&        model,
//                                                                    ScrollArea&   scrollArea,
//                                                                    const GridXy& point,
//                                                                    const GridXy& previousPoint) {
//    auto&      clusters = model.clusters();
//    const auto baseIt   = std::find_if(D_IT(clusters), D_FUNC(cluster, cluster.contains(previousPoint)));
//    if (baseIt == clusters.end() || (not previousPoint.isAdjacent(point))) {
//        return addSingleBlockToCluster(model, scrollArea, point);
//    }
//    auto extensionIt = std::find_if(D_IT(clusters), D_FUNC(cluster, cluster.contains(point)));
//    if (baseIt == extensionIt) {
//        return nullptr;
//    }
//    if (extensionIt == clusters.end()) {
//        baseIt->addGridXy(point);
//        assert(baseIt->isConnected());
//        return nullptr;
//    }
//    auto copyOfModel = model;
//    baseIt->gridXy().merge(extensionIt->gridXy());
//    clusters.erase(extensionIt);
//    return nullptr;
//}
//
//void app::ModelViewInterface::leftMouseClick(Model& model, ScrollArea& scrollArea, const GridXy& point) {
//    if (not model.level().isFreeStartBlock(point)) {
//        return;
//    }
//    addAction(addSingleBlockToCluster(model, scrollArea, point));
//    updateCommandScrollArea(model, scrollArea, APP_MODE::EDITING);
//}
//
//void app::ModelViewInterface::clearBlockFromCluster(Model& model, ScrollArea& scrollArea, const GridXy& point) {
//    if (model.level().isFreeStartBlock(point)) {
//        addAction(clearBlockFromCluster_static(model, scrollArea, point));
//        removeActionBoxesOfRemovedClusters(model.clusters(), scrollArea);
//        scrollArea.setHeightAndPositions();
//    }
//}
//
//void app::ModelViewInterface::clusterDrag(model::Model&             model,
//                                          view::widget::ScrollArea& scrollArea,
//                                          const model::GridXy&      point,
//                                          const model::GridXy&      previousPoint) {
//    if (not model.level().isFreeStartBlock(point)) {
//        return;
//    }
//    if (model.level().isFreeStartBlock(previousPoint)) {
//        addAction(linkBlocks(model, scrollArea, point, previousPoint));
//    } else {
//        addAction(addSingleBlockToCluster(model, scrollArea, point));
//    }
//    updateCommandScrollArea(model, scrollArea, APP_MODE::EDITING);
//}
//
//void app::ModelViewInterface::updateSelection(const std::list<model::Cluster>& clusters, ScrollArea& scrollArea) {
//    for (auto& commandEditBox : scrollArea.children()) {
//        auto it = findCluster(commandEditBox, clusters);
//        assert(it != clusters.end());
//        if (it->isAlive()) {
//            commandEditBox.updateSelected(it->commandVector());
//        }
//        commandEditBox.setActive(it->isAlive());
//    }
//}
//
//void app::ModelViewInterface::removeActionBoxesOfRemovedClusters(const std::list<model::Cluster>& clusters, ScrollArea& scrollArea) {
//    scrollArea.children().remove_if(D_FUNC(box, findCluster(box, clusters) == clusters.end()));
//}
