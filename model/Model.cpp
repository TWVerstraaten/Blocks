//
// Created by pc on 15-12-20.
//

#include "Model.h"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace model {

    Model::Model() {

        m_clusters.emplace_back(Cluster{{{0, 0}}, {1, 1}});
        m_clusters.back().addAction({ClusterAction::ACTION::MOVE_UP, ClusterAction::MODIFIER::NONE});
        m_clusters.back().addAction({ClusterAction::ACTION::MOVE_LEFT, ClusterAction::MODIFIER::NONE});
        m_clusters.back().addAction({ClusterAction::ACTION::MOVE_DOWN, ClusterAction::MODIFIER::NONE});
        m_clusters.back().addAction({ClusterAction::ACTION::MOVE_RIGHT, ClusterAction::MODIFIER::NONE});

        m_clusters.emplace_back(Cluster{{{0, 1}, {1, 0}, {1, 1}, {1, 2}}, {9, 1}});
        m_clusters.back().addAction({ClusterAction::ACTION::MOVE_UP, ClusterAction::MODIFIER::NONE});
        m_clusters.emplace_back(Cluster{{{0, 0}, {1, 0}, {2, 0}}, {10, 7}});
        m_clusters.back().addAction({ClusterAction::ACTION::MOVE_LEFT, ClusterAction::MODIFIER::NONE});
        m_clusters.back().addAction({ClusterAction::ACTION::MOVE_RIGHT, ClusterAction::MODIFIER::SKIP});
        m_clusters.back().addAction({ClusterAction::ACTION::MOVE_UP, ClusterAction::MODIFIER::SKIP});
        m_clusters.back().addAction({ClusterAction::ACTION::MOVE_DOWN, ClusterAction::MODIFIER::SKIP});
        m_clusters.back().addAction({ClusterAction::ACTION::MOVE_LEFT, ClusterAction::MODIFIER::IGNORE});

        m_level.addBlock({5, 1}, Level::DYNAMIC_BLOCK_TYPE::ROTATE_CCW);
        m_level.addBlock({10, 6}, Level::DYNAMIC_BLOCK_TYPE::ROTATE_CW);
        m_level.addBlock({5, 4}, Level::DYNAMIC_BLOCK_TYPE::ROTATE_CCW);

        m_level.addBlock({5, 6}, Level::INSTANT_BLOCK_TYPE::KILL);
        m_level.addBlock({3, 1}, Level::INSTANT_BLOCK_TYPE::KILL);
        m_level.addBlock({7, 2}, Level::INSTANT_BLOCK_TYPE::KILL);
        m_level.addBlock({4, 4}, Level::INSTANT_BLOCK_TYPE::KILL);
    }

    const std::list<model::Cluster>& Model::clusters() const {
        return m_clusters;
    }

    void Model::interactClustersWithDynamicBlocks() {
        for (auto& cluster : m_clusters) {
            for (auto it = cluster.gridCoordinates().begin(); it != cluster.gridCoordinates().end(); ++it) {
                cluster.addPendingOperation(*it, m_level.dynamicBlockAt(GridCoordinates(it->x(), it->y())));
            }
        }
        for (auto& cluster : m_clusters) {
            cluster.performPendingOperation();
        }
        m_clusters.erase(std::remove_if(m_clusters.begin(), m_clusters.end(), [](const Cluster& cluster) { return cluster.empty(); }),
                         m_clusters.end());
    }

    void Model::interactClustersWithInstantBlocks() {
        for (auto& cluster : m_clusters) {
            bool isDone = false;
            for (auto it = cluster.gridCoordinates().begin(); it != cluster.gridCoordinates().end() && not isDone; ++it) {
                switch (m_level.instantBlockAt(GridCoordinates(it->x(), it->y()))) {
                    case Level::INSTANT_BLOCK_TYPE::NONE:
                        break;
                    case Level::INSTANT_BLOCK_TYPE::KILL:
                        isDone = true;
                        cluster.removeBLock(*it);
                        break;
                }
            }
        }
        m_clusters.erase(std::remove_if(m_clusters.begin(), m_clusters.end(), [](const Cluster& cluster) { return cluster.empty(); }),
                         m_clusters.end());
    }

    const model::Level& Model::level() const {
        return m_level;
    }

    void Model::moveClusters() {
        for (auto& cluster : m_clusters) {
            cluster.doAction();
        }
    }

    void Model::update(double fractionOfPhase) {
        for (auto& cluster : m_clusters) {
            cluster.update(fractionOfPhase);
        }
    }
} // namespace model