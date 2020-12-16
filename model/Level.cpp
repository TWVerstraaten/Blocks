//
// Created by pc on 15-12-20.
//

#include "Level.h"

#include <cassert>
#include <iostream>

namespace model {
    Level::Level() {
        m_dynamicBLocks[IndexPair(5, 1)]  = DYNAMIC_BLOCK_TYPE::ROTATE_CCW;
        m_dynamicBLocks[IndexPair(10, 6)] = DYNAMIC_BLOCK_TYPE::ROTATE_CW;
        m_dynamicBLocks[IndexPair(5, 4)]  = DYNAMIC_BLOCK_TYPE::ROTATE_CCW;

        m_instantBLocks[IndexPair(5, 6)] = INSTANT_BLOCK_TYPE::KILL;
        m_instantBLocks[IndexPair(7, 2)] = INSTANT_BLOCK_TYPE::KILL;
        m_instantBLocks[IndexPair(4, 4)] = INSTANT_BLOCK_TYPE::KILL;
    }

    Level::DYNAMIC_BLOCK_TYPE Level::dynamicBlockAt(const IndexPair& indexPair) {
        if (m_dynamicBLocks.find(indexPair) == m_dynamicBLocks.end()) {
            return DYNAMIC_BLOCK_TYPE::NONE;
        }
        return m_dynamicBLocks[indexPair];
    }

    const std::map<IndexPair, Level::DYNAMIC_BLOCK_TYPE>& Level::dynamicBlocks() const {
        return m_dynamicBLocks;
    }

    Level::INSTANT_BLOCK_TYPE Level::instantBlockAt(const IndexPair& indexPair) {
        if (m_instantBLocks.find(indexPair) == m_instantBLocks.end()) {
            return INSTANT_BLOCK_TYPE::NONE;
        }
        return m_instantBLocks[indexPair];
    }

    const std::map<IndexPair, Level::INSTANT_BLOCK_TYPE>& Level::instantBlocks() const {
        return m_instantBLocks;
    }

} // namespace model