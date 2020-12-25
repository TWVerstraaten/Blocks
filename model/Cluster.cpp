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
    Cluster::Cluster(std::vector<GridXY>&& gridXY, const std::string& name)
        : m_index(s_maxClusterIndex), m_name(name.empty() ? "CL" + std::to_string(s_maxClusterIndex) : name), m_gridXYVector(gridXY) {
        sortGridXYVector();
        for (const auto& g : m_gridXYVector) {
            std::cout << g.x() << " " << g.y() << '\n';
        }

        ++s_maxClusterIndex;
        assert(gridXUYAreUnique());
    }

    Cluster::Cluster(const std::vector<GridXY>& gridXY, std::string name) {
    }

    void Cluster::doAction() {
        if (m_actions.empty() || not m_alive) {
            return;
        }
        assert(m_actionIndex < m_actions.size());
        resetPhase();
        m_phase = PHASE::TRANSLATING;
        for (auto& idx : m_gridXYVector) {
            switch (m_actions[m_actionIndex].m_value) {
                case Action::VALUE::MOVE_UP:
                    idx           = idx.neighbor(enums::DIRECTION::UP);
                    m_worldOffset = {0, WorldXY::m_blockSizeInWorld};
                    break;
                case Action::VALUE::MOVE_DOWN:
                    idx           = idx.neighbor(enums::DIRECTION::DOWN);
                    m_worldOffset = {0, -WorldXY::m_blockSizeInWorld};
                    break;
                case Action::VALUE::MOVE_LEFT:
                    idx           = idx.neighbor(enums::DIRECTION::LEFT);
                    m_worldOffset = {WorldXY::m_blockSizeInWorld, 0};
                    break;
                case Action::VALUE::MOVE_RIGHT:
                    idx           = idx.neighbor(enums::DIRECTION::RIGHT);
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
        if (((not m_actions.empty()) && m_actions.at(m_actionIndex).m_modifier == Action::MODIFIER::IGNORE) ||
            m_pendingOperations.empty()) {
            doAction();
            m_pendingOperations.clear();
        } else {
            tryPendingOperation();
        }
    }

    void Cluster::update(double dPhase) {
        assert(gridXUYAreUnique());
        if (not m_alive) {
            return;
        }
        m_phaseFraction = dPhase;
        if (m_phaseFraction <= 0.0) {
            resetPhase();
        }
    }

    double Cluster::angle() const {
        return m_phaseFraction * m_angle;
    }

    const GridXY& Cluster::rotationPivot() const {
        return m_rotationPivot;
    }

    const std::vector<Action>& Cluster::actions() const {
        return m_actions;
    }

    WorldXY Cluster::dynamicWorldOffset() const {
        return {static_cast<int>(m_worldOffset.x() * m_phaseFraction), static_cast<int>(m_worldOffset.y() * m_phaseFraction)};
    }

    std::set<WorldXY> Cluster::cornerPoints(int shrinkInWorld) const {
        assert(gridXUYAreUnique());
        std::set<WorldXY>                                    result;
        std::function<WorldXY(const GridXY&, const WorldXY)> f = toWorldFunction();

        for (const auto& it : m_gridXYVector) {
            const bool          u = contains(it.neighbor(enums::DIRECTION::UP));
            const bool          l = contains(it.neighbor(enums::DIRECTION::LEFT));
            const bool          d = contains(it.neighbor(enums::DIRECTION::DOWN));
            const bool          r = contains(it.neighbor(enums::DIRECTION::RIGHT));
            std::vector<GridXY> corners;
            if (not(u && l)) {
                corners.emplace_back(0, 0);
            }
            if (not r) {
                corners.emplace_back(1, 0);
            }
            if (not d) {
                corners.emplace_back(0, 1);
            }
            if (not(d || r)) {
                corners.emplace_back(1, 1);
            }
            for (const GridXY corner : corners) {
                result.emplace(f((it + corner),
                                 WorldXY{shrinkInWorld - 2 * shrinkInWorld * corner.x(), shrinkInWorld - 2 * shrinkInWorld * corner.y()}));
            }
        }
        return result;
    }

    void Cluster::kill() {
        m_alive = false;
    }

    bool Cluster::isAlive() const {
        return m_alive;
    }

    void Cluster::resetPhase() {
        m_worldOffset   = {0, 0};
        m_angle         = 0.0;
        m_rotationPivot = {0, 0};
    }

    void Cluster::setRotation(double angle, const GridXY& pivot) {
        assert(angle != 0.0);
        resetPhase();
        m_phase         = PHASE::ROTATING;
        m_angle         = angle;
        m_rotationPivot = pivot;
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
        if (m_pendingOperations.empty()) {
            return;
        }
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
        result.m_alive       = m_alive;
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
        if (not m_alive) {
            return;
        }
        switch (m_phase) {
            case PHASE::NONE:
            case PHASE::TRANSLATING:
                incrementActionIndex();
                break;
            case PHASE::ROTATING:
                if ((not m_actions.empty()) && m_actions[m_actionIndex].m_modifier == Action::MODIFIER::SKIP) {
                    incrementActionIndex();
                }
                break;
        }
        m_phase = PHASE::NONE;
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

    std::vector<GridXY>& Cluster::gridXY() {
        return m_gridXYVector;
    }

    WorldXY rotatedPoint(const GridXY& gridXY, const WorldXY& shrinkInWorld, const WorldXY& pivot, double theta) {
        return global::rotateAboutPivot(WorldXY(gridXY) + shrinkInWorld, pivot, theta);
    }

    Cluster::PHASE Cluster::phase() const {
        return m_phase;
    }

    void Cluster::collideWithLevel(const Level& level, int shrinkInWorld) {
        assert(gridXUYAreUnique());
        std::set<WorldXY>                                    result;
        std::function<WorldXY(const GridXY&, const WorldXY)> f = toWorldFunction();

        for (const auto& it : m_gridXYVector) {
            const bool          u = contains(it.neighbor(enums::DIRECTION::UP));
            const bool          l = contains(it.neighbor(enums::DIRECTION::LEFT));
            const bool          d = contains(it.neighbor(enums::DIRECTION::DOWN));
            const bool          r = contains(it.neighbor(enums::DIRECTION::RIGHT));
            std::vector<GridXY> corners;
            if (not(u && l)) {
                corners.emplace_back(0, 0);
            }
            if (not r) {
                corners.emplace_back(1, 0);
            }
            if (not d) {
                corners.emplace_back(0, 1);
            }
            if (not(d || r)) {
                corners.emplace_back(1, 1);
            }
            for (const GridXY corner : corners) {
                if (not level.isInLevel(
                        f((it + corner),
                          WorldXY{shrinkInWorld - 2 * shrinkInWorld * corner.x(), shrinkInWorld - 2 * shrinkInWorld * corner.y()}))) {
                    kill();
                    return;
                }
            }
        }
    }

    WorldXY Cluster::cornerPoint(const GridXY& it, CORNER corner, int shrinkInWorld) const {
        switch (corner) {
            case CORNER::TOP_LEFT:
                return WorldXY(it) + WorldXY{shrinkInWorld, shrinkInWorld};
            case CORNER::TOP_RIGHT:
                return WorldXY(it + GridXY{1, 0}) + WorldXY{-shrinkInWorld, shrinkInWorld};
            case CORNER::BOTTOM_LEFT:
                return WorldXY(it + GridXY{0, 1}) + WorldXY{shrinkInWorld, -shrinkInWorld};
            case CORNER::BOTTOM_RIGHT:
                return WorldXY(it + GridXY{1, 1}) + WorldXY{-shrinkInWorld, -shrinkInWorld};
        }
        assert(false);
    }

    enums::DIRECTION Cluster::currentDirection() const {
        assert(m_phase == PHASE::TRANSLATING);
        if (m_worldOffset.x() == 0) {
            return m_worldOffset.y() < 0 ? enums::DIRECTION::DOWN : enums::DIRECTION::UP;
        }
        return m_worldOffset.x() < 0 ? enums::DIRECTION::RIGHT : enums::DIRECTION::LEFT;
    }

    constexpr std::pair<Cluster::CORNER, Cluster::CORNER> Cluster::cornersAtSide(enums::DIRECTION direction) {
        switch (direction) {
            case enums::DIRECTION::UP:
                return {CORNER::TOP_LEFT, CORNER::TOP_RIGHT};
            case enums::DIRECTION::DOWN:
                return {CORNER::BOTTOM_LEFT, CORNER::BOTTOM_RIGHT};
            case enums::DIRECTION::LEFT:
                return {CORNER::BOTTOM_LEFT, CORNER::TOP_LEFT};
            case enums::DIRECTION::RIGHT:
                return {CORNER::BOTTOM_RIGHT, CORNER::TOP_RIGHT};
            case enums::DIRECTION::NONE:
                assert(false);
        }
        assert(false);
    }

    void Cluster::sortGridXYVector() {
        std::sort(m_gridXYVector.begin(), m_gridXYVector.end());
    }

    std::vector<WorldLine> Cluster::sides(int shrinkInWorld) const {
        std::vector<WorldLine> result;
        //        for (const auto& it : m_gridXYVector) {}
        return result;
    }

    std::function<WorldXY(const GridXY&, const WorldXY)> Cluster::toWorldFunction() const {
        switch (m_phase) {
            case PHASE::NONE:
                return [](const model::GridXY& a, const model::WorldXY& b) -> WorldXY { return a + b; };
            case PHASE::TRANSLATING: {
                const WorldXY offset = dynamicWorldOffset();
                return [offset](const model::GridXY& a, const model::WorldXY& b) -> WorldXY { return a + b + offset; };
            }
            case PHASE::ROTATING: {
                const WorldXY center = WorldXY(m_rotationPivot) + WorldXY::halfBlockInWorld;
                const double  theta  = -angle();
                return [center, theta](const model::GridXY& a, const model::WorldXY& b) -> WorldXY {
                    return global::rotateAboutPivot(a + b, center, theta);
                };
            }
        }
        assert(false);
    }

} // namespace model