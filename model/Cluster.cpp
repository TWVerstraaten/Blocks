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
        if (m_actions.empty() || not m_isAlive) {
            return;
        }
        for (auto& idx : m_gridCoordinates) {
            switch (m_actions[m_actionIndex].m_action) {
                case Action::VALUE::MOVE_UP:
                    idx = idx.adjacent(enums::DIRECTION::UP);
                    break;
                case Action::VALUE::MOVE_DOWN:
                    idx = idx.adjacent(enums::DIRECTION::DOWN);
                    break;
                case Action::VALUE::MOVE_LEFT:
                    idx = idx.adjacent(enums::DIRECTION::LEFT);
                    break;
                case Action::VALUE::MOVE_RIGHT:
                    idx = idx.adjacent(enums::DIRECTION::RIGHT);
                    break;
            }
        }
        clearPhase();
        m_currentPhase    = CURRENT_PHASE::TRANSLATING;
        m_fractionOfPhase = 1.0;
        switch (m_actions[m_actionIndex].m_action) {
            case Action::VALUE::MOVE_UP:
                m_worldOffset = {0, WorldCoordinates::m_blockSizeInWorld};
                break;
            case Action::VALUE::MOVE_DOWN:
                m_worldOffset = {0, -WorldCoordinates::m_blockSizeInWorld};
                break;
            case Action::VALUE::MOVE_LEFT:
                m_worldOffset = {WorldCoordinates::m_blockSizeInWorld, 0};
                break;
            case Action::VALUE::MOVE_RIGHT:
                m_worldOffset = {-WorldCoordinates::m_blockSizeInWorld, 0};
                break;
        }

        ++m_actionIndex;
        m_actionIndex %= m_actions.size();
    }

    void Cluster::rotateClockWiseAbout(const GridCoordinates& pivotGridCoordinates) {
        for (auto& gridCoordinates : m_gridCoordinates) {
            gridCoordinates = {pivotGridCoordinates.x() + pivotGridCoordinates.y() - gridCoordinates.y(),
                               pivotGridCoordinates.y() - pivotGridCoordinates.x() + gridCoordinates.x()};
        }
        std::transform(m_actions.begin(), m_actions.end(), m_actions.begin(), rotateActionClockWise);
    }

    void Cluster::rotateCounterClockWiseAbout(const GridCoordinates& pivotGridCoordinates) {
        for (auto& gridCoordinates : m_gridCoordinates) {
            gridCoordinates = {pivotGridCoordinates.x() - pivotGridCoordinates.y() + gridCoordinates.y(),
                               pivotGridCoordinates.y() + pivotGridCoordinates.x() - gridCoordinates.x()};
        }
        std::transform(m_actions.begin(), m_actions.end(), m_actions.begin(), rotateActionCounterClockWise);
    }

    Action Cluster::rotateActionClockWise(Action action) {
        switch (action.m_action) {
            case Action::VALUE::MOVE_UP:
                return {Action::VALUE::MOVE_RIGHT, action.m_modifier};
            case Action::VALUE::MOVE_DOWN:
                return {Action::VALUE::MOVE_LEFT, action.m_modifier};
            case Action::VALUE::MOVE_LEFT:
                return {Action::VALUE::MOVE_UP, action.m_modifier};
            case Action::VALUE::MOVE_RIGHT:
                return {Action::VALUE::MOVE_DOWN, action.m_modifier};
            default:
                return action;
        }
    }

    Action Cluster::rotateActionCounterClockWise(Action action) {
        switch (action.m_action) {
            case Action::VALUE::MOVE_UP:
                return {Action::VALUE::MOVE_LEFT, action.m_modifier};
            case Action::VALUE::MOVE_DOWN:
                return {Action::VALUE::MOVE_RIGHT, action.m_modifier};
            case Action::VALUE::MOVE_LEFT:
                return {Action::VALUE::MOVE_DOWN, action.m_modifier};
            case Action::VALUE::MOVE_RIGHT:
                return {Action::VALUE::MOVE_UP, action.m_modifier};
            default:
                return action;
        }
    }

    void Cluster::addAction(Action action) {
        m_actions.push_back(action);
    }

    void Cluster::removeBLock(const GridCoordinates& gridCoordinates) {
        const auto it = std::find(m_gridCoordinates.begin(), m_gridCoordinates.end(), gridCoordinates);
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
        if (m_pendingOperations.empty() || m_actions.at(m_actionIndex).m_modifier == Action::MODIFIER::IGNORE) {
            doAction();
            m_pendingOperations.clear();
            return;
        }
        if (m_actions.at(m_actionIndex).m_modifier == Action::MODIFIER::SKIP) {
            ++m_actionIndex;
            m_actionIndex %= m_actions.size();
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

    const std::vector<Action>& Cluster::actions() const {
        return m_actions;
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
        m_actions.clear();
        m_actionIndex = 0;
    }

    Cluster& Cluster::operator=(const Cluster& other) {
        assert(other.m_currentPhase == CURRENT_PHASE::NONE);
        clearPhase();
        m_isAlive         = other.m_isAlive;
        m_actionIndex     = other.m_actionIndex;
        m_gridCoordinates = other.m_gridCoordinates;
        m_actions         = other.m_actions;
        m_pendingOperations.clear();
        return *this;
    }

    size_t Cluster::currentActionIndex() const {
        if (m_fractionOfPhase != 0.0) {
            return (m_actionIndex + m_actions.size() - 1) % m_actions.size();
        } else {
            return m_actionIndex;
        }
    }
} // namespace model