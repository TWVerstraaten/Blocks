//
// Created by pc on 15-12-20.
//

#include "Level.h"

#include <cassert>

namespace model {
    Level::Level() {
        m_blocks[IndexPair(5, 1)]  = BLOCK_TYPE::ROTATE_CW;
        m_blocks[IndexPair(4, 2)]  = BLOCK_TYPE::ROTATE_CW;
        m_blocks[IndexPair(10, 6)] = BLOCK_TYPE::ROTATE_CW;
        m_blocks[IndexPair(5, 6)]  = BLOCK_TYPE::KILL;
    }

    Level::BLOCK_TYPE Level::blockAt(const IndexPair& indexPair) {
        if (m_blocks.find(indexPair) == m_blocks.end()) {
            return BLOCK_TYPE::NONE;
        }
        return m_blocks[indexPair];
    }

    const std::map<IndexPair, Level::BLOCK_TYPE>& Level::blocks() const {
        return m_blocks;
    }

} // namespace model