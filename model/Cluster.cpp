//
// Created by pc on 15-12-20.
//

#include "Cluster.h"

#include "../aux/Aux.h"
#include "WorldCoordinates.h"

#include <algorithm>
#include <cassert>
#include <queue>

namespace model {

    Cluster::Cluster(std::list<GridCoordinates>&& gridCoordinates, const GridCoordinates& offset)
        : m_worldOffset(0, 0), m_gridCoordinates(gridCoordinates) {
        for (auto& idx : m_gridCoordinates) {
            idx += offset;
        }
    }

    void Cluster::doAction() {
        if (m_clusterActions.empty() || not m_isAlive) {
            return;
        }
        for (auto& idx : m_gridCoordinates) {
            switch (m_clusterActions[m_clusterActionIndex].m_action) {
                case ClusterAction::ACTION::MOVE_UP:
                    idx = idx.adjacent(enums::DIRECTION::UP);
                    break;
                case ClusterAction::ACTION::MOVE_DOWN:
                    idx = idx.adjacent(enums::DIRECTION::DOWN);
                    break;
                case ClusterAction::ACTION::MOVE_LEFT:
                    idx = idx.adjacent(enums::DIRECTION::LEFT);
                    break;
                case ClusterAction::ACTION::MOVE_RIGHT:
                    idx = idx.adjacent(enums::DIRECTION::RIGHT);
                    break;
            }
        }
        clearPhase();
        m_currentPhase    = CURRENT_PHASE::TRANSLATING;
        m_fractionOfPhase = 1.0;
        switch (m_clusterActions[m_clusterActionIndex].m_action) {
            case ClusterAction::ACTION::MOVE_UP:
                m_worldOffset = {0, WorldCoordinates::m_blockSizeInWorld};
                break;
            case ClusterAction::ACTION::MOVE_DOWN:
                m_worldOffset = {0, -WorldCoordinates::m_blockSizeInWorld};
                break;
            case ClusterAction::ACTION::MOVE_LEFT:
                m_worldOffset = {WorldCoordinates::m_blockSizeInWorld, 0};
                break;
            case ClusterAction::ACTION::MOVE_RIGHT:
                m_worldOffset = {-WorldCoordinates::m_blockSizeInWorld, 0};
                break;
        }

        ++m_clusterActionIndex;
        m_clusterActionIndex %= m_clusterActions.size();
    }

    void Cluster::rotateClockWiseAbout(const GridCoordinates& pivotIndexPair) {
        for (auto& indexPair : m_gridCoordinates) {
            indexPair = {pivotIndexPair.x() + pivotIndexPair.y() - indexPair.y(), pivotIndexPair.y() - pivotIndexPair.x() + indexPair.x()};
        }
        std::transform(m_clusterActions.begin(), m_clusterActions.end(), m_clusterActions.begin(), rotateActionClockWise);
    }

