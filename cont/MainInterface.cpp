//
// Created by pc on 23-01-21.
//

#include "MainInterface.h"

#include "../action/RemoveBlockFromClusterAction.h"
#include "../misc/defines.h"
#include "../model/Model.h"
#include "../view/widget/CommandScrollArea.h"

void cont::MainInterface::startInteractPhase(model::Model& model, view::CommandScrollArea& commandScrollArea) {
    auto&       clusters = model.clusters();
    const auto& level    = model.level();
    model.resetPhase();
    for (auto& cluster : model.clusters()) {
        cluster.incrementCommandIndex();
    }

    handleKillBlocks(level.instantBlocks(), clusters);

    model.clearEmpty();
    model.splitDisconnectedClusters();
    model.clearEmpty();
    commandScrollArea.addNeeded(clusters);

    handleDynamicBlocks(level.dynamicBlocks(), clusters);

    commandScrollArea.updateSelection();
}

void cont::MainInterface::startMovePhase(model::Model& model, view::CommandScrollArea& commandScrollArea) {
    auto& clusters = model.clusters();
    auto& level    = model.level();

    model.resetPhase();
    for (auto& cluster : clusters) {
        if (cluster.currentType() == model::COMMAND_TYPE::STP) {
            cluster.setState(model::CLUSTER_STATE::STOPPED);
        }
    }

    std::copy_if(D_CIT(clusters), std::back_inserter(level.stoppedClusters()), D_FUNC(cluster, cluster.state() == model::CLUSTER_STATE::STOPPED));
    commandScrollArea.removeUnneeded(clusters);
    clusters.erase(std::remove_if(D_IT(clusters), D_FUNC(cluster, cluster.state() == model::CLUSTER_STATE::STOPPED)), clusters.end());

    for (auto& cluster : clusters) {
        cluster.doCommand(model);
    }
    model.update(0.0001);
    commandScrollArea.updateSelection();
}

void cont::MainInterface::handleKillBlocks(const std::map<model::GridXy, model::INSTANT_BLOCK_TYPE>& instantBlocks,
                                           std::vector<model::Cluster>&                              clusters) {
    for (const auto& [point, type] : instantBlocks) {
        switch (type) {
            case model::INSTANT_BLOCK_TYPE::KILL:
                for (auto& cluster : clusters) {
                    if (cluster.contains(point)) {
                        cluster.removeGridXy(point);
                        break;
                    }
                }
                break;
        }
    }
}

void cont::MainInterface::handleDynamicBlocks(const std::map<model::GridXy, model::DYNAMIC_BLOCK_TYPE>& dynamicBlocks,
                                              std::vector<model::Cluster>&                              clusters) {
    for (auto& cluster : clusters) {
        bool                      noPendingAction = true;
        model::GridXy             p;
        model::DYNAMIC_BLOCK_TYPE t;
        for (const auto& [point, type] : dynamicBlocks) {
            if (cluster.contains(point)) {
                if (noPendingAction) {
                    p               = point;
                    t               = type;
                    noPendingAction = false;
                } else {
                    cluster.kill();
                }
            }
        }
        if (cluster.isAlive() && (not noPendingAction)) {
            cluster.handleDynamicBlock(p, t);
        }
    }
}
