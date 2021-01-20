//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_TOFILE_H
#define BLOCKS_TOFILE_H

#include "../model/Level_enums.h"

#include <QString>

namespace view {

    QString toFile(model::DYNAMIC_BLOCK_TYPE type);

     QString toFile(model::INSTANT_BLOCK_TYPE type);
} // namespace view

#endif // BLOCKS_TOFILE_H
