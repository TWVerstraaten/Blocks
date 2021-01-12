//
// Created by pc on 15-12-20.
//

#include "Level.h"

#include "../action/AddLevelBlockAction.h"
#include "../action/RemoveLevelBlockAction.h"
#include "../global/defines.h"
#include "../global/geom.h"

#include <cassert>

namespace model {
    const std::map<GridXY, DYNAMIC_BLOCK_TYPE>& Level::dynamicBlocks() const {
        return m_dynamicBLocks;
    }

    const std::map<GridXY, INSTANT_BLOCK_TYPE>& Level::instantBlocks() const {
        return m_instantBLocks;
    }

    std::unique_ptr<action::Action> Level::addBlock(const GridXY& gridXY, DYNAMIC_BLOCK_TYPE blockType) {
        bool levelBlockAdded = false;
        if (m_dynamicBLocks.find(gridXY) != m_dynamicBLocks.end() && m_dynamicBLocks[gridXY] == blockType) {
            return nullptr;
        }
        if (m_floorBlocks.find(gridXY) == m_floorBlocks.end()) {
            levelBlockAdded = true;
            addBlock(gridXY, model::FLOOR_BLOCK_TYPE::LEVEL);
        }
        m_dynamicBLocks[gridXY] = blockType;
        if (levelBlockAdded) {
            return std::unique_ptr<action::Action>(new action::AddLevelBlockAction({blockType, FLOOR_BLOCK_TYPE::LEVEL}, gridXY));
        } else {
            return std::unique_ptr<action::Action>(new action::AddLevelBlockAction({blockType}, gridXY));
        }
    }

    std::unique_ptr<action::Action> Level::addBlock(const GridXY& gridXY, INSTANT_BLOCK_TYPE blockType) {
        bool levelBlockAdded = false;
        if (m_instantBLocks.find(gridXY) != m_instantBLocks.end() && m_instantBLocks[gridXY] == blockType) {
            return nullptr;
        }
        if (m_floorBlocks.find(gridXY) == m_floorBlocks.end()) {
            levelBlockAdded = true;
            addBlock(gridXY, model::FLOOR_BLOCK_TYPE::LEVEL);
        }
        m_instantBLocks[gridXY] = blockType;
        if (levelBlockAdded) {
            return std::unique_ptr<action::Action>(new action::AddLevelBlockAction({blockType, FLOOR_BLOCK_TYPE::LEVEL}, gridXY));
        } else {
            return std::unique_ptr<action::Action>(new action::AddLevelBlockAction({blockType}, gridXY));
        }
    }

    std::unique_ptr<action::Action> Level::addLevelBlock(const GridXY& gridXY) {
        if (m_floorBlocks.find(gridXY) != m_floorBlocks.end()) {
            return nullptr;
        }
        m_floorBlocks.emplace(std::make_pair(gridXY, FLOOR_BLOCK_TYPE::LEVEL));
        createBoundaries();
        return std::unique_ptr<action::Action>(new action::AddLevelBlockAction({FLOOR_BLOCK_TYPE::LEVEL}, gridXY));
    }

    std::unique_ptr<action::Action> Level::addStartBlock(const GridXY& gridXY) {
        if (m_floorBlocks.find(gridXY) != m_floorBlocks.end() && m_floorBlocks[gridXY] == FLOOR_BLOCK_TYPE::START) {
            return nullptr;
        }
        if (m_floorBlocks.find(gridXY) == m_floorBlocks.end()) {
            m_floorBlocks[gridXY] = FLOOR_BLOCK_TYPE::START;
            return std::unique_ptr<action::Action>(new action::AddLevelBlockAction({FLOOR_BLOCK_TYPE::LEVEL, FLOOR_BLOCK_TYPE::START}, gridXY));
        }
        m_floorBlocks[gridXY] = FLOOR_BLOCK_TYPE::START;
        return std::unique_ptr<action::Action>(new action::AddLevelBlockAction({FLOOR_BLOCK_TYPE::START}, gridXY));
    }

    void Level::clear() {
        m_floorBlocks.clear();
        m_instantBLocks.clear();
        m_dynamicBLocks.clear();
    }

    bool Level::isFreeStartBlock(const GridXY& gridXY) const {
        if (m_floorBlocks.find(gridXY) == m_floorBlocks.end() || m_floorBlocks.at(gridXY) != FLOOR_BLOCK_TYPE::START) {
            return false;
        }
        if (m_instantBLocks.find(gridXY) != m_instantBLocks.end()) {
            return false;
        }
        if (m_dynamicBLocks.find(gridXY) != m_dynamicBLocks.end()) {
            return false;
        }
        return true;
    }

    const WorldLineSet& Level::boundaries() const {
        return m_boundaries;
    }

    bool Level::contains(const GridXY& gridXY) const {
        return m_floorBlocks.find(gridXY) != m_floorBlocks.end();
    }

    void Level::createBoundaries() {
        std::set<GridXY> blocks;
        for (const auto& [point, _] : m_floorBlocks) {
            blocks.emplace(point);
        }
        m_boundaries = geom::getSidesFromGridXY(blocks);
        for (const auto& cluster : m_stoppedClusters) {
            m_boundaries.merge(geom::getSidesFromGridXY(cluster.gridXY()));
        }
    }

    std::unique_ptr<action::Action> Level::addBlock(const GridXY& gridXY, FLOOR_BLOCK_TYPE blockType) {
        __NOTE_ONCE("Implement undoable action")
        m_floorBlocks[gridXY] = blockType;
        return nullptr;
    }

