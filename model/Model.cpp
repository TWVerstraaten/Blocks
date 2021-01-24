//
// Created by pc on 15-12-20.
//

#include "Model.h"

#include "../app/Application_constants.h"
#include "../misc/defines.h"

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
        while (dPhase > app::MAX_D_PHASE) {
            updateInternal(app::MAX_D_PHASE);
            dPhase -= app::MAX_D_PHASE;
        }
        updateInternal(dPhase);
    }

    void Model::init(MODEL_PRESET modelPreset) {
        clear();
        for (int i = -2; i != 15; ++i) {
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
                m_level.addBlock({i, j}, FLOOR_BLOCK_TYPE::LEVEL);
            }
        }

        switch (modelPreset) {
            case MODEL_PRESET::EMPTY:
                break;
            case MODEL_PRESET::TEST:
                for (int i = -2; i != 15; ++i) {
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
                        m_clusters.emplace_back(model::GridXy{i, j}, std::to_string(m_clusters.size()));
                    }
                }

                break;
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
                cluster.setPendingDynamicMoves(PENDING_DYNAMIC_MOVES::ZERO);
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
            for (auto& cluster : m_clusters) {
                assert(m_phaseFraction <= 1.0);
                // SmoothStep(x) = 3x^2 - 2x^2 : [0,1] -> [0,1]
                cluster.update(m_phaseFraction * m_phaseFraction * (3 - 2 * m_phaseFraction));
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

    void Model::clearEmpty() {
        m_clusters.erase(std::remove_if(D_IT(m_clusters), D_FUNC(cluster, cluster.isEmpty())), m_clusters.end());
    }

    void Model::splitDisconnectedClusters() {
        std::vector<Cluster> newClusters;
        for (auto& cluster : m_clusters) {
            if (not cluster.isConnected()) {
                D_NOTE_ONCE("Splice")
                const auto& components = cluster.collectAllButFirstComponent();
                std::copy(D_IT(components), std::back_inserter(newClusters));
                assert(cluster.isConnected());
            }
        }
        std::copy(D_IT(newClusters), std::back_inserter(m_clusters));
        for (const auto& cluster : m_clusters) {
            assert(cluster.isConnected());
        }
    }

} // namespace model
