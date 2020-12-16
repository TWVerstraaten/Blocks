//
// Created by pc on 15-12-20.
//

#include "Model.h"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace model {

    Model::Model() {
        m_clusters.emplace_back(Cluster{{{0, 1}, {1, 0}, {1, 1}, {1, 2}}, {9, 1}});
        m_clusters.back().addAction(Cluster::Action::MOVE_DOWN);
        m_clusters.emplace_back(Cluster{{{0, 0}, {1, 0}, {2, 0}}, {10, 7}});
        m_clusters.back().addAction(Cluster::Action::MOVE_LEFT);
    }

    const std::vector<model::Cluster>& Model::clusters() const {
        return m_clusters;
    }

    void Model::interactClustersWithDynamicBlocks() {
        for (auto& cluster : m_clusters) {
            for (auto it = cluster.localIndexPairs().begin(); it != cluster.localIndexPairs().end(); ++it) {
                cluster.addPendingOperation(*it,
                                            m_level.dynamicBlockAt(IndexPair(it->row() + cluster.rowOffset(),
                                                                             it->column() + cluster.columnOffset())));
            }
        }
        for (auto& cluster : m_clusters) {
            cluster.performPendingOperation();
        }
        m_clusters.erase(std::remove_if(m_clusters.begin(), m_clusters.end(),
                                        [](const Cluster& cluster) { return cluster.empty(); }),
                         m_clusters.end());
    }

    void Model::interactClustersWithInstantBlocks() {
        for (auto& cluster : m_clusters) {
            bool isDone = false;
            for (auto it = cluster.localIndexPairs().begin(); it != cluster.localIndexPairs().end() && not isDone;
                 ++it) {
                switch (m_level.instantBlockAt(
                    IndexPair(it->row() + cluster.rowOffset(), it->column() + cluster.columnOffset()))) {
                    case Level::INSTANT_BLOCK_TYPE::NONE:
                        break;
                    case Level::INSTANT_BLOCK_TYPE::KILL:
                        isDone = true;
                        cluster.removeBLock(*it);
                        break;
                }
            }
        }
        m_clusters.erase(std::remove_if(m_clusters.begin(), m_clusters.end(),
                                        [](const Cluster& cluster) { return cluster.empty(); }),
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