    void Cluster::rotateCounterClockWiseAbout(const GridCoordinates& pivotIndexPair) {
        for (auto& indexPair : m_gridCoordinates) {
            indexPair = {pivotIndexPair.x() - pivotIndexPair.y() + indexPair.y(), pivotIndexPair.y() + pivotIndexPair.x() - indexPair.x()};
        }
        std::transform(m_clusterActions.begin(), m_clusterActions.end(), m_clusterActions.begin(), rotateActionCounterClockWise);
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

    void Cluster::removeBLock(const GridCoordinates& indexPair) {
        const auto it = std::find(m_gridCoordinates.begin(), m_gridCoordinates.end(), indexPair);
        assert(it != m_gridCoordinates.end());
        m_gridCoordinates.erase(it);
    }

    bool Cluster::empty() const {
        return m_gridCoordinates.empty();
    }

    const std::list<GridCoordinates>& Cluster::gridCoordinates() const {
        return m_gridCoordinates;
    }

    bool Cluster::intersects(const GridCoordinates& gridCoordinates) const {
        const auto it = std::find(m_gridCoordinates.begin(), m_gridCoordinates.end(), gridCoordinates);
        return it != m_gridCoordinates.end();
    }

    void Cluster::addPendingOperation(const GridCoordinates& gridCoordinates, Level::DYNAMIC_BLOCK_TYPE blockType) {
        if (blockType == Level::DYNAMIC_BLOCK_TYPE::NONE) {
            return;
        }
        m_pendingOperations.emplace_back(gridCoordinates, blockType);
    }

    void Cluster::performPendingOperation() {
        assert(m_pendingOperations.size() <= 1);
        if (m_pendingOperations.empty() || m_clusterActions.at(m_clusterActionIndex).m_modifier == ClusterAction::MODIFIER::IGNORE) {
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
                setRotation(-90.0, m_pendingOperations.front().first);
                rotateClockWiseAbout(m_pendingOperations.front().first);
                break;
            case Level::DYNAMIC_BLOCK_TYPE::ROTATE_CCW:
                setRotation(90.0, m_pendingOperations.front().first);
                rotateCounterClockWiseAbout(m_pendingOperations.front().first);
                break;
            case Level::DYNAMIC_BLOCK_TYPE::NONE:
                break;
        }
        m_pendingOperations.clear();
    }

    void Cluster::update(double fractionOfPhase) {
        if (not m_isAlive) {
            return;
        }
        m_fractionOfPhase -= fractionOfPhase;
        if (m_fractionOfPhase <= 0.0) {
            clearPhase();
        }
    }

    double Cluster::angle() const {
        return m_fractionOfPhase * m_angle;
    }

    const GridCoordinates& Cluster::rotationPivot() const {
        return m_rotationPivot;
    }

    const std::vector<ClusterAction>& Cluster::clusterActions() const {
        return m_clusterActions;
    }

    WorldVector Cluster::dynamicWorldOffset() const {
        return {static_cast<int>(m_worldOffset.x() * m_fractionOfPhase), static_cast<int>(m_worldOffset.y() * m_fractionOfPhase)};
    }

    std::set<WorldCoordinates> Cluster::cornerPoints(int shrinkInWorld) const {
        std::set<WorldCoordinates> result;
        switch (m_currentPhase) {
            case CURRENT_PHASE::NONE:
                for (const auto& it : m_gridCoordinates) {
                    for (const GridCoordinates cornerOffset :
                         {GridCoordinates{0, 0}, GridCoordinates{0, 1}, GridCoordinates{1, 1}, GridCoordinates{1, 0}}) {
                        result.emplace(WorldCoordinates::fromGridCoordinates(it + cornerOffset) +
                                       WorldCoordinates{shrinkInWorld - 2 * shrinkInWorld * cornerOffset.x(),
                                                        shrinkInWorld - 2 * shrinkInWorld * cornerOffset.y()});
                    }
                }
                break;
            case CURRENT_PHASE::TRANSLATING: {
                const WorldVector offset = dynamicWorldOffset();
                for (const auto& it : m_gridCoordinates) {
                    for (const GridCoordinates cornerOffset :
                         {GridCoordinates{0, 0}, GridCoordinates{0, 1}, GridCoordinates{1, 1}, GridCoordinates{1, 0}}) {
                        result.emplace(WorldCoordinates::fromGridCoordinates(it + cornerOffset) +
                                       WorldCoordinates{shrinkInWorld - 2 * shrinkInWorld * cornerOffset.x(),
                                                        shrinkInWorld - 2 * shrinkInWorld * cornerOffset.y()} +
                                       offset);
                    }
                }
            } break;
            case CURRENT_PHASE::ROTATING: {
                const WorldCoordinates center = WorldCoordinates::fromGridCoordinates(m_rotationPivot) + WorldCoordinates::halfBlockInWorld;
                const double           theta  = -angle();
                for (const auto& it : m_gridCoordinates) {
                    for (const GridCoordinates cornerOffset :
                         {GridCoordinates{0, 0}, GridCoordinates{0, 1}, GridCoordinates{1, 1}, GridCoordinates{1, 0}}) {
                        result.emplace(
                            aux::rotateClockWiseAboutPivot(WorldCoordinates::fromGridCoordinates(it + cornerOffset) +
                                                               WorldCoordinates{shrinkInWorld - 2 * shrinkInWorld * cornerOffset.x(),
                                                                                shrinkInWorld - 2 * shrinkInWorld * cornerOffset.y()},
                                                           center,
                                                           theta));
                    }
                }
            } break;
        }
        return result;
    }

    void Cluster::kill() {
        m_isAlive = false;
    }

    bool Cluster::isAlive() const {
        return m_isAlive;
    }

    void Cluster::clearPhase() {
        m_fractionOfPhase = 0.0;
        m_worldOffset     = {0, 0};
        m_angle           = 0.0;
        m_currentPhase    = CURRENT_PHASE::NONE;
        m_rotationPivot   = {0, 0};
    }

    void Cluster::setRotation(double angle, const GridCoordinates& pivot) {
        assert(angle != 0.0);
        clearPhase();
        m_currentPhase    = CURRENT_PHASE::ROTATING;
        m_fractionOfPhase = 1.0;
        m_angle           = angle;
        m_rotationPivot   = pivot;
    }

    void Cluster::clearActions() {
        m_clusterActions.clear();
        m_clusterActionIndex = 0;
    }

    Cluster& Cluster::operator=(const Cluster& other) {
        assert(other.m_currentPhase == CURRENT_PHASE::NONE);
        clearPhase();
        m_isAlive            = other.m_isAlive;
        m_clusterActionIndex = other.m_clusterActionIndex;
        m_gridCoordinates    = other.m_gridCoordinates;
        m_clusterActions     = other.m_clusterActions;
        m_pendingOperations.clear();
        return *this;
    }

} // namespace model