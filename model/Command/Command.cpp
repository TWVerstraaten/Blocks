//
// Created by pc on 31-12-20.
//

#include "Command.h"

#include "../../global/overloaded.h"
#include "../Cluster.h"
#include "../Level.h"

model::Command model::toCommand(const model::Command_RepeatWrapper& e) {
    return std::visit(overloaded{[](const auto& c) { return static_cast<Command>(c); }}, e.command);
}

void model::doAction(const model::Command_Simple& command, Cluster& cluster, Level& level) {
    switch (command.type) {
        case COMMAND_TYPE::SKP:
            return;
        case COMMAND_TYPE::FWD:
            break;
        case COMMAND_TYPE::BCK:
            break;
        case COMMAND_TYPE::LFT:
            break;
        case COMMAND_TYPE::RHT:
            break;
        case COMMAND_TYPE::STP:
            break;
        case COMMAND_TYPE::GRB:
            break;
        case COMMAND_TYPE::SPL:
            break;
    }
}

void model::doAction(const model::Command_Modified& command, Cluster& cluster, Level& level) {
}

void model::doAction(const model::Command_RepeatWrapper& command, Cluster& cluster, Level& level) {
    std::visit(overloaded{[&](const auto& c) { doAction(c, cluster, level); }}, command.command);
}
