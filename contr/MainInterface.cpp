//
// Created by pc on 23-01-21.
//

#include "MainInterface.h"

#include "../action/RemoveBlockFromClusterAction.h"
#include "../misc/defines.h"
#include "../misc/geom.h"
#include "../model/Model.h"
#include "../view/widget/CommandScroll.h"

namespace contr {

    void MainInterface::startInteractPhase(model::Model& model, view::CommandScroll& commandScrollArea) {
        auto&       clusters = model.clusters();
        const auto& level    = model.level();
        model.resetPhase();
        for (auto& cluster : model.clusters()) {
            cluster.incrementCommandIndex();
        }

        handleKillBlocks(level.instantBlocks(), clusters);
        handleConwayFloorBlocks(model);

        model.clearEmpty();
        model.splitDisconnectedClusters();
        model.clearEmpty();
        commandScrollArea.addNeeded(clusters);
        handleDynamicBlocks(level.dynamicBlocks(), clusters);
        commandScrollArea.updateSelection();
    }

    void MainInterface::startMovePhase(model::Model& model, view::CommandScroll& commandScrollArea) {
        auto& clusters = model.clusters();
        auto& level    = model.level();

        model.resetPhase();
        for (auto& cluster : clusters) {
            switch (cluster.currentType()) {
                case model::COMMAND_TYPE::STP:
                    cluster.setState(model::CLUSTER_STATE::STOPPED);
                    break;
                case model::COMMAND_TYPE::SPL:
                    cluster.spliceCluster(level);
                    break;
                default:
                    break;
            }
        }

        std::copy_if(D_CIT(clusters), std::back_inserter(level.stoppedClusters()), D_FUNC(cluster, cluster.state() == model::CLUSTER_STATE::STOPPED));
        model.clearEmpty();
        model.splitDisconnectedClusters();
        commandScrollArea.removeUnneeded(clusters);
        commandScrollArea.addNeeded(clusters);

        for (auto& cluster : clusters) {
            cluster.doCommand(model);
        }
        model.update(0.0001);
        commandScrollArea.updateSelection();
    }

    void MainInterface::handleKillBlocks(const std::map<model::GridXy, model::INSTANT_BLOCK_TYPE>& instantBlocks,
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

    void MainInterface::handleDynamicBlocks(const std::map<model::GridXy, model::DYNAMIC_BLOCK_TYPE>& dynamicBlocks,
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

    void MainInterface::handleConwayFloorBlocks(model::Model& model) {
        const auto                 conwayBlocks = model.level().blocks(model::FLOOR_BLOCK_TYPE::CONWAY);
        auto&                      clusters     = model.clusters();
        std::vector<model::GridXy> toRemove;
        model::Cluster             newCluster;

        for (const auto& conwayBlock : conwayBlocks) {
            size_t liveNeighbors = 0;
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    if (j == 0 && i == 0) {
                        continue;
                    }
                    if ((model.clusterContaining(conwayBlock + model::GridXy{i, j}) != clusters.end()) ||
                        (model.stoppedClusterContaining(conwayBlock + model::GridXy{i, j})) != model.level().stoppedClusters().end()) {
                        ++liveNeighbors;
                    }
                }
            }
            if ((model.clusterContaining(conwayBlock) != model.clusters().end()) ||
                (model.stoppedClusterContaining(conwayBlock)) != model.level().stoppedClusters().end()) {
                if (liveNeighbors != 2 && liveNeighbors != 3) {
                    toRemove.emplace_back(conwayBlock);
                }
            } else {
                if (liveNeighbors == 3) {
                    newCluster.addGridXy(conwayBlock);
                }
            }
        }

        for (const auto& r : toRemove) {
            if (model.clusterContaining(r) != model.clusters().end()) {
                model.clusterContaining(r)->removeGridXy(r);
            } else {
                model.stoppedClusterContaining(r)->removeGridXy(r);
            }
        }
        if (not newCluster.isEmpty()) {
            model.level().stoppedClusters().emplace_back(std::move(newCluster));
            model.clearEmpty();
            geom::splitDisconnectedClusters(model.level().stoppedClusters());
        }
    }
} // namespace contr
