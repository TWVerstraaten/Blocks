//
// Created by pc on 15-12-20.
//

#include "Cluster.h"

#include "../global/Global.h"

#include <algorithm>
#include <cassert>
#include <queue>
#include <utility>

size_t model::Cluster::s_maxClusterIndex = 0;

namespace model {
    Cluster::Cluster(std::vector<GridXY>&& gridXY, std::string name)
        : m_gridXYVector(gridXY), m_name(name.empty() ? "CL" + std::to_string(s_maxClusterIndex) : name), m_index(s_maxClusterIndex) {
        ++s_maxClusterIndex;
        assert(gridXUYAreUnique());
    }

    Cluster::Cluster(const std::vector<GridXY>& gridXY, std::string name) {
    }

    void Cluster::doAction() {
        if (m_actions.empty() || not m_isAlive) {
            return;
        }
        resetPhase();
        m_currentPhase    = CURRENT_PHASE::TRANSLATING;
        m_fractionOfPhase = 1.0;
        for (auto& idx : m_gridXYVector) {
            switch (m_actions[m_actionIndex].m_value) {
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
        for (auto& gridXY : m_gridXYVector) {
            gridXY = {pivotGridXY.x() + pivotGridXY.y() - gridXY.y(), pivotGridXY.y() - pivotGridXY.x() + gridXY.x()};
        }
        for (auto& action : m_actions) {
            action = rotateActionClockWise(action);
        }
    }

    void Cluster::rotateCounterClockWiseAbout(const GridXY& pivotGridXY) {
        for (auto& gridXY : m_gridXYVector) {
            gridXY = {pivotGridXY.x() - pivotGridXY.y() + gridXY.y(), pivotGridXY.y() + pivotGridXY.x() - gridXY.x()};
        }
        for (auto& m_action : m_actions) {
            m_action = rotateActionCounterClockWise(m_action);
        }
    }

    Action Cluster::rotateActionClockWise(Action action) {
        switch (action.m_value) {
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
        switch (action.m_value) {
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

    std::vector<GridXY>::iterator Cluster::removeBLock(const GridXY& gridXY) {
        const std::vector<GridXY>::iterator it = std::find(m_gridXYVector.begin(), m_gridXYVector.end(), gridXY);
        assert(it != m_gridXYVector.end());
        assert(gridXUYAreUnique());
        return m_gridXYVector.erase(it);
    }

    bool Cluster::empty() const {
        return m_gridXYVector.empty();
    }

    const std::vector<GridXY>& Cluster::gridXY() const {
        assert(empty() || gridXUYAreUnique());
        return m_gridXYVector;
    }

    void Cluster::addPendingOperation(const GridXY& gridXY, Level::DYNAMIC_BLOCK_TYPE blockType) {
        if (blockType == Level::DYNAMIC_BLOCK_TYPE::NONE) {
            return;
        }
        m_pendingOperations.emplace(gridXY, blockType);
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
        assert(gridXUYAreUnique());
        if (not m_isAlive) {
            return;
        }
        m_fractionOfPhase -= fractionOfPhase;
        if (m_fractionOfPhase <= 0.0) {
            resetPhase();
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
        assert(gridXUYAreUnique());

        std::set<WorldXY> result;
        switch (m_currentPhase) {
            case CURRENT_PHASE::NONE:
                for (const auto& it : m_gridXYVector) {
                    for (const GridXY cornerOffset : {GridXY{0, 0}, GridXY{0, 1}, GridXY{1, 1}, GridXY{1, 0}}) {
                        result.emplace(WorldXY::fromGridXY(it + cornerOffset) +
                                       WorldXY{shrinkInWorld - 2 * shrinkInWorld * cornerOffset.x(),
                                               shrinkInWorld - 2 * shrinkInWorld * cornerOffset.y()});
                    }
                }
                break;
            case CURRENT_PHASE::TRANSLATING: {
                const WorldXY offset = dynamicWorldOffset();
                for (const auto& it : m_gridXYVector) {
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
                for (const auto& it : m_gridXYVector) {
                    for (const GridXY cornerOffset : {GridXY{0, 0}, GridXY{0, 1}, GridXY{1, 1}, GridXY{1, 0}}) {
                        result.emplace(global::rotateAboutPivot(WorldXY::fromGridXY(it + cornerOffset) +
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

    void Cluster::resetPhase() {
        m_fractionOfPhase = 0.0;
        m_worldOffset     = {0, 0};
        m_angle           = 0.0;
        m_rotationPivot   = {0, 0};
    }

    void Cluster::setRotation(double angle, const GridXY& pivot) {
        assert(angle != 0.0);
        resetPhase();
        m_currentPhase    = CURRENT_PHASE::ROTATING;
        m_fractionOfPhase = 1.0;
        m_angle           = angle;
        m_rotationPivot   = pivot;
    }

    void Cluster::clearActions() {
        m_actions.clear();
        m_actionIndex = 0;
    }

    size_t Cluster::actionIndex() const {
        return m_actionIndex;
    }

    void Cluster::incrementActionIndex() {
        if (m_actions.empty()) {
            return;
        }
        ++m_actionIndex;
        m_actionIndex %= m_actions.size();
        assert(m_actionIndex < m_actions.size());
    }

    void Cluster::tryPendingOperation() {
        assert(!m_pendingOperations.empty());
        if (m_pendingOperations.size() > 1) {
            kill();
        }
        switch (m_pendingOperations.begin()->second) {
            case Level::DYNAMIC_BLOCK_TYPE::ROTATE_CW:
                setRotation(-90.0, m_pendingOperations.begin()->first);
                rotateClockWiseAbout(m_pendingOperations.begin()->first);
                break;
            case Level::DYNAMIC_BLOCK_TYPE::ROTATE_CCW:
                setRotation(90.0, m_pendingOperations.begin()->first);
                rotateCounterClockWiseAbout(m_pendingOperations.begin()->first);
                break;
            case Level::DYNAMIC_BLOCK_TYPE::NONE:
                break;
        }
        m_pendingOperations.clear();
    }

    const std::string& Cluster::name() const {
        return m_name;
    }

    bool Cluster::isConnected() const {
        assert(gridXUYAreUnique());
        assert(not empty());

        if (m_gridXYVector.size() == 1) {
            return true;
        }

        std::vector<bool> visited(m_gridXYVector.size(), false);
        visited[0] = true;
        std::queue<GridXY> queue;
        queue.push(m_gridXYVector[0]);
        while (not queue.empty()) {
            for (size_t i = 0; i != m_gridXYVector.size(); ++i) {
                if (visited[i]) {
                    continue;
                }
                if (m_gridXYVector.at(i).isAdjacent(queue.front())) {
                    queue.push(m_gridXYVector.at(i));
                    visited[i] = true;
                }
            }

            queue.pop();
        }
        return std::all_of(visited.begin(), visited.end(), [](const auto b) { return b; });
    }

    model::Cluster Cluster::getComponent() {
        assert(not empty());
        assert(not isConnected());
        assert(gridXUYAreUnique());

        std::vector<GridXY> copy;

        if (m_gridXYVector.size() == 1) {
            fprintf(stderr, "Fix naming, %s:%d\n", __FILE__, __LINE__);
            return Cluster{m_gridXYVector, "dummy"};
        }

        std::vector<bool> visited(m_gridXYVector.size(), false);
        visited[0] = true;
        std::queue<GridXY> queue;
        queue.push(m_gridXYVector[0]);
        copy.push_back(m_gridXYVector[0]);
        while (not queue.empty()) {
            for (size_t i = 0; i != m_gridXYVector.size(); ++i) {
                if (visited[i]) {
                    continue;
                }
                if (m_gridXYVector.at(i).isAdjacent(queue.front())) {
                    const auto r = m_gridXYVector.at(i);
                    queue.push({r.x(), r.y()});
                    copy.push_back({r.x(), r.y()});
                    visited[i] = true;
                }
            }
            queue.pop();
        }
        for (size_t i = visited.size(); i-- > 0;) {
            if (visited[i]) {
                m_gridXYVector.erase(m_gridXYVector.begin() + i);
            }
        }
        assert(not empty());

        assert(not copy.empty());
        Cluster result{std::move(copy), name() + "_"};
        assert(result.isConnected());
        result.resetPhase();
        result.m_isAlive     = m_isAlive;
        result.m_actionIndex = m_actionIndex;
        result.m_actions     = m_actions;
        result.m_pendingOperations.clear();
        return result;
    }

    size_t Cluster::index() const {
        return m_index;
    }

    std::string Cluster::string() const {
        std::string str;
        for (const auto& block : m_gridXYVector) {
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

    bool Cluster::contains(const GridXY& gridXY) const {
        return std::find(m_gridXYVector.begin(), m_gridXYVector.end(), gridXY) != m_gridXYVector.end();
    }

    void Cluster::addGridXY(const GridXY& gridXY) {
        assert(std::find_if(m_gridXYVector.begin(), m_gridXYVector.end(), [&](const GridXY& g) { return g.isAdjacent(gridXY); }) !=
               m_gridXYVector.end());

        if (not contains(gridXY)) {
            m_gridXYVector.push_back(gridXY);
        }
        assert(gridXUYAreUnique());
        assert(isConnected());
    }

    bool Cluster::gridXUYAreUnique() const {
        assert(!m_gridXYVector.empty());
        std::set<GridXY> s(m_gridXYVector.begin(), m_gridXYVector.end());
        return s.size() == m_gridXYVector.size();
    }

} // namespace model