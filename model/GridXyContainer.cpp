//
// Created by pc on 08-02-21.
//

#include "GridXyContainer.h"

#include "../misc/defines.h"
#include "../misc/geom.h"
#include "Level.h"

#include <algorithm>
#include <cassert>
#include <queue>

namespace model {

    GridXyContainer::GridXyContainer(const GridXy& gridXy) : m_gridXyVector({gridXy}) {
    }

    GridXyContainer::GridXyContainer(GridXyVector&& gridXy) : m_gridXyVector(std::move(gridXy)) {
    }

    const GridXyVector& GridXyContainer::gridXyVector() const {
        return m_gridXyVector;
    }

    bool GridXyContainer::gridXyIsAdjacent(const GridXy& point) const {
        return std::any_of(D_CIT(m_gridXyVector), D_FUNC(point2, point.isAdjacent(point2)));
    }

    bool GridXyContainer::contains(const GridXy& gridXy) const {
        assert(noDuplicates());
        return std::find(D_CIT(m_gridXyVector), gridXy) != m_gridXyVector.end();
    }

    bool GridXyContainer::noDuplicates() const {
        auto copy = m_gridXyVector;
        std::sort(D_IT(copy));
        auto it = std::unique(D_IT(copy));
        return it == copy.end();
    }

    void GridXyContainer::addGridXy(const GridXy& gridXy) {
        if (not contains(gridXy)) {
            m_gridXyVector.emplace_back(gridXy);
            m_gridXyAreSorted = false;
            m_sidesAreCorrect = false;
        }
        assert(noDuplicates());
    }

    void GridXyContainer::copyGridXy(const GridXyContainer& other) {
        m_gridXyVector.reserve(m_gridXyVector.size() + other.m_gridXyVector.size());
        std::copy(D_CIT(other.m_gridXyVector), std::back_inserter(m_gridXyVector));
        m_gridXyAreSorted = false;
        m_sidesAreCorrect = false;
    }

    void GridXyContainer::clearGridXy() {
        m_gridXyVector.clear();
    }

    void GridXyContainer::removeGridXy(const GridXy& gridXy) {
        assert(noDuplicates());
        const auto it = std::find(D_CIT(m_gridXyVector), gridXy);
        assert(it != m_gridXyVector.end());
        m_gridXyVector.erase(it);
        m_sidesAreCorrect = false;
    }

    void GridXyContainer::sortGridXy() {
        if (not m_gridXyAreSorted) {
            std::sort(D_IT(m_gridXyVector));
            m_gridXyAreSorted = true;
        }
    }

    void GridXyContainer::swapGridXy(GridXyVector& other) {
        std::swap(m_gridXyVector, other);
        m_gridXyAreSorted = false;
    }

    void GridXyContainer::addGridXy(const GridXyVector& other) {
        std::copy(D_IT(other), std::back_inserter(m_gridXyVector));
        m_gridXyAreSorted = false;
        m_sidesAreCorrect = false;
    }

    void GridXyContainer::rotateClockWiseAbout(const GridXy& pivotGridXy) {
        GridXyVector newGridXySet;
        for (auto& gridXy : m_gridXyVector) {
            newGridXySet.emplace_back(GridXy{pivotGridXy.x() + pivotGridXy.y() - gridXy.y(), pivotGridXy.y() - pivotGridXy.x() + gridXy.x()});
        }
        m_gridXyVector.swap(newGridXySet);
        m_gridXyAreSorted = false;
        m_sidesAreCorrect = false;
    }

    void GridXyContainer::rotateCounterClockWiseAbout(const GridXy& pivotGridXy) {
        GridXyVector newGridXySet;
        for (auto& gridXy : m_gridXyVector) {
            newGridXySet.emplace_back(GridXy{pivotGridXy.x() - pivotGridXy.y() + gridXy.y(), pivotGridXy.y() + pivotGridXy.x() - gridXy.x()});
        }
        m_gridXyVector.swap(newGridXySet);
        m_gridXyAreSorted = false;
        m_sidesAreCorrect = false;
    }

