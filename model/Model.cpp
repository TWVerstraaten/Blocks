//
// Created by pc on 15-12-20.
//

#include "Model.h"

#include "../action/RemoveClusterAction.h"
#include "../global/cst.h"
#include "../global/fns.h"

#include <algorithm>

namespace model {

    Model::Model() = default;

    const std::list<Cluster>& Model::clusters() const {
        return m_clusters;
    }

    std::list<Cluster>& Model::clusters() {
        return m_clusters;
    }

    void Model::intersectWithLevel() {
        for (auto& cluster : m_clusters) {
            if (not cluster.isAlive()) {
                continue;
            }
            cluster.collideWithLevel(m_level, cst::BLOCK_SHRINK_IN_WORLD);
        }
    }

    const Level& Model::level() const {
        return m_level;
    }

    void Model::update(double dPhase) {
        assert(dPhase >= 0);
        if (dPhase >= 1.0) {
            dPhase = 1.0;
        }
        assert(dPhase <= 1);
        while (dPhase > cst::MAX_D_PHASE) {
            updateInternal(cst::MAX_D_PHASE);
            dPhase -= cst::MAX_D_PHASE;
        }
        updateInternal(dPhase);
    }

    void Model::init() {
        clear();

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
        for (int i = -2; i != 4; ++i) {
            for (int j = -2; j != 3; ++j) {
                m_level.addStartBlock({i, j});
            }
        }
        m_level.createBoundaries();
    }

    void Model::clear() {
        m_clusters.clear();
        m_level.clear();
    }

    void Model::preStep() {
        m_phaseFraction = 0.0;
        for (auto& cluster : m_clusters) {
            cluster.preStep();
        }
        m_needsPreStep = false;
    }

    Model& Model::operator=(const Model& other) {
        m_level    = other.m_level;
        m_clusters = other.m_clusters;
        return *this;
    }

    void Model::startPhase() {
        if (m_needsPreStep) {
            preStep();
        }
        m_needsPreStep  = true;
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

        for (auto cluster1 = m_clusters.begin(); cluster1 != m_clusters.end(); ++cluster1) {
            for (auto cluster2 = std::next(cluster1); cluster2 != m_clusters.end(); ++cluster2) {
                if (not(cluster1->isAlive() || cluster2->isAlive())) {
                    continue;
                }
                if (cluster1->intersects(*cluster2, cst::BLOCK_SHRINK_IN_WORLD)) {
                    cluster1->kill();
                    cluster2->kill();
                }
            }
        }
    }

    std::list<Cluster>::iterator Model::clusterWithIndex(size_t index) {
        return std::find_if(m_clusters.begin(), m_clusters.end(), [&](const Cluster& cluster) { return cluster.index() == index; });
    }

    Level& Model::level() {
        return m_level;
    }

    bool Model::noClusterOnBlock(const GridXY& gridXY) const {
        return std::find_if(m_clusters.begin(), m_clusters.end(), [&](const Cluster& cluster) { return cluster.contains(gridXY); }) ==
               m_clusters.end();
    }

    std::list<Cluster>::iterator Model::clusterContaining(const GridXY& point) {
        return std::find_if(m_clusters.begin(), m_clusters.end(), [&](const auto& cluster) { return cluster.contains(point); });
    }

    double Model::phaseFraction() const {
        return m_phaseFraction;
    }

} // namespace model