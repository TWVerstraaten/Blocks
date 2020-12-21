//
// Created by pc on 15-12-20.
//

#include "Level.h"

#include <cassert>

namespace model {

    Level::DYNAMIC_BLOCK_TYPE Level::dynamicBlockAt(const GridCoordinates& indexPair) {
        if (m_dynamicBLocks.find(indexPair) == m_dynamicBLocks.end()) {
            return DYNAMIC_BLOCK_TYPE::NONE;
        }
        return m_dynamicBLocks[indexPair];
    }

    const std::map<GridCoordinates, Level::DYNAMIC_BLOCK_TYPE>& Level::dynamicBlocks() const {
        return m_dynamicBLocks;
    }

    Level::INSTANT_BLOCK_TYPE Level::instantBlockAt(const GridCoordinates& indexPair) {
        if (m_instantBLocks.find(indexPair) == m_instantBLocks.end()) {
            return INSTANT_BLOCK_TYPE::NONE;
        }
        return m_instantBLocks[indexPair];
    }

    const std::map<GridCoordinates, Level::INSTANT_BLOCK_TYPE>& Level::instantBlocks() const {
        return m_instantBLocks;
    }

    void Level::addBlock(const GridCoordinates& indexPair, Level::DYNAMIC_BLOCK_TYPE blockType) {
        assert(m_dynamicBLocks.find(indexPair) == m_dynamicBLocks.end());
        m_dynamicBLocks[indexPair] = blockType;
    }

    void Level::addBlock(const GridCoordinates& indexPair, Level::INSTANT_BLOCK_TYPE blockType) {
        assert(m_instantBLocks.find(indexPair) == m_instantBLocks.end());
        m_instantBLocks[indexPair] = blockType;
    }

} // namespace model