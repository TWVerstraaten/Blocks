//
// Created by pc on 30-12-20.
//

#ifndef BLOCKS_COMMAND_H
#define BLOCKS_COMMAND_H

#include "Command_Error.h"
#include "Command_Jump.h"
#include "Command_Label.h"
#include "Command_Modified.h"
#include "Command_RepeatWrapper.h"
#include "Command_Simple.h"

#include <variant>

namespace model {

    typedef std::variant<Command_Simple, Command_Modified, Command_RepeatWrapper, Command_Label, Command_Jump, Command_Error> Command;

    Command toCommand(const Command_RepeatWrapper& repeatWrapper);

    class Cluster;
    class Level;

    void doAction(const Command_Error& command, Cluster& cluster, Level& level);
    void doAction(const Command_Simple& command, Cluster& cluster, Level& level);
    void doAction(const Command_Jump& command, Cluster& cluster, Level& level);
    void doAction(const Command_Label& command, Cluster& cluster, Level& level);
    void doAction(const Command_Modified& command, Cluster& cluster, Level& level);
    void doAction(const Command_RepeatWrapper& command, Cluster& cluster, Level& level);

} // namespace m_model

#endif // BLOCKS_COMMAND_H
