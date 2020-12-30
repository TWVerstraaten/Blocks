//
// Created by pc on 30-12-20.
//

#ifndef BLOCKS_COMMAND_MODIFIED_H
#define BLOCKS_COMMAND_MODIFIED_H

#include "Command_enums.h"

namespace model {

    struct Command_Modified {

        COMMAND_TYPE     type;
        COMMAND_MODIFIER modifier;
    };

} // namespace model
#endif // BLOCKS_COMMAND_MODIFIED_H
