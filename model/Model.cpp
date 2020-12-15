//
// Created by pc on 15-12-20.
//

#include "Model.h"

#include <algorithm>
#include <cassert>
#include <iostream>

model::Model::Model() {
    m_clusters.emplace_back(Cluster{Matrix{{{false, true, false}, {true, true, true}}}, 3, 1});
    m_clusters.back().addAction(Cluster::Action::MOVE_UP);
    m_clusters.emplace_back(Cluster{Matrix{{{true}, {true}, {true}}}, 5, 4});
    m_clusters.back().addAction(Cluster::Action::MOVE_LEFT);
}

const std::vector<model::Cluster>& model::Model::clusters() const {
    return m_clusters;
}

void model::Model::doStep() {
    std::cout << "Step" << '\n';
    for (auto& cluster : m_clusters) {
        cluster.doStep();
    }
    for (auto& cluster : m_clusters) {
        for (auto it = cluster.matrix().begin(); not it.isDone(); it.next()) {
            switch (m_level.blockAt(IndexPair(it.get()).row() + cluster.rowOffset(),
                                    it.get().column() + cluster.columnOffset())) {
                case Level::BLOCK_TYPE::NONE:
                    break;
                case Level::BLOCK_TYPE::ROTATE_CW:
                    cluster.rotateClockWiseAbout(it.get());
                    it.forceDone();
                    break;
                case Level::BLOCK_TYPE::ROTATE_CCW:
                    cluster.rotateCounterClockWiseAbout(it.get());
                    it.forceDone();
                    break;
                case Level::BLOCK_TYPE::KILL:
                    cluster.removeBLock(it.get());
                    break;
            }
        }
    }
    m_clusters.erase(
        std::remove_if(m_clusters.begin(), m_clusters.end(), [](const Cluster& cluster) { return cluster.empty(); }),
        m_clusters.end());
}

const model::Level& model::Model::level() const {
    return m_level;
}
