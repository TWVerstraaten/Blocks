//
// Created by pc on 15-12-20.
//

#include "Model.h"

#include "../action/AddBlockToClusterAction.h"
#include "../action/GenericModelAction.h"
#include "../action/RemoveBlockFromClusterAction.h"
#include "../action/RemoveClusterAction.h"
#include "../global/fns.h"

#include <algorithm>

namespace model {

    Model::Model() {
    }

    const std::list<Cluster>& Model::clusters() const {
        return m_clusters;
    }

    std::list<Cluster>& Model::clusters() {
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
            cluster.performPendingOperationOrNextCommand();
        }
    }

    void Model::interactClustersWithInstantBlocks() {
        for (const auto& block : m_level.instantBlocks()) {
            switch (block.second) {
                case model::INSTANT_BLOCK_TYPE::NONE:
                    break;
                case model::INSTANT_BLOCK_TYPE::KILL:
                    clearBlock(block.first);
                    break;
            }
        }
        assert(not containsEmptyClusters());
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
        m_clusters.push_back(
            Cluster({{5, 5}, {3, 5}, {5, 6}, {6, 5}, {4, 5}, {5, 4}}, "CL" + std::to_string(m_clusters.size())));
        m_clusters.back().addCommand({Command::TYPE::MOVE_UP, Command::MODIFIER::NONE});

        m_level.addBlock({5, 3}, model::INSTANT_BLOCK_TYPE::KILL);

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
        m_level.createBoundaries();
    }

    void Model::clear() {
        m_clusters.clear();
        m_level.clear();
    }

    void Model::clearEmptyClusters() {
        m_clusters.erase(
            std::remove_if(m_clusters.begin(), m_clusters.end(), [](const auto& cluster) { return cluster.empty(); }),
            m_clusters.end());
    }

    void Model::preStep() {
        m_phaseFraction = 0.0;
        for (auto& cluster : m_clusters) {
            cluster.preStep();
        }
        m_needsPreStep = false;
    }

    std::unique_ptr<action::Action> Model::addCluster(const GridXY& gridXY) {
        assert(m_level.isFreeStartBlock(gridXY));
        if (std::find_if(m_clusters.begin(), m_clusters.end(), [&](const auto& cluster) {
                return cluster.contains(gridXY);
            }) == m_clusters.end()) {
            Model copy = *this;
            m_clusters.push_back(Cluster({gridXY}, "CL" + std::to_string(m_clusters.size())));
            return std::make_unique<action::AddClusterAction>(m_clusters.back());
        }
        return nullptr;
    }

    std::unique_ptr<action::Action> Model::linkBlocks(const GridXY& base, const GridXY& extension) {
        const auto baseIt = std::find_if(
            m_clusters.begin(), m_clusters.end(), [&](const auto& cluster) { return cluster.contains(base); });
        if (baseIt == m_clusters.end() || (not base.isAdjacent(extension))) {
            return addCluster(extension);
        }
        auto extensionIt = std::find_if(
            m_clusters.begin(), m_clusters.end(), [&](const auto& cluster) { return cluster.contains(extension); });
        if (baseIt == extensionIt) {
            return nullptr;
        }
        if (extensionIt == m_clusters.end()) {
            baseIt->addGridXY(extension);
            assert(baseIt->isConnected());
            return std::unique_ptr<action::Action>(new action::AddBlockToClusterAction(baseIt->index(), extension));
        }
        Model copyOfModel = *this;
        baseIt->gridXY().insert(extensionIt->gridXY().begin(), extensionIt->gridXY().end());
        m_clusters.erase(extensionIt);
        return std::unique_ptr<action::Action>(new action::GenericModelAction(copyOfModel, *this));
    }

    std::unique_ptr<action::Action> Model::clearBlock(const GridXY& gridXY) {
        const auto it = std::find_if(
            m_clusters.begin(), m_clusters.end(), [&](const auto& cluster) { return cluster.contains(gridXY); });
        if (it == m_clusters.end()) {
            return nullptr;
        }
        if (it->size() == 1) {
            auto result = std::unique_ptr<action::Action>(new action::RemoveClusterAction(*it));
            m_clusters.erase(it);
            return result;
        }
        Model copy = *this;
        it->removeBLock(gridXY);
        if (not it->isConnected()) {
            m_clusters.splice(m_clusters.end(), it->collectAllButFirstComponent());
            assert(it->isConnected());
            return std::unique_ptr<action::Action>(new action::GenericModelAction(copy, *this));
        }
        return std::unique_ptr<action::Action>(new action::RemoveBlockFromClusterAction(it->index(), gridXY));
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

    bool Model::containsEmptyClusters() const {
        return std::any_of(
            m_clusters.begin(), m_clusters.end(), [](const Cluster& cluster) { return cluster.empty(); });
    }

    std::list<Cluster>::iterator Model::clusterWithIndex(size_t index) {
        return std::find_if(
            m_clusters.begin(), m_clusters.end(), [&](const Cluster& cluster) { return cluster.index() == index; });
    }

} // namespace model