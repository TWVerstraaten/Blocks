//
// Created by pc on 15-12-20.
//

#include "Level.h"

#include "../aux/Aux.h"

#include <cassert>

namespace model {

    Level::DYNAMIC_BLOCK_TYPE Level::dynamicBlockAt(const GridCoordinates& gridCoordinates) {
        if (m_dynamicBLocks.find(gridCoordinates) == m_dynamicBLocks.end()) {
            return DYNAMIC_BLOCK_TYPE::NONE;
        }
        return m_dynamicBLocks[gridCoordinates];
    }

    const std::map<GridCoordinates, Level::DYNAMIC_BLOCK_TYPE>& Level::dynamicBlocks() const {
        return m_dynamicBLocks;
    }

    Level::INSTANT_BLOCK_TYPE Level::instantBlockAt(const GridCoordinates& gridCoordinates) {
        if (m_instantBLocks.find(gridCoordinates) == m_instantBLocks.end()) {
            return INSTANT_BLOCK_TYPE::NONE;
        }
        return m_instantBLocks[gridCoordinates];
    }

    const std::map<GridCoordinates, Level::INSTANT_BLOCK_TYPE>& Level::instantBlocks() const {
        return m_instantBLocks;
    }

    void Level::addBlock(const GridCoordinates& gridCoordinates, Level::DYNAMIC_BLOCK_TYPE blockType) {
        assert(m_dynamicBLocks.find(gridCoordinates) == m_dynamicBLocks.end());
        m_dynamicBLocks[gridCoordinates] = blockType;
    }

    void Level::addBlock(const GridCoordinates& gridCoordinates, Level::INSTANT_BLOCK_TYPE blockType) {
        assert(m_instantBLocks.find(gridCoordinates) == m_instantBLocks.end());
        m_instantBLocks[gridCoordinates] = blockType;
    }

    void Level::addLevelBlock(const GridCoordinates& gridCoordinates) {
        m_levelBlocks.emplace(gridCoordinates);
    }

    void Level::setLevelBlocks(std::set<GridCoordinates>&& levelBlocks) {
        m_levelBlocks = levelBlocks;
    }

    const std::set<GridCoordinates>& Level::levelBlocks() const {
        return m_levelBlocks;
    }

    bool Level::isInLevel(const WorldCoordinates& worldCoordinates) {
        for (const auto& levelBlock : m_levelBlocks) {
            if (aux::pointInBlock(worldCoordinates, levelBlock)) {
                return true;
            }
        }
        return false;
    }

    void Level::clear() {
        m_levelBlocks.clear();
        m_instantBLocks.clear();
        m_dynamicBLocks.clear();
    }

    Level& Level::operator=(const Level& other) {
        m_dynamicBLocks = other.m_dynamicBLocks;
        m_instantBLocks = other.m_instantBLocks;
        m_levelBlocks   = other.m_levelBlocks;
        return *this;
    }

} // namespace model