//
// Created by pc on 15-12-20.
//

#include "Cluster.h"

#include "../global/alg.h"
#include "../global/cst.h"
#include "../global/fns.h"
#include "../global/geom.h"
#include "Level.h"
#include "Model.h"

#include <queue>
#include <utility>

size_t model::Cluster::s_maxClusterIndex = 0;

namespace model {
    Cluster::Cluster(std::set<GridXY>&& gridXY, std::string name) : m_index(s_maxClusterIndex), m_gridXYVector(gridXY), m_name(std::move(name)) {
        ++s_maxClusterIndex;
        m_sides = alg::getSidesFromGridXY(m_gridXYVector);
        assert(gridXUYAreUnique());
    }

    Cluster::Cluster(std::set<GridXY>&& gridXY, std::vector<Command_Temp> commands, size_t commandIndex, std::string name)
        : m_index(s_maxClusterIndex), m_gridXYVector(gridXY), m_commands(std::move(commands)), m_commandIndex(commandIndex), m_name(std::move(name)) {
        ++s_maxClusterIndex;
    }
    void Cluster::doCommand(model::Model& model) {
        if (m_commands.empty() || m_state != CLUSTER_STATE::ALIVE) {
            return;
        }
        assert(m_commandIndex < m_commands.size());
        if (m_commands.at(m_commandIndex).m_type == COMMAND_TYPE::SKP) {
            return;
        }
        m_phase = PHASE::TRANSLATING;
        std::set<GridXY> newGridXYSet;
        switch (m_commands[m_commandIndex].m_type) {
            case COMMAND_TYPE::FWD:
                for (auto& idx : m_gridXYVector) {
                    newGridXYSet.emplace(idx.neighbor(model::GridXY::DIRECTION::UP));
                }
                std::swap(m_gridXYVector, newGridXYSet);
                m_worldOffset = {0, cst::BLOCK_SIZE_IN_WORLD};
                break;
            case COMMAND_TYPE::BCK:
                for (auto& idx : m_gridXYVector) {

                    newGridXYSet.emplace(idx.neighbor(model::GridXY::DIRECTION::DOWN));
                }
                std::swap(m_gridXYVector, newGridXYSet);
                m_worldOffset = {0, -cst::BLOCK_SIZE_IN_WORLD};
                break;
            case COMMAND_TYPE::LFT:
                for (auto& idx : m_gridXYVector) {
                    newGridXYSet.emplace(idx.neighbor(model::GridXY::DIRECTION::LEFT));
                }
                std::swap(m_gridXYVector, newGridXYSet);
                m_worldOffset = {cst::BLOCK_SIZE_IN_WORLD, 0};
                break;
            case COMMAND_TYPE::RHT:
                for (auto& idx : m_gridXYVector) {
                    newGridXYSet.emplace(idx.neighbor(model::GridXY::DIRECTION::RIGHT));
                }
                std::swap(m_gridXYVector, newGridXYSet);
                m_worldOffset = {-cst::BLOCK_SIZE_IN_WORLD, 0};
                break;
            case COMMAND_TYPE::SKP:
                break;
            case COMMAND_TYPE::STP:
                assert(false);
                break;
            case COMMAND_TYPE::GRB:
                grabAdjacentStoppedClusters(model.level());
                break;
            case COMMAND_TYPE::SPL:
                break;
        }
    }

    void Cluster::rotateClockWiseAbout(const GridXY& pivotGridXY) {
        std::set<GridXY> newGridXYSet;
        for (auto& gridXY : m_gridXYVector) {
            newGridXYSet.emplace(GridXY{pivotGridXY.x() + pivotGridXY.y() - gridXY.y(), pivotGridXY.y() - pivotGridXY.x() + gridXY.x()});
        }
        std::swap(m_gridXYVector, newGridXYSet);
        for (auto& command : m_commands) {
            command = rotateCommandClockWise(command);
        }
    }

