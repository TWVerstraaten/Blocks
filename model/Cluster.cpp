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
        if (m_clusterActions.empty()) {
            return;
        }
        m_previousOffset  = m_offset;
        m_fractionOfPhase = 1.0;
        switch (m_clusterActions[m_clusterActionIndex].m_action) {
            case ClusterAction::ACTION::MOVE_UP:
                m_offset = m_offset.adjacent(enums::DIRECTION::UP);
                break;
            case ClusterAction::ACTION::MOVE_DOWN:
                m_offset = m_offset.adjacent(enums::DIRECTION::DOWN);
                break;
            case ClusterAction::ACTION::MOVE_LEFT:
                m_offset = m_offset.adjacent(enums::DIRECTION::LEFT);
                break;
            case ClusterAction::ACTION::MOVE_RIGHT:
                m_offset = m_offset.adjacent(enums::DIRECTION::RIGHT);
                break;
        }
        ++m_clusterActionIndex;
        m_clusterActionIndex %= m_clusterActions.size();
    }

    void Cluster::rotateClockWiseAbout(const IndexPair& pivotIndexPair) {
        std::set<IndexPair> rotatedIndexPairs;
        for (auto& m_indexPair : m_localIndexPairs) {
            rotatedIndexPairs.emplace(pivotIndexPair.row() - pivotIndexPair.column() + m_indexPair.column(),
                                      pivotIndexPair.column() + pivotIndexPair.row() - m_indexPair.row());
        }
        assert(m_localIndexPairs.size() == rotatedIndexPairs.size());
        std::swap(rotatedIndexPairs, m_localIndexPairs);
        std::transform(m_clusterActions.begin(), m_clusterActions.end(), m_clusterActions.begin(),
                       rotateActionClockWise);
    }

    void Cluster::rotateCounterClockWiseAbout(const IndexPair& pivotIndexPair) {
        std::set<IndexPair> rotatedIndexPairs;
        for (auto m_indexPair : m_localIndexPairs) {
            rotatedIndexPairs.emplace(pivotIndexPair.row() + pivotIndexPair.column() - m_indexPair.column(),
                                      pivotIndexPair.column() - pivotIndexPair.row() + m_indexPair.row());
        }
        assert(m_localIndexPairs.size() == rotatedIndexPairs.size());
        std::swap(rotatedIndexPairs, m_localIndexPairs);
        std::transform(m_clusterActions.begin(), m_clusterActions.end(), m_clusterActions.begin(),
                       rotateActionCounterClockWise);
    }

    ClusterAction Cluster::rotateActionClockWise(ClusterAction action) {
        switch (action.m_action) {
            case ClusterAction::ACTION::MOVE_UP:
                return {ClusterAction::ACTION::MOVE_RIGHT, action.m_modifier};
            case ClusterAction::ACTION::MOVE_DOWN:
                return {ClusterAction::ACTION::MOVE_LEFT, action.m_modifier};
            case ClusterAction::ACTION::MOVE_LEFT:
                return {ClusterAction::ACTION::MOVE_UP, action.m_modifier};
            case ClusterAction::ACTION::MOVE_RIGHT:
                return {ClusterAction::ACTION::MOVE_DOWN, action.m_modifier};
            default:
                return action;
        }
    }

    ClusterAction Cluster::rotateActionCounterClockWise(ClusterAction action) {
        switch (action.m_action) {
            case ClusterAction::ACTION::MOVE_UP:
                return {ClusterAction::ACTION::MOVE_LEFT, action.m_modifier};
            case ClusterAction::ACTION::MOVE_DOWN:
                return {ClusterAction::ACTION::MOVE_RIGHT, action.m_modifier};
            case ClusterAction::ACTION::MOVE_LEFT:
                return {ClusterAction::ACTION::MOVE_DOWN, action.m_modifier};
            case ClusterAction::ACTION::MOVE_RIGHT:
                return {ClusterAction::ACTION::MOVE_UP, action.m_modifier};
            default:
                return action;
        }
    }

    void Cluster::addAction(ClusterAction action) {
        m_clusterActions.push_back(action);
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
        if (m_pendingOperations.empty() ||
            m_clusterActions.at(m_clusterActionIndex).m_modifier == ClusterAction::MODIFIER::IGNORE) {
            doAction();
            m_pendingOperations.clear();
            return;
        }
        if (m_clusterActions.at(m_clusterActionIndex).m_modifier == ClusterAction::MODIFIER::SKIP) {
            ++m_clusterActionIndex;
            m_clusterActionIndex %= m_clusterActions.size();
        }
        switch (m_pendingOperations.front().second) {
            case Level::DYNAMIC_BLOCK_TYPE::ROTATE_CW:
                m_fractionOfPhase = 1.0;
                m_angle           = -90.0;
                m_rotationPivot   = m_pendingOperations.front().first;
                rotateClockWiseAbout(m_pendingOperations.front().first);
                break;
            case Level::DYNAMIC_BLOCK_TYPE::ROTATE_CCW:
                m_fractionOfPhase = 1.0;
                m_angle           = 90.0;
                m_rotationPivot   = m_pendingOperations.front().first;
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
            m_angle           = 0.0;
        }
    }

    double Cluster::dynamicRowOffset() const {
        return m_offset.row() + m_fractionOfPhase * (-m_offset.row() + m_previousOffset.row());
    }

    double Cluster::dynamicColumnOffset() const {
        return m_offset.column() + m_fractionOfPhase * (-m_offset.column() + m_previousOffset.column());
    }

    double Cluster::angle() const {
        return m_fractionOfPhase * m_angle;
    }

    const IndexPair& Cluster::rotationPivot() const {
        return m_rotationPivot;
    }

    const std::vector<ClusterAction>& Cluster::clusterActions() const {
        return m_clusterActions;
    }

} // namespace model