    std::unique_ptr<action::Action> Level::removeBlock(const GridXY& gridXY) {
        if (m_instantBLocks.find(gridXY) != m_instantBLocks.end()) {
            return removeBlock(gridXY, m_instantBLocks[gridXY]);
        }
        if (m_dynamicBLocks.find(gridXY) != m_dynamicBLocks.end()) {
            return removeBlock(gridXY, m_dynamicBLocks[gridXY]);
        }
        if (m_floorBlocks.find(gridXY) != m_floorBlocks.end()) {
            if (m_floorBlocks[gridXY] == FLOOR_BLOCK_TYPE::LEVEL) {
                m_floorBlocks.erase(gridXY);
                return std::make_unique<action::RemoveLevelBlockAction>(FLOOR_BLOCK_TYPE::LEVEL, gridXY);
            }
            const auto type       = m_floorBlocks[gridXY];
            m_floorBlocks[gridXY] = FLOOR_BLOCK_TYPE::LEVEL;
            return std::make_unique<action::RemoveLevelBlockAction>(type, gridXY);
        }
        return nullptr;
    }

    std::unique_ptr<action::Action> Level::removeBlock(const GridXY& gridXY, DYNAMIC_BLOCK_TYPE blockType) {
        assert(m_dynamicBLocks.find(gridXY) != m_dynamicBLocks.end() && m_dynamicBLocks[gridXY] == blockType);
        const auto type = m_dynamicBLocks[gridXY];
        m_dynamicBLocks.erase(gridXY);
        return std::make_unique<action::RemoveLevelBlockAction>(type, gridXY);
    }

    std::unique_ptr<action::Action> Level::removeBlock(const GridXY& gridXY, INSTANT_BLOCK_TYPE blockType) {
        assert(m_instantBLocks.find(gridXY) != m_instantBLocks.end() && m_instantBLocks[gridXY] == blockType);
        const auto type = m_instantBLocks[gridXY];
        m_instantBLocks.erase(gridXY);
        return std::make_unique<action::RemoveLevelBlockAction>(type, gridXY);
    }

    std::unique_ptr<action::Action> Level::removeBlock(const GridXY& gridXY, [[maybe_unused]] FLOOR_BLOCK_TYPE blockType) {
        m_floorBlocks.erase(gridXY);
        //        assert(m_floorBlocks.find(gridXY) != m_floorBlocks.end());
        //        switch (blockType) {
        //            case FLOOR_BLOCK_TYPE::LEVEL:
        //                assert(m_floorBlocks[gridXY] == FLOOR_BLOCK_TYPE::LEVEL);
        //                m_floorBlocks.erase(gridXY);
        //                createBoundaries();
        //                return std::make_unique<action::RemoveLevelBlockAction>(FLOOR_BLOCK_TYPE::LEVEL, gridXY);
        //            case FLOOR_BLOCK_TYPE::START:
        //                assert(m_floorBlocks[gridXY] == FLOOR_BLOCK_TYPE::START);
        //                m_floorBlocks[gridXY] = FLOOR_BLOCK_TYPE::LEVEL;
        //                return std::make_unique<action::RemoveLevelBlockAction>(FLOOR_BLOCK_TYPE::START, gridXY);
        //            case FLOOR_BLOCK_TYPE::SPLICE:
        //                assert(m_floorBlocks[gridXY] == FLOOR_BLOCK_TYPE::SPLICE);
        //                m_floorBlocks[gridXY] = FLOOR_BLOCK_TYPE::LEVEL;
        //                return std::make_unique<action::RemoveLevelBlockAction>(FLOOR_BLOCK_TYPE::SPLICE, gridXY);
        //        }
        return nullptr;
    }

    std::list<Cluster>& Level::stoppedClusters() {
        return m_stoppedClusters;
    }

    const std::list<Cluster>& Level::stoppedClusters() const {
        return m_stoppedClusters;
    }

    Level::Action_u_ptr Level::addSpliceBlock(const GridXY& gridXY) {
        if (m_floorBlocks.find(gridXY) != m_floorBlocks.end() && m_floorBlocks[gridXY] == FLOOR_BLOCK_TYPE::SPLICE) {
            return nullptr;
        }
        if (m_floorBlocks.find(gridXY) == m_floorBlocks.end()) {
            m_floorBlocks[gridXY] = FLOOR_BLOCK_TYPE::SPLICE;
            return std::unique_ptr<action::Action>(new action::AddLevelBlockAction({FLOOR_BLOCK_TYPE::LEVEL, FLOOR_BLOCK_TYPE::SPLICE}, gridXY));
        }
        m_floorBlocks[gridXY] = FLOOR_BLOCK_TYPE::SPLICE;
        return std::unique_ptr<action::Action>(new action::AddLevelBlockAction({FLOOR_BLOCK_TYPE::SPLICE}, gridXY));
    }

    const std::map<GridXY, FLOOR_BLOCK_TYPE>& Level::floorBlocks() const {
        return m_floorBlocks;
    }

    GridXYSet Level::blocks(FLOOR_BLOCK_TYPE blockType) const {
        std::set<GridXY> result;
        for (const auto& [point, type] : m_floorBlocks) {
            if (type == blockType) {
                result.emplace(point);
            }
        }
        return result;
    }

} // namespace model