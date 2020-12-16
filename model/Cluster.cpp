//
// Created by pc on 15-12-20.
//

#include "Cluster.h"

#include <algorithm>
#include <cassert>
#include <queue>

namespace model {

    Cluster::Cluster(Matrix&& matrix, size_t rowOffset, size_t columnOffset)
        : m_rowOffset(rowOffset), m_columnOffset(columnOffset) {
        for (size_t i = 0; i != matrix.rowCount(); ++i) {
            for (size_t j = 0; j != matrix.columnCount(); ++j) {
                if (matrix.at(i, j)) {
                    m_indexPairs.emplace(i, j);
                }
            }
        }
    }

    void Cluster::doStep() {
        doAction();
    }

    int Cluster::rowOffset() const {
        return m_rowOffset;
    }

    int Cluster::columnOffset() const {
        return m_columnOffset;
    }

    void Cluster::doAction() {
        if (m_actions.empty()) {
            return;
        }
        switch (m_actions[m_actionIndex]) {
            case Action::MOVE_UP:
                m_rowOffset -= 1;
                break;
            case Action::MOVE_DOWN:
                m_rowOffset += 1;
                break;
            case Action::MOVE_LEFT:
                m_columnOffset -= 1;
                break;
            case Action::MOVE_RIGHT:
                m_columnOffset += 1;
                break;
        }
        ++m_actionIndex;
        m_actionIndex %= m_actions.size();
    }

    void Cluster::rotateClockWiseAbout(const IndexPair& pivotIndexPair) {
        std::set<IndexPair> rotatedIndexPairs;
        for (auto m_indexPair : m_indexPairs) {
            rotatedIndexPairs.emplace(pivotIndexPair.row() - pivotIndexPair.column() + m_indexPair.column(),
                                      pivotIndexPair.column() + pivotIndexPair.row() - m_indexPair.row());
        }
        assert(m_indexPairs.size() == rotatedIndexPairs.size());
        std::swap(rotatedIndexPairs, m_indexPairs);
        std::transform(m_actions.begin(), m_actions.end(), m_actions.begin(), rotateActionClockWise);
    }

    void Cluster::rotateCounterClockWiseAbout(const IndexPair& pivotIndexPair) {
        std::set<IndexPair> rotatedIndexPairs;
        for (auto m_indexPair : m_indexPairs) {
            rotatedIndexPairs.emplace(pivotIndexPair.row() + pivotIndexPair.column() - m_indexPair.column(),
                                      pivotIndexPair.column() - pivotIndexPair.row() + m_indexPair.row());
        }
        assert(m_indexPairs.size() == rotatedIndexPairs.size());
        std::swap(rotatedIndexPairs, m_indexPairs);
        std::transform(m_actions.begin(), m_actions.end(), m_actions.begin(), rotateActionCounterClockWise);
    }

    Cluster::Action Cluster::rotateActionClockWise(Cluster::Action action) {
        switch (action) {
            case Action::MOVE_UP:
                return Action::MOVE_RIGHT;
            case Action::MOVE_DOWN:
                return Action::MOVE_LEFT;
            case Action::MOVE_LEFT:
                return Action::MOVE_UP;
            case Action::MOVE_RIGHT:
                return Action::MOVE_DOWN;
            default:
                return action;
        }
    }

    Cluster::Action Cluster::rotateActionCounterClockWise(Cluster::Action action) {
        switch (action) {
            case Action::MOVE_UP:
                return Action::MOVE_LEFT;
            case Action::MOVE_DOWN:
                return Action::MOVE_RIGHT;
            case Action::MOVE_LEFT:
                return Action::MOVE_DOWN;
            case Action::MOVE_RIGHT:
                return Action::MOVE_UP;
            default:
                return action;
        }
    }

    void Cluster::addAction(Cluster::Action action) {
        m_actions.push_back(action);
    }

    void Cluster::removeBLock(const IndexPair& indexPair) {
        assert(m_indexPairs.find(indexPair) != m_indexPairs.end());
        m_indexPairs.erase(m_indexPairs.find(indexPair));
    }

    bool Cluster::empty() const {
        return m_indexPairs.empty();
    }

    const std::set<IndexPair>& Cluster::indexPairs() const {
        return m_indexPairs;
    }

    bool Cluster::intersects(const IndexPair& indexPair) const {
        return m_indexPairs.find(indexPair) != m_indexPairs.end();
    }

    enums::DIRECTION Cluster::adjacent(const IndexPair& indexPair) const {
        for (auto dir :
             {enums::DIRECTION::UP, enums::DIRECTION::DOWN, enums::DIRECTION::LEFT, enums::DIRECTION::RIGHT}) {
            if (intersects(indexPair.adjacent(dir))) {
                return dir;
            }
        }
        return enums::DIRECTION::NONE;
    }

} // namespace model