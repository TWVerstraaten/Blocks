//
// Created by pc on 30-12-20.
//

#ifndef BLOCKS_COMMAND_REPEATWRAPPER_H
#define BLOCKS_COMMAND_REPEATWRAPPER_H

#include "Command_Modified.h"
#include "Command_Simple.h"

#include <variant>

namespace model {

    struct Command_RepeatWrapper {
        int                                            repeatCount;
        std::variant<Command_Simple, Command_Modified> command;
    };

} // namespace model

#endif // BLOCKS_COMMAND_REPEATWRAPPER_H
