//
// Created by pc on 15-12-20.
//

#include "Level.h"

#include "../global/alg.h"
#include "../global/fns.h"

#include <algorithm>
#include <cassert>
#include <vector>

namespace model {

    Level::DYNAMIC_BLOCK_TYPE Level::dynamicBlockAt(const GridXY& gridXY) {
        if (m_dynamicBLocks.find(gridXY) == m_dynamicBLocks.end()) {
            return DYNAMIC_BLOCK_TYPE::NONE;
        }
        return m_dynamicBLocks[gridXY];
    }

    const std::map<GridXY, Level::DYNAMIC_BLOCK_TYPE>& Level::dynamicBlocks() const {
        return m_dynamicBLocks;
    }

    Level::INSTANT_BLOCK_TYPE Level::instantBlockAt(const GridXY& gridXY) {
        if (m_instantBLocks.find(gridXY) == m_instantBLocks.end()) {
            return INSTANT_BLOCK_TYPE::NONE;
        }
        return m_instantBLocks[gridXY];
    }

    const std::map<GridXY, Level::INSTANT_BLOCK_TYPE>& Level::instantBlocks() const {
        return m_instantBLocks;
    }

    void Level::addBlock(const GridXY& gridXY, Level::DYNAMIC_BLOCK_TYPE blockType) {
        assert(m_dynamicBLocks.find(gridXY) == m_dynamicBLocks.end());
        m_dynamicBLocks[gridXY] = blockType;
    }

    void Level::addBlock(const GridXY& gridXY, Level::INSTANT_BLOCK_TYPE blockType) {
        assert(m_instantBLocks.find(gridXY) == m_instantBLocks.end());
        m_instantBLocks[gridXY] = blockType;
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
    bool Level::isFreeStartBlock(const GridXY gridXY) const {
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

    void Level::sort() {
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

} // namespace model