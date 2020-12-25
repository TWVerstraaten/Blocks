//
// Created by pc on 15-12-20.
//

#include "Level.h"

#include "../global/Global.h"

#include <algorithm>
#include <cassert>

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

    void Level::buildBoundaries() {
        for (const auto& it : m_levelBlocks) {
            std::cout << it.x() << " " << it.y() << '\n';
        }

        auto it = m_levelBlocks.begin();
        while (it != m_levelBlocks.end()) {
            if (not contains(*it + GridXY{0, -1})) {
                const GridXY start = *it;
                int          idx   = it->x() + 1;
                ++it;
                while (it != m_levelBlocks.end() && it->x() == idx && it->y() == start.y() && (not contains(*it + GridXY{0, -1}))) {
                    idx = it->x() + 1;
                    ++it;
                }
                m_boundaries.emplace(start, GridXY{idx, start.y()});
            } else {
                ++it;
            }
        }

        it = m_levelBlocks.begin();
        while (it != m_levelBlocks.end()) {
            if (not contains(*it + GridXY{0, 1})) {
                const GridXY start = *it;
                int          idx   = it->x() + 1;
                ++it;
                while (it != m_levelBlocks.end() && it->x() == idx && it->y() == start.y() && (not contains(*it + GridXY{0, 1}))) {
                    idx = it->x() + 1;
                    ++it;
                }
                m_boundaries.emplace(start + GridXY{0, 1}, GridXY{idx, start.y() + 1});
            } else {
                ++it;
            }
        }
    }

    const std::set<WorldLine>& Level::boundaries() const {
        return m_boundaries;
    }

    bool Level::contains(const GridXY& gridXY) const {
        return m_levelBlocks.find(gridXY) != m_levelBlocks.end();
    }

} // namespace model