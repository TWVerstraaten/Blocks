//
// Created by pc on 15-12-20.
//

#include "Model.h"

#include "../Io/Serializer.h"
#include "../app/Application_constants.h"
#include "../misc/defines.h"
#include "../misc/geom.h"

#include <QDebug>
#include <QElapsedTimer>
#include <cassert>

namespace model {

    const std::vector<Cluster>& Model::clusters() const {
        return m_clusters;
    }

    std::vector<Cluster>& Model::clusters() {
        return m_clusters;
    }

    void Model::intersectWithLevel() {
        for (auto& cluster : m_clusters) {
            if (cluster.isAlive()) {
                cluster.collideWithLevel(m_level, app::BLOCK_SHRINK_IN_WORLD);
            }
        }
    }

    const Level& Model::level() const {
        return m_level;
    }

    void Model::update(double dPhase) {
        assert(dPhase >= 0);
        dPhase = std::clamp(dPhase, 0.0, 1.0);
        while (dPhase > app::MAX_D_PHASE && m_phaseFraction > 0.0f) {
            updateInternal(app::MAX_D_PHASE);
            dPhase -= app::MAX_D_PHASE;
        }
        if (m_phaseFraction > 0.0f) {
            updateInternal(dPhase);
        }
    }

    void Model::init(const std::string& path) {
        using namespace io;
        std::ifstream levelFile(path);
        if (levelFile.is_open()) {
            try {
                levelFile >> *this;
            } catch (...) { initEmpty(); }
            levelFile.close();
        } else {
            initEmpty();
        }
    }

    void Model::clear() {
        m_clusters.clear();
        m_level.clear();
    }

    void Model::resetPhase() {
        m_phaseFraction = 1.0;
        for (auto& cluster : m_clusters) {
            if (cluster.isAlive()) {
                cluster.resetPhase();
            }
        }
    }

    void Model::updateInternal(double dPhase) {
        assert(dPhase <= app::MAX_D_PHASE);
        assert(dPhase >= 0);
        if (m_phaseFraction == 0.0) {
            return;
        }
        m_phaseFraction -= dPhase;
        if (m_phaseFraction <= 0.0) {
            m_phaseFraction = 0.0;
        } else {
            const auto smoothStep = m_phaseFraction;
            for (auto& cluster : m_clusters) {
                assert(m_phaseFraction <= 1.0);
                cluster.update(smoothStep);
            }

            intersectWithLevel();
            intersectClusters();
        }
    }

    void Model::intersectClusters() {
        if (m_clusters.size() == 1) {
            return;
        }
        for (auto& cluster : m_clusters) {
            cluster.buildBoundingAlignedRectangle();
        }
        for (auto cluster1 = m_clusters.begin(); cluster1 != m_clusters.end(); ++cluster1) {
            for (auto cluster2 = std::next(cluster1); cluster2 != m_clusters.end(); ++cluster2) {
                if (cluster1->intersects(*cluster2, app::BLOCK_SHRINK_IN_WORLD)) {
                    cluster1->kill();
                    cluster2->kill();
                }
            }
        }
    }

    std::vector<Cluster>::iterator Model::clusterWithIndex(size_t index) {
        return std::find_if(D_IT(m_clusters), D_FUNC(cluster, cluster.index() == index));
    }

    Level& Model::level() {
        return m_level;
    }

    bool Model::noLiveOrStoppedClusterOnBlock(const GridXy& gridXy) const {
        return (std::find_if(D_IT(m_clusters), D_FUNC(cluster, cluster.contains(gridXy))) == m_clusters.end()) &&
               (std::find_if(D_IT(m_level.stoppedClusters()), D_FUNC(cluster, cluster.contains(gridXy))) == m_level.stoppedClusters().end());
    }

    std::vector<Cluster>::iterator Model::clusterContaining(const GridXy& point) {
        return std::find_if(D_IT(m_clusters), D_FUNC(cluster, cluster.contains(point)));
    }

    std::vector<Cluster>::iterator Model::stoppedClusterContaining(const GridXy& point) {
        return std::find_if(D_IT(m_level.stoppedClusters()), D_FUNC(cluster, cluster.contains(point)));
    }

    void Model::clearEmpty() {
        m_clusters.erase(std::remove_if(D_IT(m_clusters), D_FUNC(cluster, cluster.isEmpty())), m_clusters.end());
        m_level.stoppedClusters().erase(std::remove_if(D_IT(m_level.stoppedClusters()), D_FUNC(cluster, cluster.isEmpty())),
                                        m_level.stoppedClusters().end());
    }

    void Model::splitDisconnectedClusters() {
        geom::splitDisconnectedClusters(m_clusters);
    }

    void Model::clearStopped() {
        m_clusters.erase(std::remove_if(D_IT(m_clusters), D_FUNC(cluster, cluster.state() == CLUSTER_STATE::STOPPED)), m_clusters.end());
    }

    void Model::handleStoppedAndSplitClusters() {
        std::copy_if(D_CIT(m_clusters), std::back_inserter(m_level.stoppedClusters()), D_FUNC(cluster, cluster.state() == CLUSTER_STATE::STOPPED));
        clearEmpty();
        clearStopped();
        splitDisconnectedClusters();
    }

    void Model::initEmpty() {
        clear();
        for (int i = 0; i != 10; ++i) {
            for (int j = 0; j != 10; ++j) {
                m_level.addBlock(GridXy{i, j}, FLOOR_BLOCK_TYPE::LEVEL);
            }
        }
    }
} // namespace model
