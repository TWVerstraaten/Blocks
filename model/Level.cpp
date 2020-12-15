//
// Created by pc on 15-12-20.
//

#include "Level.h"

#include <cassert>

namespace model {
    Level::Level() {
        m_blocks.emplace_back(IndexPair(5, 3), BLOCK_TYPE::ROTATE_CW);
        m_blocks.emplace_back(IndexPair(5, 5), BLOCK_TYPE::ROTATE_CCW);
        m_blocks.emplace_back(IndexPair(2, 2), BLOCK_TYPE::KILL);
    }

    size_t Level::blockCount() const {
        return m_blocks.size();
    }

    const Level::Block& Level::blockAt(size_t index) const {
        assert(index < blockCount());
        return m_blocks.at(index);
    }

    Level::BLOCK_TYPE Level::blockAt(int row, int column) {
        for (const auto& block : m_blocks) {
            if (block.first.row() == row && block.first.column() == column) {
                return block.second;
            }
        }

        return Level::BLOCK_TYPE::NONE;
    }

    Level::BLOCK_TYPE Level::blockAt(const IndexPair& indexPair) {
        return blockAt(indexPair.row(), indexPair.column());
    }

} // namespace model