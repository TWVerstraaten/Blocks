//
// Created by pc on 15-12-20.
//

#include "Cluster.h"

#include "../app/Application_constants.h"
#include "../misc/defines.h"
#include "../misc/geom.h"
#include "Level.h"
#include "Model.h"

#include <cassert>
#include <numeric>
#include <queue>

size_t model::Cluster::s_maxClusterIndex = 0;

namespace model {

    Cluster::Cluster() : m_index(s_maxClusterIndex), m_name("dummy") {
        ++s_maxClusterIndex;
    }

    Cluster::Cluster(GridXyContainer&& gridXyContainer, std::string name)
        : GridXyContainer(std::move(gridXyContainer)), m_index(s_maxClusterIndex), m_name(std::move(name)) {
        ++s_maxClusterIndex;
    }

    Cluster::Cluster(GridXyContainer&& gridXyContainer, CommandVector commandVector, std::string name)
        : GridXyContainer(std::move(gridXyContainer)), m_index(s_maxClusterIndex), m_commandVector(std::move(commandVector)),
          m_name(std::move(name)) {
        ++s_maxClusterIndex;
    }

    void Cluster::doCommand(Model& model) {
        if (m_commandVector.isEmpty() || m_state != CLUSTER_STATE::ALIVE) {
            return;
        }
        std::visit(D_FUNC(c, model::handleCommand(c, *this, model.level())), m_commandVector.currentCommand());
        m_gridXyAreSorted = false;
        m_sidesAreCorrect = false;
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

    WorldXy Cluster::dynamicWorldOffset() const {
        return {static_cast<int>(m_worldOffset.x() * m_phaseFraction), static_cast<int>(m_worldOffset.y() * m_phaseFraction)};
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
        m_phaseFraction = 1.0;
        m_phase         = PHASE::ROTATING;
        m_angle         = angle;
        m_rotationPivot = pivot;
    }

    void Cluster::incrementCommandIndex() {
        m_commandVector.increment();
    }

    const std::string& Cluster::name() const {
        return m_name;
    }

    size_t Cluster::index() const {
        return m_index;
    }

    std::string Cluster::string() const {
        std::string str;
        for (const auto& block : m_gridXyVector) {
            str += "(" + std::to_string(block.x()) + " " + std::to_string(block.y()) + ") " + ' ';
        }
        str += '\n';
        return str;
    }

    void Cluster::intersectWithLevel(const Level& level, int shrinkInWorld) {
        assert(noDuplicates());
        if (geom::intersect(sides(shrinkInWorld), level.sides())) {
            kill();
        } else {
            const auto& stoppedClusters = level.stoppedClusters();
            if (std::find_if(D_CIT(stoppedClusters),
                             D_FUNC(stoppedCluster, geom::intersect(sides(shrinkInWorld), stoppedCluster.sides(shrinkInWorld, idTransformation)))) !=
                stoppedClusters.end()) {
                kill();
            }
        }
    }

    PhaseTransformation Cluster::phaseTransformation() const {
        switch (m_phase) {
            case PHASE::NONE:
                return static_cast<PhaseTransformation>([](const WorldXy& b) { return b; });
            case PHASE::TRANSLATING: {
                const WorldXy offset = dynamicWorldOffset();
                return static_cast<PhaseTransformation>([offset](const WorldXy& b) { return b + offset; });
            }
            case PHASE::ROTATING: {
                const WorldXy center = WorldXy(m_rotationPivot) + app::HALF_BLOCK_IN_WORLD;
                const double  theta  = -angle();
                return static_cast<PhaseTransformation>([center, theta](const WorldXy& b) { return geom::rotateAboutPivot(b, center, theta); });
            }
        }
        assert(false);
        return idTransformation;
    }

    bool Cluster::intersects(const Cluster& other, int shrinkInWorld) const {
        assert(noDuplicates());
        assert(other.noDuplicates());
        assert(m_sidesAreCorrect);
        assert(other.m_sidesAreCorrect);
        return geom::intersect(*this, other, shrinkInWorld);
    }

    std::vector<Cluster> Cluster::collectAllButFirstComponent() {
        assert(noDuplicates());
        std::vector<Cluster> result;
        result.emplace_back(grabAllButFirstComponent(), m_name + "_");
        while (not result.back().isEmpty()) {
            if (result.back().size() == 1) {
                break;
            }
            result.emplace_back(result.back().grabAllButFirstComponent(), result.back().m_name + "_");
        }
        if (result.back().isEmpty()) {
            result.pop_back();
        }
        assert(isConnected());
        m_gridXyAreSorted = false;
        m_sidesAreCorrect = false;

        return result;
    }

    CLUSTER_STATE Cluster::state() const {
        return m_state;
    }

    void Cluster::spliceCluster(Level& level) {
        GridXyVector splicedGridXy;
        const auto   spliceBlocks = level.blocks(FLOOR_BLOCK_TYPE::SPLICE);
        auto         gridXyIt     = m_gridXyVector.begin();
        auto         spliceIt     = spliceBlocks.cbegin();
        while (gridXyIt != m_gridXyVector.end() && spliceIt != spliceBlocks.cend()) {
            if (*gridXyIt < *spliceIt) {
                ++gridXyIt;
            } else if (*spliceIt < *gridXyIt) {
                ++spliceIt;
            } else {
                splicedGridXy.emplace_back(*gridXyIt);
                gridXyIt = m_gridXyVector.erase(gridXyIt);
                ++spliceIt;
            }
        }
        if (not splicedGridXy.empty()) {
            level.stoppedClusters().emplace_back(GridXyContainer{std::move(splicedGridXy)});
        }
        m_gridXyAreSorted = false;
        m_sidesAreCorrect = false;
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
        m_gridXyAreSorted = false;
        m_sidesAreCorrect = false;
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

    PHASE Cluster::phase() const {
        return m_phase;
    }

    COMMAND_TYPE Cluster::currentType() const {
        return m_commandVector.currentType();
    }

    const geom::AlignedRectangle& Cluster::boundingAlignedRectangle() const {
        return m_boundingAlignedRectangle;
    }

    void Cluster::buildBoundingAlignedRectangle() {
        m_boundingAlignedRectangle = geom::AlignedRectangle::boundingAlignedRectangle(*this);
    }

    void Cluster::setPhaseFraction(double phaseFraction) {
        m_phaseFraction = phaseFraction;
    }

    size_t Cluster::maxClusterIndex() {
        return s_maxClusterIndex;
    }

    void Cluster::setMaxClusterIndex(size_t maxClusterIndex) {
        s_maxClusterIndex = maxClusterIndex;
    }

    WorldLineVector Cluster::sides(int shrinkInWorld) const {
        return GridXyContainer::sides(shrinkInWorld, phaseTransformation());
    }

} // namespace model