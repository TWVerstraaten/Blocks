//
// Created by pc on 15-12-20.
//

#include "Level.h"

#include "../action/AddLevelBlockAction.h"
#include "../action/RemoveLevelBlockAction.h"
#include "../global/alg.h"
#include "../global/fns.h"
#include "../global/overloaded.h"

namespace model {

    DYNAMIC_BLOCK_TYPE Level::dynamicBlockAt(const GridXY& gridXY) const {
        if (m_dynamicBLocks.find(gridXY) == m_dynamicBLocks.end()) {
            return DYNAMIC_BLOCK_TYPE::NONE;
        }
        return m_dynamicBLocks.at(gridXY);
    }

    const std::map<GridXY, DYNAMIC_BLOCK_TYPE>& Level::dynamicBlocks() const {
        return m_dynamicBLocks;
    }

    INSTANT_BLOCK_TYPE Level::instantBlockAt(const GridXY& gridXY) const {
        if (m_instantBLocks.find(gridXY) == m_instantBLocks.end()) {
            return INSTANT_BLOCK_TYPE::NONE;
        }
        return m_instantBLocks.at(gridXY);
    }

    const std::map<GridXY, INSTANT_BLOCK_TYPE>& Level::instantBlocks() const {
        return m_instantBLocks;
    }

    std::unique_ptr<action::Action> Level::addBlock(const GridXY& gridXY, DYNAMIC_BLOCK_TYPE blockType) {
        bool levelBlockAdded = false;
        if (m_levelBlocks.find(gridXY) == m_levelBlocks.end()) {
            levelBlockAdded = true;
            addBlock(gridXY, model::FLOOR_BLOCK_TYPE::LEVEL);
        }
        if (m_dynamicBLocks.find(gridXY) != m_dynamicBLocks.end() && m_dynamicBLocks[gridXY] == blockType) {
            assert(not levelBlockAdded);
            return nullptr;
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
        if (m_levelBlocks.find(gridXY) == m_levelBlocks.end()) {
            levelBlockAdded = true;
            addBlock(gridXY, model::FLOOR_BLOCK_TYPE::LEVEL);
        }
        if (m_instantBLocks.find(gridXY) != m_instantBLocks.end() && m_instantBLocks[gridXY] == blockType) {
            assert(not levelBlockAdded);
            return nullptr;
        }
        m_instantBLocks[gridXY] = blockType;
        if (levelBlockAdded) {
            return std::unique_ptr<action::Action>(new action::AddLevelBlockAction({blockType, FLOOR_BLOCK_TYPE::LEVEL}, gridXY));
        } else {
            return std::unique_ptr<action::Action>(new action::AddLevelBlockAction({blockType}, gridXY));
        }
    }

    void Level::addLevelBlock(const GridXY& gridXY) {
        m_levelBlocks.emplace(gridXY);
    }

    void Level::addStartBlock(const GridXY& gridXY) {
        m_startBlocks.emplace(gridXY);
    }

    const std::set<GridXY>& Level::levelBlocks() const {
        return m_levelBlocks;
    }

    bool Level::isInLevel(const WorldXY& worldXY) const {
        return m_levelBlocks.find(GridXY::fromWorldXY(worldXY)) != m_levelBlocks.end();
    }

    void Level::clear() {
        m_levelBlocks.clear();
        m_startBlocks.clear();
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
        m_boundaries = alg::getSidesInGridXY<WorldXY>(m_levelBlocks);
    }

    std::unique_ptr<action::Action> Level::addBlock(const GridXY& gridXY, FLOOR_BLOCK_TYPE blockType) {
        bool levelBlockAdded = false;
        switch (blockType) {
            case FLOOR_BLOCK_TYPE::LEVEL:
                m_levelBlocks.emplace(gridXY);
                break;
            case FLOOR_BLOCK_TYPE::START:
                levelBlockAdded = true;
                m_levelBlocks.emplace(gridXY);
                m_startBlocks.emplace(gridXY);
                break;
        }
        createBoundaries();
        if (levelBlockAdded) {
            return std::unique_ptr<action::Action>(new action::AddLevelBlockAction({blockType, FLOOR_BLOCK_TYPE::LEVEL}, gridXY));
        } else {
            return std::unique_ptr<action::Action>(new action::AddLevelBlockAction({blockType}, gridXY));
        }
    }

    std::unique_ptr<action::Action> Level::removeBlock(const GridXY& gridXY) {
        if (m_instantBLocks.find(gridXY) != m_instantBLocks.end()) {
            const auto type = m_instantBLocks[gridXY];
            m_instantBLocks.erase(gridXY);
            assert(m_instantBLocks.find(gridXY) == m_instantBLocks.end());
            return std::make_unique<action::RemoveLevelBlockAction>(type, gridXY);
        } else if (m_dynamicBLocks.find(gridXY) != m_dynamicBLocks.end()) {
            const auto type = m_instantBLocks[gridXY];
            m_dynamicBLocks.erase(gridXY);
            assert(m_dynamicBLocks.find(gridXY) == m_dynamicBLocks.end());
            return std::make_unique<action::RemoveLevelBlockAction>(type, gridXY);
        } else if (m_startBlocks.find(gridXY) != m_startBlocks.end()) {
            m_startBlocks.erase(gridXY);
            return std::make_unique<action::RemoveLevelBlockAction>(FLOOR_BLOCK_TYPE::START, gridXY);
        } else if (m_levelBlocks.find(gridXY) != m_levelBlocks.end()) {
            m_levelBlocks.erase(gridXY);
            createBoundaries();
            return std::make_unique<action::RemoveLevelBlockAction>(FLOOR_BLOCK_TYPE::LEVEL, gridXY);
        }
        return nullptr;
    }

    void Level::removeBlock(const GridXY& gridXY, DYNAMIC_BLOCK_TYPE blockType) {
        assert(m_dynamicBLocks.find(gridXY) != m_dynamicBLocks.end() && m_dynamicBLocks[gridXY] == blockType);
        m_dynamicBLocks.erase(gridXY);
    }

    void Level::removeBlock(const GridXY& gridXY, INSTANT_BLOCK_TYPE blockType) {
        assert(m_instantBLocks.find(gridXY) != m_instantBLocks.end() && m_instantBLocks[gridXY] == blockType);
        m_instantBLocks.erase(gridXY);
    }

    void Level::removeBlock(const GridXY& gridXY, FLOOR_BLOCK_TYPE blockType) {
        switch (blockType) {
            case FLOOR_BLOCK_TYPE::LEVEL:
                assert(m_levelBlocks.find(gridXY) != m_levelBlocks.end());
                m_levelBlocks.erase(gridXY);
                createBoundaries();
                break;
            case FLOOR_BLOCK_TYPE::START:
                assert(m_startBlocks.find(gridXY) != m_startBlocks.end());
                m_startBlocks.erase(gridXY);
                break;
        }
    }

} // namespace model