//
// Created by pc on 15-12-20.
//

#include "Model.h"

#include "../global/Global.h"

#include <algorithm>
#include <cassert>

namespace model {

    Model::Model() {
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

    void Model::update(double dPhase) {
        m_phaseFraction -= dPhase;
        if (m_phaseFraction <= 0.0 && m_needsPreStep) {
            m_phaseFraction = 0.0;
            preStep();
            m_needsPreStep = false;
        }
        for (auto& cluster : m_clusters) {
            cluster.update(m_phaseFraction);
        }
    }

    void Model::init() {
        clear();

        m_clusters.push_back(Cluster({{4, 5}, {4, 6}, {5, 6}, {6, 6}, {7, 6}, {8, 6}, {8, 5}}, "CL" + std::to_string(m_clusters.size())));
        m_clusters.back().addAction({Action::VALUE::MOVE_UP, Action::MODIFIER::NONE});

        m_level.addBlock({7, 5}, Level::DYNAMIC_BLOCK_TYPE::ROTATE_CCW);

        for (int i = -2; i != 15; ++i) {
            for (int j = -2; j != 11; ++j) {
                if (i == 11 && j == 3) {
                    continue;
                }
                m_level.addLevelBlock({i, j});
            }
        }
        for (int i = -2; i != 4; ++i) {
            for (int j = -2; j != 11; ++j) {
                if (i == 11 && j == 3) {
                    continue;
                }
                m_level.addStartBlock({i, j});
            }
        }
    }

    void Model::clear() {
        m_clusters.clear();
        m_level.clear();
    }

    void Model::clearEmptyClusters() {
        m_clusters.erase(std::remove_if(m_clusters.begin(), m_clusters.end(), [](const auto& cluster) { return cluster.empty(); }),
                         m_clusters.end());
    }

    void Model::splitDisconnectedClusters() {
        for (size_t i = 0; i != m_clusters.size(); ++i) {
            while (not m_clusters[i].isConnected()) {
                m_clusters.push_back(m_clusters[i].getComponent());
            }
        }
    }

    void Model::preStep() {
        m_phaseFraction = 0.0;
        for (auto& cluster : m_clusters) {
            cluster.preStep();
        }
        m_needsPreStep = false;
    }

    void Model::addBlock(const GridXY& gridXY) {
        assert(m_level.isFreeStartBlock(gridXY));
        if (std::find_if(m_clusters.begin(), m_clusters.end(), [&](const auto& cluster) { return cluster.contains(gridXY); }) ==
            m_clusters.end()) {
            m_clusters.push_back(Cluster({gridXY}, "CL" + std::to_string(m_clusters.size())));
        }
    }

    void Model::linkBlocks(const GridXY& base, const GridXY& extension) {
        const auto baseIt = std::find_if(m_clusters.begin(), m_clusters.end(), [&](const auto& cluster) { return cluster.contains(base); });
        if (baseIt == m_clusters.end() || (not base.isAdjacent(extension))) {
            addBlock(extension);
            return;
        }
        auto extensionIt =
            std::find_if(m_clusters.begin(), m_clusters.end(), [&](const auto& cluster) { return cluster.contains(extension); });
        if (baseIt == extensionIt) {
            return;
        } else if (extensionIt == m_clusters.end()) {
            baseIt->addGridXY(extension);
            assert(baseIt->isConnected());
        } else {
            std::move(extensionIt->gridXY().begin(), extensionIt->gridXY().end(), std::back_inserter(baseIt->gridXY())); // ##
            extensionIt->gridXY().erase(extensionIt->gridXY().begin(), extensionIt->gridXY().end());
            clearEmptyClusters();
        }
    }

    void Model::clearBlock(const GridXY& gridXY) {
        const auto it = std::find_if(m_clusters.begin(), m_clusters.end(), [&](const auto& cluster) { return cluster.contains(gridXY); });
        if (it == m_clusters.end()) {
            return;
        }
        it->removeBLock(gridXY);
        clearEmptyClusters();
        splitDisconnectedClusters();
    }

    Model& Model::operator=(const Model& other) {
        m_level    = other.m_level;
        m_clusters = other.m_clusters;
        return *this;
    }

    void Model::startPhase() {
        m_phaseFraction = 1.0;
        m_needsPreStep  = true;
    }

} // namespace model