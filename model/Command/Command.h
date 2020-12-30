//
// Created by pc on 30-12-20.
//

#ifndef BLOCKS_COMMAND_H
#define BLOCKS_COMMAND_H

#include "Command_Modified.h"
#include "Command_RepeatWrapper.h"
#include "Command_Simple.h"

#include <variant>

namespace model {

    typedef std::variant<Command_Simple, Command_Modified, Command_RepeatWrapper> Command;

}
#endif // BLOCKS_COMMAND_H