    void Cluster::rotateCounterClockWiseAbout(const GridXY& pivotGridXY) {
        std::set<GridXY> newGridXYSet;
        for (auto& gridXY : m_gridXYVector) {
            newGridXYSet.emplace(GridXY{pivotGridXY.x() - pivotGridXY.y() + gridXY.y(), pivotGridXY.y() + pivotGridXY.x() - gridXY.x()});
        }
        std::swap(m_gridXYVector, newGridXYSet);
        for (auto& command : m_commands) {
            command = rotateCommandCounterClockWise(command);
        }
    }

    Command_Temp Cluster::rotateCommandClockWise(Command_Temp command) {
        switch (command.m_type) {
            case COMMAND_TYPE::FWD:
                return Command_Temp{COMMAND_TYPE::RHT, command.m_modifier};
            case COMMAND_TYPE::BCK:
                return Command_Temp{COMMAND_TYPE::LFT, command.m_modifier};
            case COMMAND_TYPE::LFT:
                return Command_Temp{COMMAND_TYPE::FWD, command.m_modifier};
            case COMMAND_TYPE::RHT:
                return Command_Temp{COMMAND_TYPE::BCK, command.m_modifier};
            default:
                return command;
        }
    }

    Command_Temp Cluster::rotateCommandCounterClockWise(Command_Temp command) {
        switch (command.m_type) {
            case COMMAND_TYPE::FWD:
                return {COMMAND_TYPE::LFT, command.m_modifier};
            case COMMAND_TYPE::BCK:
                return {COMMAND_TYPE::RHT, command.m_modifier};
            case COMMAND_TYPE::LFT:
                return {COMMAND_TYPE::BCK, command.m_modifier};
            case COMMAND_TYPE::RHT:
                return {COMMAND_TYPE::FWD, command.m_modifier};
            default:
                return command;
        }
    }

