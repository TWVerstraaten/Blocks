//
// Created by pc on 15-12-20.
//

#include "Level.h"

#include "../aux/Aux.h"

#include <algorithm>
#include <cassert>

namespace model {

    Level::DYNAMIC_BLOCK_TYPE Level::dynamicBlockAt(const GridXY& gridCoordinates) {
        if (m_dynamicBLocks.find(gridCoordinates) == m_dynamicBLocks.end()) {
            return DYNAMIC_BLOCK_TYPE::NONE;
        }
        return m_dynamicBLocks[gridCoordinates];
    }

    const std::map<GridXY, Level::DYNAMIC_BLOCK_TYPE>& Level::dynamicBlocks() const {
        return m_dynamicBLocks;
    }

    Level::INSTANT_BLOCK_TYPE Level::instantBlockAt(const GridXY& gridCoordinates) {
        if (m_instantBLocks.find(gridCoordinates) == m_instantBLocks.end()) {
            return INSTANT_BLOCK_TYPE::NONE;
        }
        return m_instantBLocks[gridCoordinates];
    }

    const std::map<GridXY, Level::INSTANT_BLOCK_TYPE>& Level::instantBlocks() const {
        return m_instantBLocks;
    }

    void Level::addBlock(const GridXY& gridCoordinates, Level::DYNAMIC_BLOCK_TYPE blockType) {
        assert(m_dynamicBLocks.find(gridCoordinates) == m_dynamicBLocks.end());
        m_dynamicBLocks[gridCoordinates] = blockType;
    }

    void Level::addBlock(const GridXY& gridCoordinates, Level::INSTANT_BLOCK_TYPE blockType) {
        assert(m_instantBLocks.find(gridCoordinates) == m_instantBLocks.end());
        m_instantBLocks[gridCoordinates] = blockType;
    }

    void Level::addLevelBlock(const GridXY& gridCoordinates) {
        m_levelBlocks.emplace(gridCoordinates);
    }

    void Level::setLevelBlocks(std::set<GridXY>&& levelBlocks) {
        m_levelBlocks = levelBlocks;
    }

    const std::set<GridXY>& Level::levelBlocks() const {
        return m_levelBlocks;
    }

    bool Level::isInLevel(const WorldXY& worldCoordinates) {
        return std::any_of(m_levelBlocks.begin(), m_levelBlocks.end(), [&](const GridXY& gridCoordinates) {
            return aux::pointInBlock(worldCoordinates, gridCoordinates);
        });
    }

    void Level::clear() {
        m_levelBlocks.clear();
        m_instantBLocks.clear();
        m_dynamicBLocks.clear();
    }


} // namespace model