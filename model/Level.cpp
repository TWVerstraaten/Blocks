//
// Created by pc on 15-12-20.
//

#include "Level.h"

#include "../action/AddLevelBlockAction.h"
#include "../action/RemoveLevelBlockAction.h"
#include "../global/alg.h"
#include "../global/fns.h"

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
        if (m_levelBlocks.find(gridXY) == m_levelBlocks.end()) {
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
        if (m_levelBlocks.find(gridXY) == m_levelBlocks.end()) {
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
        if (m_levelBlocks.find(gridXY) != m_levelBlocks.end()) {
            return nullptr;
        }
        m_levelBlocks.emplace(gridXY);
        createBoundaries();
        return std::unique_ptr<action::Action>(new action::AddLevelBlockAction({FLOOR_BLOCK_TYPE::LEVEL}, gridXY));
    }

    std::unique_ptr<action::Action> Level::addStartBlock(const GridXY& gridXY) {
        if (m_startBlocks.find(gridXY) != m_startBlocks.end()) {
            return nullptr;
        }
        if (m_levelBlocks.find(gridXY) == m_levelBlocks.end()) {
            m_levelBlocks.emplace(gridXY);
            createBoundaries();
            m_startBlocks.emplace(gridXY);
            return std::unique_ptr<action::Action>(new action::AddLevelBlockAction({FLOOR_BLOCK_TYPE::LEVEL, FLOOR_BLOCK_TYPE::START}, gridXY));
        }
        m_startBlocks.emplace(gridXY);
        return std::unique_ptr<action::Action>(new action::AddLevelBlockAction({FLOOR_BLOCK_TYPE::START}, gridXY));
    }

    const std::set<GridXY>& Level::levelBlocks() const {
        return m_levelBlocks;
    }

    void Level::clear() {
        m_levelBlocks.clear();
        m_startBlocks.clear();
        m_spliceBlocks.clear();
        m_instantBLocks.clear();
        m_dynamicBLocks.clear();
    }

    const std::set<GridXY>& Level::startBlocks() const {
        return m_startBlocks;
    }

    bool Level::isFreeStartBlock(const GridXY& gridXY) const {
        if (m_startBlocks.find(gridXY) == m_startBlocks.end()) {
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

    const std::set<Line<WorldXY>>& Level::boundaries() const {
        return m_boundaries;
    }

    bool Level::contains(const GridXY& gridXY) const {
        return m_levelBlocks.find(gridXY) != m_levelBlocks.end();
    }

    void Level::createBoundaries() {
        m_boundaries = alg::getSidesFromGridXY(m_levelBlocks);
        for (const auto& cluster : m_stoppedClusters) {
            m_boundaries.merge(alg::getSidesFromGridXY(cluster.gridXY()));
        }
    }

    std::unique_ptr<action::Action> Level::addBlock(const GridXY& gridXY, FLOOR_BLOCK_TYPE blockType) {
        switch (blockType) {
            case FLOOR_BLOCK_TYPE::LEVEL:
                return addLevelBlock(gridXY);
            case FLOOR_BLOCK_TYPE::START:
                return addStartBlock(gridXY);
            case FLOOR_BLOCK_TYPE::SPLICE:
                return addSpliceBlock(gridXY);
                break;
        }
        return nullptr;
    }

    std::unique_ptr<action::Action> Level::removeBlock(const GridXY& gridXY) {
        if (m_instantBLocks.find(gridXY) != m_instantBLocks.end()) {
            return removeBlock(gridXY, m_instantBLocks[gridXY]);
        } else if (m_dynamicBLocks.find(gridXY) != m_dynamicBLocks.end()) {
            return removeBlock(gridXY, m_dynamicBLocks[gridXY]);
        } else if (m_startBlocks.find(gridXY) != m_startBlocks.end()) {
            return removeBlock(gridXY, FLOOR_BLOCK_TYPE::START);
        } else if (m_spliceBlocks.find(gridXY) != m_spliceBlocks.end()) {
            return removeBlock(gridXY, FLOOR_BLOCK_TYPE::SPLICE);
        } else if (m_levelBlocks.find(gridXY) != m_levelBlocks.end()) {
            return removeBlock(gridXY, FLOOR_BLOCK_TYPE::LEVEL);
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

    std::unique_ptr<action::Action> Level::removeBlock(const GridXY& gridXY, FLOOR_BLOCK_TYPE blockType) {
        switch (blockType) {
            case FLOOR_BLOCK_TYPE::LEVEL:
                assert(m_levelBlocks.find(gridXY) != m_levelBlocks.end());
                m_levelBlocks.erase(gridXY);
                createBoundaries();
                return std::make_unique<action::RemoveLevelBlockAction>(FLOOR_BLOCK_TYPE::LEVEL, gridXY);
            case FLOOR_BLOCK_TYPE::START:
                assert(m_startBlocks.find(gridXY) != m_startBlocks.end());
                m_startBlocks.erase(gridXY);
                return std::make_unique<action::RemoveLevelBlockAction>(FLOOR_BLOCK_TYPE::START, gridXY);
            case FLOOR_BLOCK_TYPE::SPLICE:
                assert(m_spliceBlocks.find(gridXY) != m_spliceBlocks.end());
                m_spliceBlocks.erase(gridXY);
                return std::make_unique<action::RemoveLevelBlockAction>(FLOOR_BLOCK_TYPE::SPLICE, gridXY);
        }
        return nullptr;
    }

    std::list<Cluster>& Level::stoppedClusters() {
        return m_stoppedClusters;
    }

    const std::list<Cluster>& Level::stoppedClusters() const {
        return m_stoppedClusters;
    }

    Level::Action_u_ptr Level::addSpliceBlock(const GridXY& gridXY) {
        if (m_spliceBlocks.find(gridXY) != m_spliceBlocks.end()) {
            return nullptr;
        }
        if (m_levelBlocks.find(gridXY) == m_levelBlocks.end()) {
            m_levelBlocks.emplace(gridXY);
            createBoundaries();
            m_spliceBlocks.emplace(gridXY);
            return std::unique_ptr<action::Action>(new action::AddLevelBlockAction({FLOOR_BLOCK_TYPE::LEVEL, FLOOR_BLOCK_TYPE::SPLICE}, gridXY));
        }
        m_spliceBlocks.emplace(gridXY);
        return std::unique_ptr<action::Action>(new action::AddLevelBlockAction({FLOOR_BLOCK_TYPE::SPLICE}, gridXY));
    }

    const std::set<GridXY>& Level::spliceBlocks() const {
        return m_spliceBlocks;
    }

} // namespace model