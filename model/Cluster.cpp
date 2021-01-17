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
    Cluster::Cluster(GridXYSet&& gridXY, std::string name) : m_index(s_maxClusterIndex), m_gridXYSet(gridXY), m_name(std::move(name)) {
        ++s_maxClusterIndex;
        m_sides = geom::getSidesFromGridXY(m_gridXYSet);
    }

    Cluster::Cluster(const GridXY& gridXY, std::string name) : m_index(s_maxClusterIndex), m_gridXYSet({gridXY}), m_name(std::move(name)) {
        ++s_maxClusterIndex;
        m_sides = geom::getSidesFromGridXY(m_gridXYSet);
    }

    Cluster::Cluster(GridXYSet&& gridXY, CommandVector commandVector, std::string name)
        : m_index(s_maxClusterIndex), m_commandVector(std::move(commandVector)), m_gridXYSet(gridXY), m_name(std::move(name)) {
        ++s_maxClusterIndex;
    }

    void Cluster::doCommand(model::Model& model) {
        if (m_commandVector.isEmpty() || m_state != CLUSTER_STATE::ALIVE) {
            return;
        }
        std::visit(D_FUNC(c, doAction(c, *this, model.level())), m_commandVector.currentCommand());
    }

    void Cluster::rotateClockWiseAbout(const GridXY& pivotGridXY) {
        GridXYSet newGridXYSet;
        for (auto& gridXY : m_gridXYSet) {
            newGridXYSet.emplace(GridXY{pivotGridXY.x() + pivotGridXY.y() - gridXY.y(), pivotGridXY.y() - pivotGridXY.x() + gridXY.x()});
        }
        m_gridXYSet.swap(newGridXYSet);
        D_NOTE_ONCE("Do we want to rotate the directions...?")
    }

    void Cluster::rotateCounterClockWiseAbout(const GridXY& pivotGridXY) {
        GridXYSet newGridXYSet;
        for (auto& gridXY : m_gridXYSet) {
            newGridXYSet.emplace(GridXY{pivotGridXY.x() - pivotGridXY.y() + gridXY.y(), pivotGridXY.y() + pivotGridXY.x() - gridXY.x()});
        }
        m_gridXYSet.swap(newGridXYSet);
    }

    GridXYSet::iterator Cluster::removeGridXY(const GridXY& gridXY) {
        const auto it = m_gridXYSet.find(gridXY);
        assert(it != m_gridXYSet.end());
        return m_gridXYSet.erase(it);
    }

    bool Cluster::isEmpty() const {
        return m_gridXYSet.empty();
    }

    const GridXYSet& Cluster::gridXY() const {
        return m_gridXYSet;
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

    const GridXY& Cluster::rotationPivot() const {
        return m_rotationPivot;
    }

    WorldXY Cluster::dynamicWorldOffset() const {
        return {static_cast<int>(m_worldOffset.x() * m_phaseFraction), static_cast<int>(m_worldOffset.y() * m_phaseFraction)};
    }

    std::set<WorldXY> Cluster::cornerPoints(int shrinkInWorld) const {
        std::set<WorldXY> result;
        const auto        f = phaseTransformation();

        for (const auto& it : m_gridXYSet) {
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
        if (m_gridXYSet.size() == 1) {
            return true;
        }
        auto copy = *this;
        return copy.grabAllButFirstComponent().isEmpty();
    }

    model::Cluster Cluster::grabAllButFirstComponent() {
        assert(not isEmpty());
        assert(size() > 1);

        std::queue<GridXY> queue;
        queue.push(*m_gridXYSet.begin());
        GridXYSet copy;
        copy.emplace(*m_gridXYSet.begin());
        m_gridXYSet.erase(m_gridXYSet.begin());
        while (not queue.empty() && not m_gridXYSet.empty()) {
            using dir = model::GridXY::DIRECTION;
            for (const auto direction : {dir::RIGHT, dir::DOWN, dir::UP, dir::LEFT}) {
                const auto it = m_gridXYSet.find(queue.front().neighbor(direction));
                if (it != m_gridXYSet.end()) {
                    queue.push(*it);
                    m_gridXYSet.erase(it);
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
        m_gridXYSet.swap(result.m_gridXYSet);
        return result;
    }

    size_t Cluster::index() const {
        return m_index;
    }

    std::string Cluster::string() const {
        std::string str;
        for (const auto& block : m_gridXYSet) {
            str += "(" + std::to_string(block.x()) + " " + std::to_string(block.y()) + ") " + ' ';
        }
        str += '\n';
        return str;
    }

    bool Cluster::contains(const GridXY& gridXY) const {
        return m_gridXYSet.find(gridXY) != m_gridXYSet.end();
    }

    void Cluster::addGridXY(const GridXY& gridXY) {
        if (not contains(gridXY)) {
            m_gridXYSet.emplace(gridXY);
        }
    }

    GridXYSet& Cluster::gridXY() {
        return m_gridXYSet;
    }

    void Cluster::collideWithLevel(const Level& level, int shrinkInWorld) {
        if (geom::intersect(sides(shrinkInWorld), level.boundaries())) {
            kill();
        }
    }

    WorldLineSet Cluster::sides(int shrinkInWorld) const {
        const auto   f = phaseTransformation();
        WorldLineSet result;
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
                const WorldXY center = WorldXY(m_rotationPivot) + app::HALF_BLOCK_IN_WORLD;
                const double  theta  = -angle();
                return [center, theta](const model::WorldXY& b) { return geom::rotateAboutPivot(b, center, theta); };
            }
        }
        assert(false);
        return [](const model::WorldXY& b) { return b; };
    }

    size_t Cluster::size() const {
        return m_gridXYSet.size();
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

        const int minX = geom::minX(cluster.gridXY());
        const int minY = geom::minY(cluster.gridXY());
        const int maxX = geom::maxX(cluster.gridXY());
        const int maxY = geom::maxY(cluster.gridXY());
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
        assert(not m_gridXYSet.empty());
        const auto f = phaseTransformation();
        return std::accumulate(D_CIT(m_gridXYSet), WorldXY{0, 0}, D_FUNC_2(a, b, a + f(b + app::HALF_BLOCK_IN_WORLD))) / m_gridXYSet.size();
    }

    CLUSTER_STATE Cluster::state() const {
        return m_state;
    }

    bool Cluster::isAdjacent(const Cluster& other) const {
        const auto otherGridXY = other.m_gridXYSet;
        return std::any_of(D_CIT(otherGridXY), D_FUNC(point1, gridXYIsAdjacent(point1)));
    }

    void Cluster::grabAdjacentStoppedClusters(Level& level) {
        auto&     stoppedClusters = level.stoppedClusters();
        GridXYSet newGridXy;
        std::for_each(D_IT(stoppedClusters), [&](auto& cluster) {
            if (isAdjacent(cluster)) {
                newGridXy.merge(cluster.m_gridXYSet);
            }
        });
        m_gridXYSet.merge(newGridXy);
        stoppedClusters.remove_if(D_FUNC(cluster, cluster.isEmpty()));
    }

    void Cluster::spliceCluster(Level& level) {
        GridXYSet  splicedGridXY;
        const auto spliceBlocks = level.blocks(FLOOR_BLOCK_TYPE::SPLICE);
        auto       gridXYIt     = m_gridXYSet.begin();
        auto       spliceIt     = spliceBlocks.cbegin();
        while (gridXYIt != m_gridXYSet.end() && spliceIt != spliceBlocks.cend()) {
            if (*gridXYIt < *spliceIt) {
                ++gridXYIt;
            } else if (*spliceIt < *gridXYIt) {
                ++spliceIt;
            } else {
                splicedGridXY.emplace(*gridXYIt);
                gridXYIt = m_gridXYSet.erase(gridXYIt);
                ++spliceIt;
            }
        }
        if (not splicedGridXY.empty()) {
            level.stoppedClusters().emplace_back(std::move(splicedGridXY), name() + "_");
        }
    }

    void Cluster::handleDynamicBlock(const GridXY& point, DYNAMIC_BLOCK_TYPE type) {
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
        m_sides = geom::getSidesFromGridXY(m_gridXYSet);
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

    void Cluster::setWorldOffset(const WorldXY& worldOffset) {
        m_worldOffset = worldOffset;
    }

    void Cluster::setPhase(PHASE phase) {
        m_phase = phase;
    }

    bool Cluster::gridXYIsAdjacent(const GridXY& point) const {
        return std::any_of(D_CIT(m_gridXYSet), D_FUNC(point2, point.isAdjacent(point2)));
    }

    PENDING_DYNAMIC_MOVES Cluster::pendingDynamicMoves() const {
        return m_pendingDynamicMoves;
    }

    void Cluster::setPendingDynamicMoves(PENDING_DYNAMIC_MOVES pendingDynamicMoves) {
        m_pendingDynamicMoves = pendingDynamicMoves;
    }

} // namespace model