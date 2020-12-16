//
// Created by pc on 15-12-20.
//

#include "Model.h"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace model {

    Model::Model() {
        m_clusters.emplace_back(Cluster{Matrix{{{false, true, false}, {true, true, true}}}, 8, 1});
        m_clusters.back().addAction(Cluster::Action::MOVE_UP);
        m_clusters.emplace_back(Cluster{Matrix{{{true}, {true}, {true}}}, 10, 7});
        m_clusters.back().addAction(Cluster::Action::MOVE_LEFT);
    }

    const std::vector<model::Cluster>& Model::clusters() const {
        return m_clusters;
    }

    void Model::interactClustersWithLevel() {
        for (auto& cluster : m_clusters) {
            for (auto it = cluster.indexPairs().begin(); it != cluster.indexPairs().end(); ++it) {
                cluster.interactWithBlock(*it, m_level.blockAt(IndexPair(it->row() + cluster.rowOffset(),
                                                                         it->column() + cluster.columnOffset())));
            }
        }
        for (auto& cluster : m_clusters) {
            cluster.performPendingActions();
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
            cluster.doStep();
        }
    }
} // namespace model