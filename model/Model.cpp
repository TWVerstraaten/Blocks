//
// Created by pc on 15-12-20.
//

#include "Model.h"

#include "../aux/Aux.h"

#include <algorithm>

namespace model {

    Model::Model() {
        init();
    }

    Model::Model(const Model& other) {
        *this = other;
    }

    const std::vector<model::Cluster>& Model::clusters() const {
        return m_clusters;
    }

    std::vector<model::Cluster>& Model::clusters() {
        return m_clusters;
    }

    void Model::interactClustersWithDynamicBlocks() {
        for (auto& cluster : m_clusters) {
            for (auto it = cluster.gridXY().begin(); it != cluster.gridXY().end(); ++it) {
                cluster.addPendingOperation(*it, m_level.dynamicBlockAt(GridXY(it->x(), it->y())));
            }
        }
        for (auto& cluster : m_clusters) {
            cluster.performPendingOperationOrNextAction();
        }
        clearEmptyClusters();
    }

    void Model::interactClustersWithInstantBlocks() {
        for (auto& cluster : m_clusters) {
            bool isDone = false;
            for (auto it = cluster.gridXY().begin(); it != cluster.gridXY().end() && not isDone; ++it) {
                switch (m_level.instantBlockAt(GridXY(it->x(), it->y()))) {
                    case Level::INSTANT_BLOCK_TYPE::NONE:
                        break;
                    case Level::INSTANT_BLOCK_TYPE::KILL:
                        isDone = true;
                        cluster.removeBLock(*it);
                        break;
                }
            }
        }
        clearEmptyClusters();
    }

    void Model::interactClustersWithLevel() {
        for (auto& cluster : m_clusters) {
            if (not cluster.isAlive()) {
                continue;
            }
            const auto points = cluster.cornerPoints(1);
            for (const auto& point : points) {
                if (not m_level.isInLevel(point)) {
                    cluster.kill();
                    break;
                }
            }
        }
    }

    const model::Level& Model::level() const {
        return m_level;
    }

    void Model::update(double fractionOfPhase) {
        for (auto& cluster : m_clusters) {
            cluster.update(fractionOfPhase);
        }
    }

    void Model::init() {
        clear();

        m_clusters.emplace_back(Cluster{{{1, 3}}, "Cluster_" + std::to_string(m_clusters.size())});
        m_clusters.back().addAction({Action::VALUE::MOVE_UP, Action::MODIFIER::NONE});
        m_clusters.back().addAction({Action::VALUE::MOVE_LEFT, Action::MODIFIER::NONE});
        m_clusters.back().addAction({Action::VALUE::MOVE_DOWN, Action::MODIFIER::NONE});
        m_clusters.back().addAction({Action::VALUE::MOVE_RIGHT, Action::MODIFIER::NONE});

        //        m_clusters.emplace_back(Cluster{{{9, 4}}, "Cluster_" + std::to_string(m_clusters.size())});
        //        m_clusters.back().addAction({Action::VALUE::MOVE_UP, Action::MODIFIER::NONE});

        m_clusters.emplace_back(Cluster{{{10, 7}, {11, 7}, {12, 7}}, "Cluster_" + std::to_string(m_clusters.size())});
        m_clusters.back().addAction({Action::VALUE::MOVE_LEFT, Action::MODIFIER::NONE});
        m_clusters.back().addAction({Action::VALUE::MOVE_RIGHT, Action::MODIFIER::SKIP});
        m_clusters.back().addAction({Action::VALUE::MOVE_UP, Action::MODIFIER::SKIP});
        m_clusters.back().addAction({Action::VALUE::MOVE_DOWN, Action::MODIFIER::SKIP});
        m_clusters.back().addAction({Action::VALUE::MOVE_LEFT, Action::MODIFIER::IGNORE});

        m_level.addBlock({5, 1}, Level::DYNAMIC_BLOCK_TYPE::ROTATE_CCW);
        m_level.addBlock({10, 6}, Level::DYNAMIC_BLOCK_TYPE::ROTATE_CW);
        m_level.addBlock({5, 4}, Level::DYNAMIC_BLOCK_TYPE::ROTATE_CCW);

        m_level.addBlock({5, 6}, Level::INSTANT_BLOCK_TYPE::KILL);
        m_level.addBlock({3, 1}, Level::INSTANT_BLOCK_TYPE::KILL);
        m_level.addBlock({9, 3}, Level::INSTANT_BLOCK_TYPE::KILL);
        m_level.addBlock({7, 2}, Level::INSTANT_BLOCK_TYPE::KILL);
        m_level.addBlock({4, 4}, Level::INSTANT_BLOCK_TYPE::KILL);

        for (int i = -1; i != 14; ++i) {
            for (int j = -1; j != 9; ++j) {
                m_level.addLevelBlock({(i), (j)});
            }
        }
    }

    void Model::clear() {
        m_clusters.clear();
        m_level.clear();
    }

    void Model::clearEmptyClusters() {
        m_clusters.erase(std::remove_if(m_clusters.begin(), m_clusters.end(), [](const Cluster& cluster) { return cluster.empty(); }),
                         m_clusters.end());
    }

} // namespace model