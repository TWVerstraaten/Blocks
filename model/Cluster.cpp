//
// Created by pc on 15-12-20.
//

#include "Cluster.h"

#include "../aux/Aux.h"

#include <algorithm>
#include <cassert>
#include <queue>

namespace model {

    Cluster::Cluster(std::list<GridXY>&& gridXY, const GridXY& offset) : m_worldOffset(0, 0), m_gridXY(gridXY) {
        for (auto& idx : m_gridXY) {
            idx += offset;
        }
    }

    void Cluster::doAction() {
        if (m_actions.empty() || not m_isAlive) {
            return;
        }
        clearPhase();
        m_currentPhase    = CURRENT_PHASE::TRANSLATING;
        m_fractionOfPhase = 1.0;
        for (auto& idx : m_gridXY) {
            switch (m_actions[m_actionIndex].m_action) {
                case Action::VALUE::MOVE_UP:
                    idx           = idx.adjacent(enums::DIRECTION::UP);
                    m_worldOffset = {0, WorldXY::m_blockSizeInWorld};
                    break;
                case Action::VALUE::MOVE_DOWN:
                    idx           = idx.adjacent(enums::DIRECTION::DOWN);
                    m_worldOffset = {0, -WorldXY::m_blockSizeInWorld};
                    break;
                case Action::VALUE::MOVE_LEFT:
                    idx           = idx.adjacent(enums::DIRECTION::LEFT);
                    m_worldOffset = {WorldXY::m_blockSizeInWorld, 0};
                    break;
                case Action::VALUE::MOVE_RIGHT:
                    idx           = idx.adjacent(enums::DIRECTION::RIGHT);
                    m_worldOffset = {-WorldXY::m_blockSizeInWorld, 0};
                    break;
            }
        }
        incrementActionIndex();
    }

    void Cluster::rotateClockWiseAbout(const GridXY& pivotGridXY) {
        for (auto& gridXY : m_gridXY) {
            gridXY = {pivotGridXY.x() + pivotGridXY.y() - gridXY.y(), pivotGridXY.y() - pivotGridXY.x() + gridXY.x()};
        }
        std::transform(m_actions.begin(), m_actions.end(), m_actions.begin(), rotateActionClockWise);
    }

    void Cluster::rotateCounterClockWiseAbout(const GridXY& pivotGridXY) {
        for (auto& gridXY : m_gridXY) {
            gridXY = {pivotGridXY.x() - pivotGridXY.y() + gridXY.y(), pivotGridXY.y() + pivotGridXY.x() - gridXY.x()};
        }
        std::transform(m_actions.begin(), m_actions.end(), m_actions.begin(), rotateActionCounterClockWise);
    }

    Action Cluster::rotateActionClockWise(Action action) {
        switch (action.m_action) {
            case Action::VALUE::MOVE_UP:
                return Action{Action::VALUE::MOVE_RIGHT, action.m_modifier};
            case Action::VALUE::MOVE_DOWN:
                return Action{Action::VALUE::MOVE_LEFT, action.m_modifier};
            case Action::VALUE::MOVE_LEFT:
                return Action{Action::VALUE::MOVE_UP, action.m_modifier};
            case Action::VALUE::MOVE_RIGHT:
                return Action{Action::VALUE::MOVE_DOWN, action.m_modifier};
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

    void Cluster::removeBLock(const GridXY& gridXY) {
        const auto it = std::find(m_gridXY.begin(), m_gridXY.end(), gridXY);
        assert(it != m_gridXY.end());
        m_gridXY.erase(it);
    }

    bool Cluster::empty() const {
        return m_gridXY.empty();
    }

    const std::list<GridXY>& Cluster::gridXY() const {
        return m_gridXY;
    }

    void Cluster::addPendingOperation(const GridXY& gridXY, Level::DYNAMIC_BLOCK_TYPE blockType) {
        if (blockType == Level::DYNAMIC_BLOCK_TYPE::NONE) {
            return;
        }
        m_pendingOperations.emplace_back(gridXY, blockType);
    }

    void Cluster::performPendingOperationOrNextAction() {
        assert(m_pendingOperations.size() <= 1);
        if (m_pendingOperations.empty() || m_actions.at(m_actionIndex).m_modifier == Action::MODIFIER::IGNORE) {
            doAction();
            m_pendingOperations.clear();
        } else {
            if (m_actions.at(m_actionIndex).m_modifier == Action::MODIFIER::SKIP) {
                incrementActionIndex();
            }
            tryPendingOperation();
        }
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

    const GridXY& Cluster::rotationPivot() const {
        return m_rotationPivot;
    }

    const std::vector<Action>& Cluster::actions() const {
        return m_actions;
    }

    WorldVector Cluster::dynamicWorldOffset() const {
        return {static_cast<int>(m_worldOffset.x() * m_fractionOfPhase), static_cast<int>(m_worldOffset.y() * m_fractionOfPhase)};
    }

    std::set<WorldXY> Cluster::cornerPoints(int shrinkInWorld) const {
        std::set<WorldXY> result;
        switch (m_currentPhase) {
            case CURRENT_PHASE::NONE:
                for (const auto& it : m_gridXY) {
                    for (const GridXY cornerOffset : {GridXY{0, 0}, GridXY{0, 1}, GridXY{1, 1}, GridXY{1, 0}}) {
                        result.emplace(WorldXY::fromGridXY(it + cornerOffset) +
                                       WorldXY{shrinkInWorld - 2 * shrinkInWorld * cornerOffset.x(),
                                               shrinkInWorld - 2 * shrinkInWorld * cornerOffset.y()});
                    }
                }
                break;
            case CURRENT_PHASE::TRANSLATING: {
                const WorldVector offset = dynamicWorldOffset();
                for (const auto& it : m_gridXY) {
                    for (const GridXY cornerOffset : {GridXY{0, 0}, GridXY{0, 1}, GridXY{1, 1}, GridXY{1, 0}}) {
                        result.emplace(WorldXY::fromGridXY(it + cornerOffset) +
                                       WorldXY{shrinkInWorld - 2 * shrinkInWorld * cornerOffset.x(),
                                               shrinkInWorld - 2 * shrinkInWorld * cornerOffset.y()} +
                                       offset);
                    }
                }
            } break;
            case CURRENT_PHASE::ROTATING: {
                const WorldXY center = WorldXY::fromGridXY(m_rotationPivot) + WorldXY::halfBlockInWorld;
                const double  theta  = -angle();
                for (const auto& it : m_gridXY) {
                    for (const GridXY cornerOffset : {GridXY{0, 0}, GridXY{0, 1}, GridXY{1, 1}, GridXY{1, 0}}) {
                        result.emplace(aux::rotateAboutPivot(WorldXY::fromGridXY(it + cornerOffset) +
                                                                 WorldXY{shrinkInWorld - 2 * shrinkInWorld * cornerOffset.x(),
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

    void Cluster::setRotation(double angle, const GridXY& pivot) {
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
        m_isAlive     = other.m_isAlive;
        m_actionIndex = other.m_actionIndex;
        m_gridXY      = other.m_gridXY;
        m_actions     = other.m_actions;
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

    void Cluster::incrementActionIndex() {
        ++m_actionIndex;
        m_actionIndex %= m_actions.size();
    }

    void Cluster::tryPendingOperation() {
        assert(!m_pendingOperations.empty());
        if (m_pendingOperations.size() > 1) {
            kill();
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
} // namespace model