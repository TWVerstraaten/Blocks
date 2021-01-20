//
// Created by pc on 15-12-20.
//

#include "Cluster.h"

#include "../app/Application_constants.h"
#include "../global/defines.h"
#include "../global/geom.h"
#include "Level.h"
#include "Model.h"

#include <cassert>
#include <numeric>
#include <queue>

size_t model::Cluster::s_maxClusterIndex = 0;

namespace model {

    Cluster::Cluster(GridXySet&& gridXy, std::string name) : m_index(s_maxClusterIndex), m_gridXySet(gridXy), m_name(std::move(name)) {
        ++s_maxClusterIndex;
        m_sides = geom::getSidesFromGridXy(m_gridXySet);
    }

    Cluster::Cluster(const GridXy& gridXy, std::string name) : m_index(s_maxClusterIndex), m_gridXySet({gridXy}), m_name(std::move(name)) {
        ++s_maxClusterIndex;
        m_sides = geom::getSidesFromGridXy(m_gridXySet);
    }

    Cluster::Cluster(GridXySet&& gridXy, CommandVector commandVector, std::string name)
        : m_index(s_maxClusterIndex), m_commandVector(std::move(commandVector)), m_gridXySet(gridXy), m_name(std::move(name)) {
        ++s_maxClusterIndex;
    }

    void Cluster::doCommand(Model& model) {
        if (m_commandVector.isEmpty() || m_state != CLUSTER_STATE::ALIVE) {
            return;
        }
        std::visit(D_FUNC(c, doAction(c, *this, model.level())), m_commandVector.currentCommand());
    }

    void Cluster::rotateClockWiseAbout(const GridXy& pivotGridXy) {
        GridXySet newGridXySet;
        for (auto& gridXy : m_gridXySet) {
            newGridXySet.emplace(GridXy{pivotGridXy.x() + pivotGridXy.y() - gridXy.y(), pivotGridXy.y() - pivotGridXy.x() + gridXy.x()});
        }
        m_gridXySet.swap(newGridXySet);
        D_NOTE_ONCE("Do we want to rotate the directions...?")
    }

    void Cluster::rotateCounterClockWiseAbout(const GridXy& pivotGridXy) {
        GridXySet newGridXySet;
        for (auto& gridXy : m_gridXySet) {
            newGridXySet.emplace(GridXy{pivotGridXy.x() - pivotGridXy.y() + gridXy.y(), pivotGridXy.y() + pivotGridXy.x() - gridXy.x()});
        }
        m_gridXySet.swap(newGridXySet);
    }

    GridXySet::iterator Cluster::removeGridXy(const GridXy& gridXy) {
        const auto it = m_gridXySet.find(gridXy);
        assert(it != m_gridXySet.end());
        return m_gridXySet.erase(it);
    }

    bool Cluster::isEmpty() const {
        return m_gridXySet.empty();
    }

    const GridXySet& Cluster::gridXy() const {
        return m_gridXySet;
    }

