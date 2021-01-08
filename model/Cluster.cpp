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

    Cluster::Cluster(std::set<GridXY>&& gridXY, const CommandVector& commandVector, std::string name)
        : m_index(s_maxClusterIndex), m_gridXYVector(gridXY), m_commandVector(std::move(commandVector)), m_name(std::move(name)) {
        ++s_maxClusterIndex;
    }

    void Cluster::doCommand(model::Model& model) {
        if (m_commandVector.empty() || m_state != CLUSTER_STATE::ALIVE) {
            return;
        }

        std::visit([&](auto c) { doAction(c, *this, model.level()); }, m_commandVector.currentCommand());
    }

    void Cluster::rotateClockWiseAbout(const GridXY& pivotGridXY) {
        std::set<GridXY> newGridXYSet;
        for (auto& gridXY : m_gridXYVector) {
            newGridXYSet.emplace(GridXY{pivotGridXY.x() + pivotGridXY.y() - gridXY.y(), pivotGridXY.y() - pivotGridXY.x() + gridXY.x()});
        }
        std::swap(m_gridXYVector, newGridXYSet);
        NOTE_ONCE("Do we want to rotate the directions...?")
    }

    void Cluster::rotateCounterClockWiseAbout(const GridXY& pivotGridXY) {
        std::set<GridXY> newGridXYSet;
        for (auto& gridXY : m_gridXYVector) {
            newGridXYSet.emplace(GridXY{pivotGridXY.x() - pivotGridXY.y() + gridXY.y(), pivotGridXY.y() + pivotGridXY.x() - gridXY.x()});
        }
        std::swap(m_gridXYVector, newGridXYSet);
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

    bool Cluster::alive() const {
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
        m_commandVector.clear();
    }

    void Cluster::incrementCommandIndex() {
        m_commandVector.increment();
    }

    const std::string& Cluster::name() const {
        return m_name;
    }

    bool Cluster::connected() const {
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
        Cluster result{std::move(copy), m_commandVector, name() + "_"};
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
        incrementCommandIndex();
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

    PHASE Cluster::phase() const {
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

    bool Cluster::adjacent(const Cluster& other) const {
        const auto otherGridXY = other.m_gridXYVector;
        return std::any_of(m_gridXYVector.begin(), m_gridXYVector.end(), [&](const GridXY& point1) {
            return std::find_if(otherGridXY.begin(), otherGridXY.end(), [&](const GridXY& point2) { return point2.isAdjacent(point1); }) !=
                   otherGridXY.end();
        });
    }

    void Cluster::stopIfNeeded() {
        if (m_commandVector.empty() || m_state != CLUSTER_STATE::ALIVE) {
            return;
        }
        if (m_commandVector.currentType() == COMMAND_TYPE::STP) {
            resetPhase();
            m_state = CLUSTER_STATE::STOPPED;
        }
    }

    void Cluster::grabAdjacentStoppedClusters(Level& level) {
        auto&            stoppedClusters = level.stoppedClusters();
        std::set<GridXY> newGridXy;
        for (auto& cluster : stoppedClusters) {
            if (adjacent(cluster)) {
                newGridXy.merge(cluster.m_gridXYVector);
            }
        }
        m_gridXYVector.merge(newGridXy);
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

        level.stoppedClusters().emplace_back(std::move(splicedGridXY), m_commandVector, name() + "_");
    }

    void Cluster::spliceIfNeeded(model::Model& model) {
        NOTE_ONCE("Implement")
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
        return m_commandVector.currentModifier();
    }

    size_t Cluster::commandIndex() const {
        return m_commandVector.commandIndex();
    }

    const CommandVector& Cluster::commandVector() const {
        return m_commandVector;
    }

    CommandVector& Cluster::commandVector() {
        return m_commandVector;
    }

    void Cluster::setState(CLUSTER_STATE state) {
        m_state = state;
    }

    void Cluster::setWorldOffset(const WorldXY& worldOffset) {
        m_worldOffset = worldOffset;
    }

    void Cluster::setPhase(PHASE phase) {
        m_phase = phase;
    }

} // namespace model