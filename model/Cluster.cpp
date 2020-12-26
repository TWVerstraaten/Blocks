//
// Created by pc on 15-12-20.
//

#include "Cluster.h"

#include "../global/alg.h"
#include "../global/fns.h"
#include "../global/geom.h"
#include "Model.h"

#include <algorithm>
#include <cassert>
#include <queue>
#include <utility>

size_t model::Cluster::s_maxClusterIndex = 0;

namespace model {
    Cluster::Cluster(std::set<GridXY>&& gridXY, const std::string& name)
        : m_index(s_maxClusterIndex), m_name(name.empty() ? "CL" + std::to_string(s_maxClusterIndex) : name), m_gridXYVector(gridXY) {
        ++s_maxClusterIndex;
        m_sides = alg::getSidesInGridXY<GridXY>(m_gridXYVector);
        assert(gridXUYAreUnique());
    }

    Cluster::Cluster(const std::set<GridXY>& gridXY, std::string name) {
    }

    void Cluster::doAction() {
        if (m_actions.empty() || not m_alive) {
            return;
        }
        assert(m_actionIndex < m_actions.size());
        resetPhase();
        m_phase = PHASE::TRANSLATING;
        std::set<GridXY> newGridXYSet;
        for (auto& idx : m_gridXYVector) {
            switch (m_actions[m_actionIndex].m_value) {
                case Action::VALUE::MOVE_UP:
                    newGridXYSet.emplace(idx.neighbor(enums::DIRECTION::UP));
                    m_worldOffset = {0, cst::BLOCK_SIZE_IN_WORLD};
                    break;
                case Action::VALUE::MOVE_DOWN:
                    newGridXYSet.emplace(idx.neighbor(enums::DIRECTION::DOWN));
                    m_worldOffset = {0, -cst::BLOCK_SIZE_IN_WORLD};
                    break;
                case Action::VALUE::MOVE_LEFT:
                    newGridXYSet.emplace(idx.neighbor(enums::DIRECTION::LEFT));
                    m_worldOffset = {cst::BLOCK_SIZE_IN_WORLD, 0};
                    break;
                case Action::VALUE::MOVE_RIGHT:
                    newGridXYSet.emplace(idx.neighbor(enums::DIRECTION::RIGHT));
                    m_worldOffset = {-cst::BLOCK_SIZE_IN_WORLD, 0};
                    break;
            }
        }
        std::swap(m_gridXYVector, newGridXYSet);
    }

    void Cluster::rotateClockWiseAbout(const GridXY& pivotGridXY) {
        std::set<GridXY> newGridXYSet;
        for (auto& gridXY : m_gridXYVector) {
            newGridXYSet.emplace(GridXY{pivotGridXY.x() + pivotGridXY.y() - gridXY.y(), pivotGridXY.y() - pivotGridXY.x() + gridXY.x()});
        }
        std::swap(m_gridXYVector, newGridXYSet);
        for (auto& action : m_actions) {
            action = rotateActionClockWise(action);
        }
    }

