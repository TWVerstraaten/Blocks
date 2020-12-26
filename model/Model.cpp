//
// Created by pc on 15-12-20.
//

#include "Model.h"

#include "../global/fns.h"

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

    void Model::intersectWithLevel() {
        for (auto& cluster : m_clusters) {
            if (not cluster.isAlive()) {
                continue;
            }
            cluster.collideWithLevel(m_level, cst::BLOCK_SHRINK_IN_WORLD);
        }
    }

    const model::Level& Model::level() const {
        return m_level;
    }

    void Model::update(double dPhase) {
        assert(dPhase >= 0);
        if (dPhase >= 1.0) {
            dPhase = 1.0;
        }
        //        assert(dPhase <= 1);
        while (dPhase > cst::MAX_D_PHASE) {
            updateInternal(cst::MAX_D_PHASE);
            dPhase -= cst::MAX_D_PHASE;
        }
        updateInternal(dPhase);
    }

    void Model::init() {
        clear();
        m_clusters.push_back(Cluster({{5, 5}, {3, 5}, {5, 6}, {6, 5}, {4, 5}, {5, 4}}, "CL" + std::to_string(m_clusters.size())));
        m_clusters.back().addAction({Action::VALUE::MOVE_UP, Action::MODIFIER::NONE});

        m_level.addBlock({5, 3}, Level::INSTANT_BLOCK_TYPE::KILL);

        for (int i = -2; i != 15; ++i) {
            if (i > 5) {
                m_level.addLevelBlock({i, -3});
            }
            for (int j = -2; j != 11; ++j) {
                if (i == 11 && j == 3) {
                    continue;
                }
                if (i == 10 && j == 3) {
                    continue;
                }
                if (i == 11 && j == 4) {
                    continue;
                }
                m_level.addLevelBlock({i, j});
            }
        }
        for (int i = -2; i != 9; ++i) {
            for (int j = -2; j != 11; ++j) {
                if (i == 11 && j == 3) {
                    continue;
                }
                m_level.addStartBlock({i, j});
            }
        }
        m_level.sort();
        m_level.createBoundaries();
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
            m_clusters.push_back(m_clusters[i].grabSecondComponent());
            if (m_clusters.back().empty()) {
                m_clusters.erase(m_clusters.end());
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
            baseIt->gridXY().merge(extensionIt->gridXY());
            assert(extensionIt->empty());
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
        if (m_needsPreStep) {
            preStep();
        } else {
            m_needsPreStep = true;
        }
        m_phaseFraction = 1.0;
    }

    void Model::updateInternal(double dPhase) {
        assert(dPhase <= cst::MAX_D_PHASE);
        assert(dPhase >= 0);
        if (m_phaseFraction == 0.0) {
            return;
        }
        m_phaseFraction -= dPhase;
        if (m_phaseFraction <= 0.0 && m_needsPreStep) {
            m_phaseFraction = 0.0;
            preStep();
            m_needsPreStep = false;
        }
        for (auto& cluster : m_clusters) {
            cluster.update(m_phaseFraction);
        }
        intersectWithLevel();
        intersectClusters();
    }

    void Model::finishInteractions() {
    }

    void Model::intersectClusters() {
        if (m_clusters.size() == 1) {
            return;
        }
        for (size_t i = 0; i + 1 != m_clusters.size(); ++i) {
            for (size_t j = i + 1; j != m_clusters.size(); ++j) {
                if (not(m_clusters.at(i).isAlive() || m_clusters.at(j).isAlive())) {
                    continue;
                }

                if (m_clusters.at(i).intersects(m_clusters.at(j), cst::BLOCK_SHRINK_IN_WORLD)) {
                    m_clusters[i].kill();
                    m_clusters[j].kill();
                }
            }
        }
    }

} // namespace model