    WorldLineVector GridXyContainer::sides(int shrinkInWorld, const PhaseTransformation& f) const {
        assert(noDuplicates());
        assert(m_sidesAreCorrect);
        WorldLineVector result;
        result.reserve(m_sides.size());
        for (const auto& line : m_sides) {
            const int d = contains(GridXy::fromWorldXy(line.start())) ? 1 : -1;
            if (line.start().x() == line.end().x()) {
                result.emplace_back(WorldLine{f(line.start() + WorldXy{d * shrinkInWorld, shrinkInWorld}),
                                              f(line.end() + WorldXy{d * shrinkInWorld, -shrinkInWorld})});
            } else {
                result.emplace_back(WorldLine{f(line.start() + WorldXy{shrinkInWorld, d * shrinkInWorld}),
                                              f(line.end() + WorldXy{-shrinkInWorld, d * shrinkInWorld})});
            }
        }
        return result;
    }

    void GridXyContainer::buildSides() {
        assert(noDuplicates());
        sortGridXy();
        if (not m_sidesAreCorrect) {
            m_sides           = geom::getSidesFromGridXy(m_gridXyVector);
            m_sidesAreCorrect = true;
        }
    }

    size_t GridXyContainer::size() const {
        return m_gridXyVector.size();
    }

    bool GridXyContainer::isEmpty() const {
        return m_gridXyVector.empty();
    }

    bool GridXyContainer::isAdjacent(const GridXyContainer& other) const {
        assert(noDuplicates());
        const auto otherGridXy = other.m_gridXyVector;
        return std::any_of(D_CIT(otherGridXy), D_FUNC(point1, gridXyIsAdjacent(point1)));
    }

    void GridXyContainer::grabAdjacentStoppedClusters(Level& level) {
        assert(noDuplicates());
        auto&        stoppedClusters = level.stoppedClusters();
        GridXyVector newGridXy;
        std::for_each(D_IT(stoppedClusters), [&](auto& cluster) {
            if (isAdjacent(cluster)) {
                std::copy(D_IT(cluster.m_gridXyVector), std::back_inserter(newGridXy));
                cluster.m_gridXyVector.clear();
            }
        });
        std::copy(D_CIT(newGridXy), std::back_inserter(m_gridXyVector));
        stoppedClusters.erase(std::remove_if(D_IT(stoppedClusters), D_FUNC(cluster, cluster.isEmpty())), stoppedClusters.end());
        m_gridXyAreSorted = false;
        m_sidesAreCorrect = false;
    }

    GridXyContainer GridXyContainer::grabAllButFirstComponent() {
        assert(noDuplicates());
        assert(not isEmpty());
        assert(size() > 1);

        std::queue<GridXy> queue;
        queue.push(*m_gridXyVector.begin());
        GridXyContainer copy;
        GridXyVector&   vectorOfCopy = copy.m_gridXyVector;
        vectorOfCopy.reserve(m_gridXyVector.size());
        vectorOfCopy.emplace_back(*m_gridXyVector.begin());
        m_gridXyVector.erase(m_gridXyVector.begin());
        while (not queue.empty() && not m_gridXyVector.empty()) {
            using dir = GridXy::DIRECTION;
            for (const auto direction : {dir::RIGHT, dir::DOWN, dir::UP, dir::LEFT}) {
                const auto it = std::find(D_CIT(m_gridXyVector), queue.front().neighbor(direction));
                if (it != m_gridXyVector.end()) {
                    queue.push(*it);
                    m_gridXyVector.erase(it);
                }
            }
            vectorOfCopy.emplace_back(queue.front());
            queue.pop();
        }
        while (not queue.empty()) {
            vectorOfCopy.emplace_back(queue.front());
            queue.pop();
        }
        std::sort(vectorOfCopy.begin(), vectorOfCopy.end());
        vectorOfCopy.erase(std::unique(vectorOfCopy.begin(), vectorOfCopy.end()), vectorOfCopy.end());

        assert(not vectorOfCopy.empty());
        m_gridXyVector.swap(vectorOfCopy);
        assert(noDuplicates());
        m_gridXyAreSorted = false;
        m_sidesAreCorrect = false;
        return copy;
    }

    bool GridXyContainer::isConnected() const {
        D_NOTE_ONCE("Implement proper variant of this function")
        assert(noDuplicates());
        assert(not isEmpty());
        if (m_gridXyVector.size() == 1) {
            return true;
        }
        auto copy = *this;
        return copy.grabAllButFirstComponent().isEmpty();
    }
} // namespace model