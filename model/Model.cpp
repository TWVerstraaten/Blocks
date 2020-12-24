//
// Created by pc on 15-12-20.
//

#include "Model.h"

#include "../global/Global.h"

#include <algorithm>
#include <cassert>

namespace model {

    Model::Model() {
        init();
    }

    Model::Model(const Model& other) {
        *this = other;
    }

    const std::vector<Cluster>& Model::clusters() const {
        return m_clusters;
    }

    std::vector<model::Cluster>& Model::clusters() {
        return m_clusters;
    }

    void Model::interactClustersWithDynamicBlocks() {
        for (auto& cluster : m_clusters) {
            if (not cluster.isAlive()) {
                continue;
            }
            for (auto it = cluster.gridXY().begin(); it != cluster.gridXY().end(); ++it) {
                cluster.addPendingOperation(*it, m_level.dynamicBlockAt(GridXY(it->x(), it->y())));
            }
            cluster.performPendingOperationOrNextAction();
        }
    }

    void Model::interactClustersWithInstantBlocks() {
        for (auto& cluster : m_clusters) {
            for (auto it = cluster.gridXY().begin(); it != cluster.gridXY().end();) {
                switch (m_level.instantBlockAt(GridXY(it->x(), it->y()))) {
                    case Level::INSTANT_BLOCK_TYPE::KILL:
                        it = cluster.removeBLock(*it);
                        break;
                    default:
                        ++it;
                        break;
                }
            }
        }
        clearEmptyClusters();
        splitDisconnectedClusters();
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

            while (not cluster.isConnected()) {
                m_clusters.emplace_back(cluster.getComponent());
            }
        }
    }

    void Model::init() {
        clear();

        m_clusters.emplace_back(Cluster{{{3, 4}, {4, 4}}, "CL" + std::to_string(m_clusters.size())});
        m_clusters.back().addAction({Action::VALUE::MOVE_UP, Action::MODIFIER::NONE});

        m_level.addBlock({3, 3}, Level::INSTANT_BLOCK_TYPE::KILL);
        m_level.addBlock({4, 3}, Level::INSTANT_BLOCK_TYPE::KILL);

        m_clusters.emplace_back(Cluster{{{9, 7}, {10, 7}, {11, 7}, {12, 7}}, "CL" + std::to_string(m_clusters.size())});
        m_clusters.back().addAction({Action::VALUE::MOVE_UP, Action::MODIFIER::NONE});
        m_clusters.back().addAction({Action::VALUE::MOVE_UP, Action::MODIFIER::NONE});
        m_clusters.back().addAction({Action::VALUE::MOVE_UP, Action::MODIFIER::NONE});

        m_level.addBlock({11, 6}, Level::DYNAMIC_BLOCK_TYPE::ROTATE_CCW);

        for (int i = -1; i != 15; ++i) {
            for (int j = -1; j != 11; ++j) {
                if (i == 11 && j == 3) {
                    continue;
                }
                m_level.addLevelBlock({(i), (j)});
            }
        }
    }

    void Model::clear() {
        m_clusters.clear();
        m_level.clear();
    }

    void Model::clearEmptyClusters() {
        //        m_clusters.erase(std::remove_if(m_clusters.begin(), m_clusters.end(), [](const auto& cluster) { return cluster.empty();
        //        }),
        //                         m_clusters.end());
    }

    void Model::splitDisconnectedClusters() {
        for (auto& cluster : m_clusters) {
            if (not cluster.isConnected()) {
                m_clusters.push_back(cluster.getComponent());
                splitDisconnectedClusters();
            }
        }
    }

    void Model::preStep() {
        for (auto& cluster : m_clusters) {
            cluster.preStep();
        }
    }

} // namespace model