    void Cluster::addCommand(Command_Temp command) {
        m_commands.push_back(command);
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

    void Cluster::update(double phaseFraction) {
        assert(gridXUYAreUnique());
        if (m_state != CLUSTER_STATE::ALIVE) {
            return;
        }
        m_phaseFraction = phaseFraction;
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

    const std::vector<Command_Temp>& Cluster::commands() const {
        return m_commands;
    }

    WorldXY Cluster::dynamicWorldOffset() const {
        return {static_cast<int>(m_worldOffset.x() * m_phaseFraction), static_cast<int>(m_worldOffset.y() * m_phaseFraction)};
    }

    std::set<WorldXY> Cluster::cornerPoints(int shrinkInWorld) const {
        assert(gridXUYAreUnique());
        std::set<WorldXY> result;
        const auto        f = phaseTransformation();

        for (const auto& it : m_gridXYVector) {
            const bool          u = contains(it.neighbor(model::GridXY::DIRECTION::UP));
            const bool          l = contains(it.neighbor(model::GridXY::DIRECTION::LEFT));
            const bool          d = contains(it.neighbor(model::GridXY::DIRECTION::DOWN));
            const bool          r = contains(it.neighbor(model::GridXY::DIRECTION::RIGHT));
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
                result.emplace(f(
                    (GridXY(it + corner) + WorldXY{shrinkInWorld - 2 * shrinkInWorld * corner.x(), shrinkInWorld - 2 * shrinkInWorld * corner.y()})));
            }
        }
        return result;
    }

    void Cluster::kill() {
        m_state = CLUSTER_STATE::DEAD;
    }

    bool Cluster::isAlive() const {
        return m_state == CLUSTER_STATE::ALIVE;
    }

    void Cluster::resetPhase() {
        m_phase         = PHASE::NONE;
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

    void Cluster::clearCommands() {
        m_commands.clear();
        m_commandIndex = 0;
    }

    size_t Cluster::commandIndex() const {
        return m_commandIndex;
    }

    void Cluster::incrementCommandIndex() {
        if (m_commands.empty()) {
            return;
        }
        ++m_commandIndex;
        m_commandIndex %= m_commands.size();
        assert(m_commandIndex < m_commands.size());
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
        auto copy = *this;
        return copy.grabAllButFirstComponent().empty();
    }

    model::Cluster Cluster::grabAllButFirstComponent() {
        assert(not empty());
        assert(size() > 1);
        assert(gridXUYAreUnique());

        std::queue<GridXY> queue;
        queue.push(*m_gridXYVector.begin());
        std::set<GridXY> copy;
        copy.emplace(*m_gridXYVector.begin());
        m_gridXYVector.erase(m_gridXYVector.begin());
        while (not queue.empty() && not m_gridXYVector.empty()) {
            using dir = model::GridXY::DIRECTION;
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
        Cluster result{std::move(copy), m_commands, m_commandIndex, name() + "_"};
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
        if (m_state != CLUSTER_STATE::ALIVE) {
            return;
        }
        switch (m_phase) {
            case PHASE::NONE:
            case PHASE::TRANSLATING:
                incrementCommandIndex();
                break;
            case PHASE::ROTATING:
                if ((not m_commands.empty()) && m_commands[m_commandIndex].m_modifier == COMMAND_MODIFIER::INCREMENT) {
                    incrementCommandIndex();
                }
                break;
        }
        resetPhase();
    }

    bool Cluster::contains(const GridXY& gridXY) const {
        return m_gridXYVector.find(gridXY) != m_gridXYVector.end();
    }

    void Cluster::addGridXY(const GridXY& gridXY) {
        if (not contains(gridXY)) {
            m_gridXYVector.emplace(gridXY);
        }
    }

    bool Cluster::gridXUYAreUnique() const {
        assert(!m_gridXYVector.empty());
        std::set<GridXY> s(m_gridXYVector.begin(), m_gridXYVector.end());
        return s.size() == m_gridXYVector.size();
    }

    std::set<GridXY>& Cluster::gridXY() {
        return m_gridXYVector;
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
            const int d = contains(GridXY::fromWorldXY(line.start())) ? 1 : -1;
            if (line.start().x() == line.end().x()) {
                result.insert(
                    {f(line.start() + WorldXY{d * shrinkInWorld, shrinkInWorld}), f(line.end() + WorldXY{d * shrinkInWorld, -shrinkInWorld})});
            } else {
                result.insert(
                    {f(line.start() + WorldXY{shrinkInWorld, d * shrinkInWorld}), f(line.end() + WorldXY{-shrinkInWorld, d * shrinkInWorld})});
            }
        }
        return result;
    }

    std::function<WorldXY(const WorldXY&)> Cluster::phaseTransformation() const {
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
        return [](const model::WorldXY& b) { return b; };
    }

    size_t Cluster::size() const {
        return m_gridXYVector.size();
    }

    bool Cluster::intersects(const Cluster& other, int shrinkInWorld) const {
        return alg::intersect(sides(shrinkInWorld), other.sides(shrinkInWorld));
    }

    std::list<Cluster> Cluster::collectAllButFirstComponent() {
        std::list<Cluster> result;
        result.emplace_back(grabAllButFirstComponent());
        while (not result.back().empty()) {
            if (result.back().size() == 1) {
                break;
            }
            result.emplace_back(result.back().grabAllButFirstComponent());
        }
        if (result.back().empty()) {
            result.pop_back();
        }
        return result;
    }

    std::ostream& operator<<(std::ostream& out, const Cluster& cluster) {
        if (cluster.empty()) {
            return out;
        }
        assert(not cluster.empty());
        const int minX = std::min_element(cluster.m_gridXYVector.begin(), cluster.m_gridXYVector.end(), [](const GridXY& lhs, const GridXY& rhs) {
                             return lhs.x() < rhs.x();
                         })->x();
        const int minY = cluster.m_gridXYVector.begin()->y();
        const int maxX = std::max_element(cluster.m_gridXYVector.begin(), cluster.m_gridXYVector.end(), [](const GridXY& lhs, const GridXY& rhs) {
                             return lhs.x() < rhs.x();
                         })->x();
        const int maxY = cluster.m_gridXYVector.rbegin()->y();
        assert(minX <= maxX);
        assert(minY <= maxY);

        out << "Cluster " << cluster.m_index << '\n';
        for (int j = minY; j <= maxY; ++j) {
            for (int i = minX; i <= maxX; ++i) {
                if (cluster.contains(GridXY{i, j})) {
                    out << "o";
                } else {
                    out << " ";
                }
            }
            out << "\n";
        }

        return out;
    }

    WorldXY Cluster::approximateCenter() const {
        const auto f = phaseTransformation();
        WorldXY    result{0, 0};
        for (const auto& g : m_gridXYVector) {
            result += f(g);
        }
        result /= m_gridXYVector.size();
        return result;
    }

    CLUSTER_STATE Cluster::state() const {
        return m_state;
    }

    bool Cluster::isAdjacent(const Cluster& other) const {
        for (const auto& it1 : m_gridXYVector) {
            for (const auto& it2 : other.m_gridXYVector) {
                if (it1.isAdjacent(it2)) {
                    return true;
                }
            }
        }

        return false;
    }

    void Cluster::stopIfNeeded() {
        if (m_commands.empty() || m_state != CLUSTER_STATE::ALIVE) {
            return;
        }
        if (((not m_commands.empty()) && m_commands.at(m_commandIndex).m_modifier == COMMAND_MODIFIER::IGNORE)) {
            resetPhase();
            if (m_commands.at(m_commandIndex).m_type == COMMAND_TYPE::STP) {
                m_state = CLUSTER_STATE::STOPPED;
            }
        }
    }

    void Cluster::grabAdjacentStoppedClusters(Level& level) {
        auto& stoppedClusters = level.stoppedClusters();
        for (auto& cluster : stoppedClusters) {
            if (isAdjacent(cluster)) {
                m_gridXYVector.merge(cluster.m_gridXYVector);
            }
        }
        stoppedClusters.remove_if([](const Cluster& cluster) { return cluster.empty(); });
    }

    void Cluster::spliceCluster(Level& level) {
        std::set<GridXY> splicedGridXY;
        const auto&      spliceBlocks = level.spliceBlocks();
        for (const auto& gridXY : m_gridXYVector) {
            if (spliceBlocks.find(gridXY) != spliceBlocks.end()) {
                splicedGridXY.emplace(gridXY);
            }
        }
        if (splicedGridXY.empty()) {
            return;
        }
        for (const auto& gridXY : splicedGridXY) {
            m_gridXYVector.erase(gridXY);
        }

        level.stoppedClusters().emplace_back(std::move(splicedGridXY), m_commands, m_commandIndex, name() + "_");
    }

    void Cluster::spliceIfNeeded(model::Model& model) {
        if (m_commands.empty() || m_state != CLUSTER_STATE::ALIVE) {
            return;
        }
        if (((not m_commands.empty()) && m_commands.at(m_commandIndex).m_modifier == COMMAND_MODIFIER::IGNORE)) {
            resetPhase();
            if (m_commands.at(m_commandIndex).m_type == COMMAND_TYPE::SPL) {
                spliceCluster(model.level());
            }
        }
    }

    void Cluster::doOperation(const GridXY& point, DYNAMIC_BLOCK_TYPE type) {
        switch (type) {
            case model::DYNAMIC_BLOCK_TYPE::ROTATE_CW:
                setRotation(-90.0, point);
                rotateClockWiseAbout(point);
                break;
            case model::DYNAMIC_BLOCK_TYPE::ROTATE_CCW:
                setRotation(90.0, point);
                rotateCounterClockWiseAbout(point);
                break;
        }
    }

    void Cluster::buildSides() {
        m_sides = alg::getSidesFromGridXY(m_gridXYVector);
    }

    COMMAND_MODIFIER Cluster::currentModifier() const {
        if (m_commands.empty()) {
            return COMMAND_MODIFIER::NONE;
        }
        assert(m_commandIndex < m_commands.size());
        return m_commands.at(m_commandIndex).m_modifier;
    }

} // namespace model