    void Cluster::rotateCounterClockWiseAbout(const GridXY& pivotGridXY) {
        std::set<GridXY> newGridXYSet;
        for (auto& gridXY : m_gridXYVector) {
            newGridXYSet.emplace(GridXY{pivotGridXY.x() - pivotGridXY.y() + gridXY.y(), pivotGridXY.y() + pivotGridXY.x() - gridXY.x()});
        }
        std::swap(m_gridXYVector, newGridXYSet);
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

    std::set<GridXY>::iterator Cluster::removeBLock(const GridXY& gridXY) {
        const auto it = m_gridXYVector.find(gridXY);
        assert(it != m_gridXYVector.end());
        assert(gridXUYAreUnique());
        return m_gridXYVector.erase(it);
    }

    bool Cluster::empty() const {
        return m_gridXYVector.empty();
    }

    const std::set<GridXY>& Cluster::gridXY() const {
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
        m_sides = alg::getSidesInGridXY<GridXY>(m_gridXYVector);
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
        std::set<WorldXY>                     result;
        std::function<WorldXY(const WorldXY)> f = phaseTransformation();

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
                result.emplace(f((GridXY(it + corner) + WorldXY{shrinkInWorld - 2 * shrinkInWorld * corner.x(),
                                                                shrinkInWorld - 2 * shrinkInWorld * corner.y()})));
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
        NOTE_ONCE("Implement proper variant of this function")

        assert(gridXUYAreUnique());
        assert(not empty());
        if (m_gridXYVector.size() == 1) {
            return true;
        }
        auto       copy = *this;
        const auto r    = copy.grabSecondComponent();
        if (copy.blockCount() == blockCount()) {
            assert(r.empty());
            return true;
        } else {
            return false;
        }
    }

    model::Cluster Cluster::grabSecondComponent() {
        assert(not empty());
        assert(gridXUYAreUnique());

        if (m_gridXYVector.size() == 1) {
            NOTE_ONCE("Fix naming");
            return Cluster{m_gridXYVector, "dummy"};
        }

        std::queue<GridXY> queue;
        queue.push(*m_gridXYVector.begin());
        std::set<GridXY> copy;
        copy.emplace(*m_gridXYVector.begin());
        m_gridXYVector.erase(m_gridXYVector.begin());
        while (not queue.empty() && not m_gridXYVector.empty()) {
            using dir = enums::DIRECTION;
            for (const auto direction : {dir::RIGHT, dir::DOWN, dir::UP, dir::LEFT}) {
                const auto it = m_gridXYVector.find(queue.front().neighbor(direction));
                if (it != m_gridXYVector.end()) {
                    queue.push(*it);
                    m_gridXYVector.erase(it);
                }
            }
            copy.emplace(queue.front());
            queue.pop();
        }
        while (not queue.empty()) {
            copy.emplace(queue.front());
            queue.pop();
        }
        assert(not copy.empty());
        Cluster result{std::move(copy), name() + "_"};
        result.resetPhase();
        result.m_alive       = m_alive;
        result.m_actionIndex = m_actionIndex;
        result.m_actions     = m_actions;
        result.m_pendingOperations.clear();
        std::swap(m_gridXYVector, result.m_gridXYVector);
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
        return m_gridXYVector.find(gridXY) != m_gridXYVector.end();
    }

    void Cluster::addGridXY(const GridXY& gridXY) {
        assert(std::find_if(m_gridXYVector.begin(), m_gridXYVector.end(), [&](const GridXY& g) { return g.isAdjacent(gridXY); }) !=
               m_gridXYVector.end());
        if (not contains(gridXY)) {
            m_gridXYVector.emplace(gridXY);
        }
        assert(gridXUYAreUnique());
        assert(isConnected());
    }

    bool Cluster::gridXUYAreUnique() const {
        assert(!m_gridXYVector.empty());
        std::set<GridXY> s(m_gridXYVector.begin(), m_gridXYVector.end());
        return s.size() == m_gridXYVector.size();
    }

    std::set<GridXY>& Cluster::gridXY() {
        return m_gridXYVector;
    }

    WorldXY rotatedPoint(const GridXY& gridXY, const WorldXY& shrinkInWorld, const WorldXY& pivot, double theta) {
        return geom::rotateAboutPivot(WorldXY(gridXY) + shrinkInWorld, pivot, theta);
    }

    Cluster::PHASE Cluster::phase() const {
        return m_phase;
    }

    void Cluster::collideWithLevel(const Level& level, int shrinkInWorld) {
        assert(gridXUYAreUnique());
        if (alg::intersect(sides(shrinkInWorld), level.boundaries())) {
            kill();
        }
    }

    std::set<Line<WorldXY>> Cluster::sides(int shrinkInWorld) const {
        const auto              f = phaseTransformation();
        std::set<Line<WorldXY>> result;
        for (const auto& line : m_sides) {
            const int d = contains(line.start()) ? 1 : -1;
            if (line.start().x() == line.end().x()) {
                result.insert({f(line.start() + WorldXY{d * shrinkInWorld, shrinkInWorld}),
                               f(line.end() + WorldXY{d * shrinkInWorld, -shrinkInWorld})});
            } else {
                result.insert({f(line.start() + WorldXY{shrinkInWorld, d * shrinkInWorld}),
                               f(line.end() + WorldXY{-shrinkInWorld, d * shrinkInWorld})});
            }
        }
        return result;
    }

    std::function<WorldXY(const WorldXY)> Cluster::phaseTransformation() const {
        switch (m_phase) {
            case PHASE::NONE:
                return [](const model::WorldXY& b) { return b; };
            case PHASE::TRANSLATING: {
                const WorldXY offset = dynamicWorldOffset();
                return [offset](const model::WorldXY& b) { return b + offset; };
            }
            case PHASE::ROTATING: {
                const WorldXY center = WorldXY(m_rotationPivot) + cst::HALF_BLOCK_IN_WORLD;
                const double  theta  = -angle();
                return [center, theta](const model::WorldXY& b) { return geom::rotateAboutPivot(b, center, theta); };
            }
        }
        assert(false);
    }

    size_t Cluster::blockCount() const {
        return m_gridXYVector.size();
    }

    bool Cluster::intersects(const Cluster& other, int shrinkInWorld) const {
        return alg::intersect(sides(shrinkInWorld), other.sides(shrinkInWorld));
    }

} // namespace model