//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_INDEXPAIR_H
#define BLOCKS_INDEXPAIR_H

#include <cstddef>

class IndexPair {

  public:
    explicit IndexPair(int row = 0, int column = 0);
    int row() const;
    int column() const;

  private:
    int m_row;
    int m_column;
};

#endif // BLOCKS_INDEXPAIR_H