    void Cluster::update(double phaseFraction) {
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

    const GridXy& Cluster::rotationPivot() const {
        return m_rotationPivot;
    }

    WorldXy Cluster::dynamicWorldOffset() const {
        return {static_cast<int>(m_worldOffset.x() * m_phaseFraction), static_cast<int>(m_worldOffset.y() * m_phaseFraction)};
    }

    std::set<WorldXy> Cluster::cornerPoints(int shrinkInWorld) const {
        std::set<WorldXy> result;
        const auto        f = phaseTransformation();

        for (const auto& it : m_gridXySet) {
            const bool          u = contains(it.neighbor(GridXy::DIRECTION::UP));
            const bool          l = contains(it.neighbor(GridXy::DIRECTION::LEFT));
            const bool          d = contains(it.neighbor(GridXy::DIRECTION::DOWN));
            const bool          r = contains(it.neighbor(GridXy::DIRECTION::RIGHT));
            std::vector<GridXy> corners;
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
            for (const GridXy corner : corners) {
                result.emplace(f(
                    (GridXy(it + corner) + WorldXy{shrinkInWorld - 2 * shrinkInWorld * corner.x(), shrinkInWorld - 2 * shrinkInWorld * corner.y()})));
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

    void Cluster::setRotation(double angle, const GridXy& pivot) {
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

    bool Cluster::isConnected() const {
        D_NOTE_ONCE("Implement proper variant of this function")
        assert(not isEmpty());
        if (m_gridXySet.size() == 1) {
            return true;
        }
        auto copy = *this;
        return copy.grabAllButFirstComponent().isEmpty();
    }

    Cluster Cluster::grabAllButFirstComponent() {
        assert(not isEmpty());
        assert(size() > 1);

        std::queue<GridXy> queue;
        queue.push(*m_gridXySet.begin());
        GridXySet copy;
        copy.emplace(*m_gridXySet.begin());
        m_gridXySet.erase(m_gridXySet.begin());
        while (not queue.empty() && not m_gridXySet.empty()) {
            using dir = GridXy::DIRECTION;
            for (const auto direction : {dir::RIGHT, dir::DOWN, dir::UP, dir::LEFT}) {
                const auto it = m_gridXySet.find(queue.front().neighbor(direction));
                if (it != m_gridXySet.end()) {
                    queue.push(*it);
                    m_gridXySet.erase(it);
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
        m_gridXySet.swap(result.m_gridXySet);
        return result;
    }

    size_t Cluster::index() const {
        return m_index;
    }

    std::string Cluster::string() const {
        std::string str;
        for (const auto& block : m_gridXySet) {
            str += "(" + std::to_string(block.x()) + " " + std::to_string(block.y()) + ") " + ' ';
        }
        str += '\n';
        return str;
    }

    bool Cluster::contains(const GridXy& gridXy) const {
        return m_gridXySet.find(gridXy) != m_gridXySet.end();
    }

    void Cluster::addGridXy(const GridXy& gridXy) {
        if (not contains(gridXy)) {
            m_gridXySet.emplace(gridXy);
        }
    }

    GridXySet& Cluster::gridXy() {
        return m_gridXySet;
    }

    void Cluster::collideWithLevel(const Level& level, int shrinkInWorld) {
        if (geom::intersect(sides(shrinkInWorld), level.sides())) {
            kill();
        }
    }

    WorldLineSet Cluster::sides(int shrinkInWorld) const {
        const auto   f = phaseTransformation();
        WorldLineSet result;
        for (const auto& line : m_sides) {
            const int d = contains(GridXy::fromWorldXy(line.start())) ? 1 : -1;
            if (line.start().x() == line.end().x()) {
                result.insert(
                    {f(line.start() + WorldXy{d * shrinkInWorld, shrinkInWorld}), f(line.end() + WorldXy{d * shrinkInWorld, -shrinkInWorld})});
            } else {
                result.insert(
                    {f(line.start() + WorldXy{shrinkInWorld, d * shrinkInWorld}), f(line.end() + WorldXy{-shrinkInWorld, d * shrinkInWorld})});
            }
        }
        return result;
    }

    std::function<WorldXy(const WorldXy&)> Cluster::phaseTransformation() const {
        switch (m_phase) {
            case PHASE::NONE:
                return [](const WorldXy& b) { return b; };
            case PHASE::TRANSLATING: {
                const WorldXy offset = dynamicWorldOffset();
                return [offset](const WorldXy& b) { return b + offset; };
            }
            case PHASE::ROTATING: {
                const WorldXy center = WorldXy(m_rotationPivot) + app::HALF_BLOCK_IN_WORLD;
                const double  theta  = -angle();
                return [center, theta](const WorldXy& b) { return geom::rotateAboutPivot(b, center, theta); };
            }
        }
        assert(false);
        return [](const WorldXy& b) { return b; };
    }

    size_t Cluster::size() const {
        return m_gridXySet.size();
    }

    bool Cluster::intersects(const Cluster& other, int shrinkInWorld) const {
        return geom::intersect(sides(shrinkInWorld), other.sides(shrinkInWorld));
    }

    std::list<Cluster> Cluster::collectAllButFirstComponent() {
        std::list<Cluster> result;
        result.emplace_back(grabAllButFirstComponent());
        while (not result.back().isEmpty()) {
            if (result.back().size() == 1) {
                break;
            }
            result.emplace_back(result.back().grabAllButFirstComponent());
        }
        if (result.back().isEmpty()) {
            result.pop_back();
        }

        for (const auto& n : result) {
            assert(n.commandVector().strings() == m_commandVector.strings());
        }

        return result;
    }

    std::ostream& operator<<(std::ostream& out, const Cluster& cluster) {
        if (cluster.isEmpty()) {
            return out;
        }
        assert(not cluster.isEmpty());

        const int minX = geom::minX(cluster.gridXy());
        const int minY = geom::minY(cluster.gridXy());
        const int maxX = geom::maxX(cluster.gridXy());
        const int maxY = geom::maxY(cluster.gridXy());
        assert(minX <= maxX);
        assert(minY <= maxY);

        out << "Cluster " << cluster.m_index << '\n';
        for (int j = minY; j <= maxY; ++j) {
            for (int i = minX; i <= maxX; ++i) {
                if (cluster.contains(GridXy{i, j})) {
                    out << "o";
                } else {
                    out << " ";
                }
            }
            out << "\n";
        }

        return out;
    }

    WorldXy Cluster::approximateCenter() const {
        assert(not m_gridXySet.empty());
        const auto f = phaseTransformation();
        return std::accumulate(D_CIT(m_gridXySet), WorldXy{0, 0}, D_FUNC_2(a, b, a + f(b + app::HALF_BLOCK_IN_WORLD))) / m_gridXySet.size();
    }

    CLUSTER_STATE Cluster::state() const {
        return m_state;
    }

    bool Cluster::isAdjacent(const Cluster& other) const {
        const auto otherGridXy = other.m_gridXySet;
        return std::any_of(D_CIT(otherGridXy), D_FUNC(point1, gridXyIsAdjacent(point1)));
    }

    void Cluster::grabAdjacentStoppedClusters(Level& level) {
        auto&     stoppedClusters = level.stoppedClusters();
        GridXySet newGridXy;
        std::for_each(D_IT(stoppedClusters), [&](auto& cluster) {
            if (isAdjacent(cluster)) {
                newGridXy.merge(cluster.m_gridXySet);
            }
        });
        m_gridXySet.merge(newGridXy);
        stoppedClusters.remove_if(D_FUNC(cluster, cluster.isEmpty()));
    }

    void Cluster::spliceCluster(Level& level) {
        GridXySet  splicedGridXy;
        const auto spliceBlocks = level.blocks(FLOOR_BLOCK_TYPE::SPLICE);
        auto       gridXyIt     = m_gridXySet.begin();
        auto       spliceIt     = spliceBlocks.cbegin();
        while (gridXyIt != m_gridXySet.end() && spliceIt != spliceBlocks.cend()) {
            if (*gridXyIt < *spliceIt) {
                ++gridXyIt;
            } else if (*spliceIt < *gridXyIt) {
                ++spliceIt;
            } else {
                splicedGridXy.emplace(*gridXyIt);
                gridXyIt = m_gridXySet.erase(gridXyIt);
                ++spliceIt;
            }
        }
        if (not splicedGridXy.empty()) {
            level.stoppedClusters().emplace_back(std::move(splicedGridXy), name() + "_");
        }
    }

    void Cluster::handleDynamicBlock(const GridXy& point, DYNAMIC_BLOCK_TYPE type) {
        switch (type) {
            case DYNAMIC_BLOCK_TYPE::ROTATE_CW:
                setRotation(-90.0, point);
                rotateClockWiseAbout(point);
                break;
            case DYNAMIC_BLOCK_TYPE::ROTATE_CCW:
                setRotation(90.0, point);
                rotateCounterClockWiseAbout(point);
                break;
        }
    }

    void Cluster::buildSides() {
        m_sides = geom::getSidesFromGridXy(m_gridXySet);
    }

    COMMAND_MODIFIER Cluster::currentModifier() const {
        return m_commandVector.currentModifier();
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

    void Cluster::setWorldOffset(const WorldXy& worldOffset) {
        m_worldOffset = worldOffset;
    }

    void Cluster::setPhase(PHASE phase) {
        m_phase = phase;
    }

    bool Cluster::gridXyIsAdjacent(const GridXy& point) const {
        return std::any_of(D_CIT(m_gridXySet), D_FUNC(point2, point.isAdjacent(point2)));
    }

    PENDING_DYNAMIC_MOVES Cluster::pendingDynamicMoves() const {
        return m_pendingDynamicMoves;
    }

    void Cluster::setPendingDynamicMoves(PENDING_DYNAMIC_MOVES pendingDynamicMoves) {
        m_pendingDynamicMoves = pendingDynamicMoves;
    }

} // namespace model