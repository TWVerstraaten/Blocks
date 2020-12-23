//
// Created by pc on 15-12-20.
//

#include "Cluster.h"

#include "../aux/Aux.h"

#include <algorithm>
#include <cassert>
#include <queue>
#include <utility>

size_t model::Cluster::s_maxClusterIndex = 0;

namespace model {
    Cluster::Cluster(std::list<GridXY>&& gridXY, std::string name)
        : m_gridXYList(gridXY), m_name(name.empty() ? "CL" + std::to_string(s_maxClusterIndex) : name) {
        m_index = s_maxClusterIndex;
        ++s_maxClusterIndex;
    }

    void Cluster::doAction() {
        if (m_actions.empty() || not m_isAlive) {
            return;
        }
        clearPhase();
        m_currentPhase    = CURRENT_PHASE::TRANSLATING;
        m_fractionOfPhase = 1.0;
        for (auto& idx : m_gridXYList) {
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
    }

    void Cluster::rotateClockWiseAbout(const GridXY& pivotGridXY) {
        for (auto& gridXY : m_gridXYList) {
            gridXY = {pivotGridXY.x() + pivotGridXY.y() - gridXY.y(), pivotGridXY.y() - pivotGridXY.x() + gridXY.x()};
        }
        std::transform(m_actions.begin(), m_actions.end(), m_actions.begin(), rotateActionClockWise);
    }

    void Cluster::rotateCounterClockWiseAbout(const GridXY& pivotGridXY) {
        for (auto& gridXY : m_gridXYList) {
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

    std::list<GridXY>::iterator Cluster::removeBLock(const GridXY& gridXY) {
        const std::list<GridXY>::iterator it = std::find(m_gridXYList.begin(), m_gridXYList.end(), gridXY);
        assert(it != m_gridXYList.end());
        return m_gridXYList.erase(it);
    }

    bool Cluster::empty() const {
        return m_gridXYList.empty();
    }

    const std::list<GridXY>& Cluster::gridXY() const {
        return m_gridXYList;
    }

    void Cluster::addPendingOperation(const GridXY& gridXY, Level::DYNAMIC_BLOCK_TYPE blockType) {
        if (blockType == Level::DYNAMIC_BLOCK_TYPE::NONE) {
            return;
        }
        m_pendingOperations.emplace_back(gridXY, blockType);
    }

    void Cluster::performPendingOperationOrNextAction() {
        if (m_pendingOperations.size() > 1) {
            kill();
            return;
        }

        if (m_pendingOperations.empty() || m_actions.at(m_actionIndex).m_modifier == Action::MODIFIER::IGNORE) {
            doAction();
            m_pendingOperations.clear();
        } else {
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

    WorldXY Cluster::dynamicWorldOffset() const {
        return {static_cast<int>(m_worldOffset.x() * m_fractionOfPhase), static_cast<int>(m_worldOffset.y() * m_fractionOfPhase)};
    }

    std::set<WorldXY> Cluster::cornerPoints(int shrinkInWorld) const {
        std::set<WorldXY> result;
        switch (m_currentPhase) {
            case CURRENT_PHASE::NONE:
                for (const auto& it : m_gridXYList) {
                    for (const GridXY cornerOffset : {GridXY{0, 0}, GridXY{0, 1}, GridXY{1, 1}, GridXY{1, 0}}) {
                        result.emplace(WorldXY::fromGridXY(it + cornerOffset) +
                                       WorldXY{shrinkInWorld - 2 * shrinkInWorld * cornerOffset.x(),
                                               shrinkInWorld - 2 * shrinkInWorld * cornerOffset.y()});
                    }
                }
                break;
            case CURRENT_PHASE::TRANSLATING: {
                const WorldXY offset = dynamicWorldOffset();
                for (const auto& it : m_gridXYList) {
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
                for (const auto& it : m_gridXYList) {
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
        clearPhase();
        assert(m_index > 1);
        m_name        = other.m_name;
        m_isAlive     = other.m_isAlive;
        m_actionIndex = other.m_actionIndex;
        m_gridXYList  = other.m_gridXYList;
        m_actions     = other.m_actions;
        m_pendingOperations.clear();
        return *this;
    }

    size_t Cluster::currentActionIndex() const {
        return m_actionIndex;
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

    const std::string& Cluster::getName() const {
        return m_name;
    }

    bool Cluster::isConnected() const {
        assert(not empty());
        if (m_gridXYList.size() == 1) {
            return true;
        }
        std::list<GridXY> copy = m_gridXYList;

        std::queue<GridXY> queue;
        queue.push(copy.front());
        while (not queue.empty() && not copy.empty()) {
            copy.remove(queue.front());
            const auto fr = queue.front();
            queue.pop();
            for (const auto& c : copy) {
                if (fr.isAdjacent(c)) {
                    queue.push(c);
                }
            }
        }

        return copy.empty();
    }

    Cluster Cluster::getComponent() {
        assert(not isConnected());
        std::list<GridXY> copy = m_gridXYList;

        std::queue<GridXY> queue;
        queue.push(copy.front());
        while (not queue.empty() && not copy.empty()) {
            copy.remove(queue.front());
            const auto fr = queue.front();
            queue.pop();
            for (const auto& c : copy) {
                if (fr.isAdjacent(c)) {
                    queue.push(c);
                }
            }
        }
        Cluster result{{}, getName() + "_"};
        result.clearPhase();
        result.m_isAlive     = m_isAlive;
        result.m_actionIndex = m_actionIndex;
        result.m_actions     = m_actions;
        result.m_pendingOperations.clear();

        result.m_gridXYList = std::move(copy);

        for (const auto& c : result.m_gridXYList) {
            m_gridXYList.remove(c);
        }

        return result;
    }

    size_t Cluster::index() const {
        return m_index;
    }

    std::string Cluster::toString() const {
        std::string str;
        for (const auto& block : m_gridXYList) {
            str += "(" + std::to_string(block.x()) + " " + std::to_string(block.y()) + ") " + ' ';
        }
        str += '\n';
        return str;
    }

    void Cluster::preStep() {
        if (m_currentPhase == CURRENT_PHASE::ROTATING) {
            if (m_actions.at(m_actionIndex).m_modifier == Action::MODIFIER::SKIP) {
                incrementActionIndex();
            }
        } else {
            incrementActionIndex();
            m_currentPhase = CURRENT_PHASE::NONE;
        }
    }
} // namespace model