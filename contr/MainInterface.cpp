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

    using namespace model;

    void MainInterface::startInteractPhase(Model& model, view::CommandScroll& commandScrollArea) {
        auto&       clusters = model.clusters();
        const auto& level    = model.level();
        model.resetPhase();

        handleKillBlocks(level.instantBlocks(), clusters);
        handleConwayFloorBlocks(model);

        model.clearEmpty();
        model.splitDisconnectedClusters();
        model.clearEmpty();
        commandScrollArea.addNeeded(clusters);
        handleDynamicBlocks(level.dynamicBlocks(), clusters);
        commandScrollArea.updateSelection();
    }

    void MainInterface::startMovePhase(Model& model, view::CommandScroll& commandScrollArea) {
        auto& clusters = model.clusters();
        auto& level    = model.level();

        model.resetPhase();
        handleStopAndSplice(model);
        handleGrabs(model);

        std::copy_if(D_CIT(clusters), std::back_inserter(level.stoppedClusters()), D_FUNC(cluster, cluster.state() == CLUSTER_STATE::STOPPED));
        model.clearEmpty();
        model.clearStopped();
        model.splitDisconnectedClusters();
        commandScrollArea.removeUnneeded(clusters);
        commandScrollArea.addNeeded(clusters);

        for (auto& cluster : clusters) {
            cluster.doCommand(model);
        }
        model.update(0.0001);
        commandScrollArea.updateSelection();
    }

    void MainInterface::handleKillBlocks(const std::map<GridXy, INSTANT_BLOCK_TYPE>& instantBlocks, std::vector<Cluster>& clusters) {
        for (const auto& [point, type] : instantBlocks) {
            switch (type) {
                case INSTANT_BLOCK_TYPE::KILL:
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

    void MainInterface::handleDynamicBlocks(const std::map<GridXy, DYNAMIC_BLOCK_TYPE>& dynamicBlocks, std::vector<Cluster>& clusters) {
        for (auto& cluster : clusters) {
            bool               noPendingAction = true;
            GridXy             p;
            DYNAMIC_BLOCK_TYPE t;
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

    void MainInterface::handleConwayFloorBlocks(Model& model) {
        const auto          conwayBlocks = model.level().blocks(FLOOR_BLOCK_TYPE::CONWAY);
        auto&               clusters     = model.clusters();
        std::vector<GridXy> toRemove;
        Cluster             newCluster;

        for (const auto& conwayBlock : conwayBlocks) {
            size_t liveNeighbors = 0;
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    if (j == 0 && i == 0) {
                        continue;
                    }
                    if ((model.clusterContaining(conwayBlock + GridXy{i, j}) != clusters.end()) ||
                        (model.stoppedClusterContaining(conwayBlock + GridXy{i, j})) != model.level().stoppedClusters().end()) {
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
            std::vector<Cluster> newClusters;
            newClusters.emplace_back(std::move(newCluster));
            geom::splitDisconnectedClusters(newClusters);
            for (const auto& it : newClusters) {
                assert(not it.isEmpty());
                assert(it.isConnected());
            }
            for (auto& it : newClusters) {
                const auto& neighbors = geom::neighbors(model.clusters(), it);
                if (neighbors.size() == 1) {
                    neighbors.front()->copyGridXy(it);
                    it.clearGridXy();
                }
            }
            auto& stoppedClusters = model.level().stoppedClusters();
            stoppedClusters.reserve(stoppedClusters.size() + std::count_if(D_CIT(newClusters), D_FUNC(cluster, not cluster.isEmpty())));

            for (size_t i = 0; i != newClusters.size(); ++i) {
                stoppedClusters.emplace_back(std::move(newClusters[i]));
            }
        }
    }

    void MainInterface::handleStopAndSplice(Model& model) {
        auto& clusters = model.clusters();
        auto& level    = model.level();
        for (auto& cluster : clusters) {
            switch (cluster.currentType()) {
                case COMMAND_TYPE::STP:
                    cluster.setState(CLUSTER_STATE::STOPPED);
                    break;
                case COMMAND_TYPE::SPL:
                    cluster.spliceCluster(level);
                    break;
                default:
                    break;
            }
        }

        std::copy_if(D_CIT(clusters), std::back_inserter(level.stoppedClusters()), D_FUNC(cluster, cluster.state() == CLUSTER_STATE::STOPPED));
        model.clearEmpty();
        model.clearStopped();
        model.splitDisconnectedClusters();
    }

    void MainInterface::handleGrabs(Model& model) {
        auto& clusters = model.clusters();
        auto& level    = model.level();

        std::map<Cluster*, std::vector<Cluster*>> grabberMap;
        for (auto& cluster : clusters) {
            switch (cluster.currentType()) {
                case COMMAND_TYPE::GRB: {
                    const auto neighbors = geom::neighbors(model.level().stoppedClusters(), cluster);
                    for (auto* it : neighbors) {
                        if (grabberMap.find(it) == grabberMap.end()) {
                            grabberMap[it] = {&cluster};
                        } else {
                            grabberMap[it].emplace_back(&cluster);
                        }
                    }
                }
                default:
                    break;
            }
        }

        for (auto& [grabbee, grabbers] : grabberMap) {
            if (grabbers.size() > 1) {
                std::for_each(D_IT(grabbers), D_VOID_FUNC(grabber, grabber->setState(CLUSTER_STATE::STOPPED);));
            }
        }

        for (auto& [grabbee, grabbers] : grabberMap) {
            if (grabbers.size() == 1) {
                grabbers.front()->copyGridXy(*grabbee);
                grabbee->clearGridXy();
            }
        }
        model.clearEmpty();
    }

} // namespace contr
