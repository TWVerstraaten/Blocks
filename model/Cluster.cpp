//
// Created by pc on 15-12-20.
//

#include "Cluster.h"

#include <algorithm>
#include <cassert>
#include <queue>

namespace model {

    Cluster::Cluster(std::set<IndexPair>&& indexPairs, const IndexPair& offset)
        : m_offset(offset), m_previousOffset(m_offset), m_localIndexPairs(indexPairs) {
    }

    int Cluster::rowOffset() const {
        return m_offset.row();
    }

    int Cluster::columnOffset() const {
        return m_offset.column();
    }

    void Cluster::doAction() {
        if (m_actions.empty()) {
            return;
        }
        m_previousOffset  = m_offset;
        m_fractionOfPhase = 1.0;
        switch (m_actions[m_actionIndex]) {
            case Action::MOVE_UP:
                m_offset = m_offset.adjacent(enums::DIRECTION::UP);
                break;
            case Action::MOVE_DOWN:
                m_offset = m_offset.adjacent(enums::DIRECTION::DOWN);
                break;
            case Action::MOVE_LEFT:
                m_offset = m_offset.adjacent(enums::DIRECTION::LEFT);
                break;
            case Action::MOVE_RIGHT:
                m_offset = m_offset.adjacent(enums::DIRECTION::RIGHT);
                break;
        }
        ++m_actionIndex;
        m_actionIndex %= m_actions.size();
    }

    void Cluster::rotateClockWiseAbout(const IndexPair& pivotIndexPair) {
        std::set<IndexPair> rotatedIndexPairs;
        for (auto& m_indexPair : m_localIndexPairs) {
            rotatedIndexPairs.emplace(pivotIndexPair.row() - pivotIndexPair.column() + m_indexPair.column(),
                                      pivotIndexPair.column() + pivotIndexPair.row() - m_indexPair.row());
        }
        assert(m_localIndexPairs.size() == rotatedIndexPairs.size());
        std::swap(rotatedIndexPairs, m_localIndexPairs);
        std::transform(m_actions.begin(), m_actions.end(), m_actions.begin(), rotateActionClockWise);
    }

    void Cluster::rotateCounterClockWiseAbout(const IndexPair& pivotIndexPair) {
        std::set<IndexPair> rotatedIndexPairs;
        for (auto m_indexPair : m_localIndexPairs) {
            rotatedIndexPairs.emplace(pivotIndexPair.row() + pivotIndexPair.column() - m_indexPair.column(),
                                      pivotIndexPair.column() - pivotIndexPair.row() + m_indexPair.row());
        }
        assert(m_localIndexPairs.size() == rotatedIndexPairs.size());
        std::swap(rotatedIndexPairs, m_localIndexPairs);
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
        assert(m_localIndexPairs.find(indexPair) != m_localIndexPairs.end());
        m_localIndexPairs.erase(m_localIndexPairs.find(indexPair));
    }

    bool Cluster::empty() const {
        return m_localIndexPairs.empty();
    }

    const std::set<IndexPair>& Cluster::localIndexPairs() const {
        return m_localIndexPairs;
    }

    bool Cluster::intersects(const IndexPair& indexPair) const {
        return m_localIndexPairs.find(indexPair) != m_localIndexPairs.end();
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

    void Cluster::addPendingOperation(const IndexPair& indexPair, Level::DYNAMIC_BLOCK_TYPE blockType) {
        if (blockType == Level::DYNAMIC_BLOCK_TYPE::NONE) {
            return;
        }
        m_pendingOperations.emplace_back(indexPair, blockType);
    }

    void Cluster::performPendingOperation() {
        assert(m_pendingOperations.size() <= 1);
        if (m_pendingOperations.empty()) {
            return;
        }
        switch (m_pendingOperations.front().second) {
            case Level::DYNAMIC_BLOCK_TYPE::ROTATE_CW:
                rotateClockWiseAbout(m_pendingOperations.front().first);
                break;
            case Level::DYNAMIC_BLOCK_TYPE::ROTATE_CCW:
                rotateCounterClockWiseAbout(m_pendingOperations.front().first);
                break;
            case Level::DYNAMIC_BLOCK_TYPE::NONE:
                break;
        }

        m_pendingOperations.clear();
    }

    void Cluster::update(double fractionOfPhase) {
        m_fractionOfPhase -= fractionOfPhase;
        if (m_fractionOfPhase <= 0.0) {
            m_fractionOfPhase = 0.0;
            m_previousOffset  = m_offset;
        }
    }

    double Cluster::dynamicRowOffset() const {
        return m_offset.row() + m_fractionOfPhase * (-m_offset.row() + m_previousOffset.row());
    }

    double Cluster::dynamicColumnOffset() const {
        return m_offset.column() + m_fractionOfPhase * (-m_offset.column() + m_previousOffset.column());
    }

} // namespace model