//
// Created by teunv on 1/19/2021.
//

#ifndef BLOCKS_PHASETRANSFORMATION_H
#define BLOCKS_PHASETRANSFORMATION_H

#include "WorldXy.h"

#include <functional>

namespace model {

    typedef std::function<WorldXy(const WorldXy&)> PhaseTransformation;

}

#endif // BLOCKS_PHASETRANSFORMATION_H
