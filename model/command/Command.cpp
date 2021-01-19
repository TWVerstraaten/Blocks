//
// Created by pc on 31-12-20.
//

#include "Command.h"

#include "../../app/Application_constants.h"
#include "../../global/defines.h"
#include "../Cluster.h"
#include "../Level.h"

static void translate(model::GridXy::DIRECTION direction, model::Cluster& cluster) {
    model::GridXySet newGridXySet;
    cluster.setPhase(model::PHASE::TRANSLATING);
    for (auto& idx : cluster.gridXy()) {
        newGridXySet.emplace(idx.neighbor(direction));
    }
    cluster.gridXy().swap(newGridXySet);
    switch (direction) {
        case model::GridXy::DIRECTION::UP:
            cluster.setWorldOffset({0, app::BLOCK_SIZE_IN_WORLD});
            break;
        case model::GridXy::DIRECTION::DOWN:
            cluster.setWorldOffset({0, -app::BLOCK_SIZE_IN_WORLD});
            break;
        case model::GridXy::DIRECTION::LEFT:
            cluster.setWorldOffset({app::BLOCK_SIZE_IN_WORLD, 0});
            break;
        case model::GridXy::DIRECTION::RIGHT:
            cluster.setWorldOffset({-app::BLOCK_SIZE_IN_WORLD, 0});
            break;
    }
}

model::Command model::toCommand(const model::Command_RepeatWrapper& repeatWrapper) {
    return std::visit(D_FUNC(command, static_cast<Command>(command)), repeatWrapper.command);
}

void model::doAction([[maybe_unused]] const model::Command_Error& command, [[maybe_unused]] Cluster& cluster, [[maybe_unused]] Level& level) {
}

void model::doAction([[maybe_unused]] const model::Command_Label& command, [[maybe_unused]] Cluster& cluster, [[maybe_unused]] Level& level) {
}

void model::doAction([[maybe_unused]] const model::Command_Jump& command, [[maybe_unused]] Cluster& cluster, [[maybe_unused]] Level& level) {
}

void model::doAction(const model::Command_Simple& command, Cluster& cluster, Level& level) {
    switch (command.type) {
        case COMMAND_TYPE::FWD:
            translate(GridXy::DIRECTION::UP, cluster);
            break;
        case COMMAND_TYPE::BCK:
            translate(GridXy::DIRECTION::DOWN, cluster);
            break;
        case COMMAND_TYPE::LFT:
            translate(GridXy::DIRECTION::LEFT, cluster);
            break;
        case COMMAND_TYPE::RHT:
            translate(GridXy::DIRECTION::RIGHT, cluster);
            break;
        case COMMAND_TYPE::GRB:
            cluster.grabAdjacentStoppedClusters(level);
            break;
        case COMMAND_TYPE::SKP:
        case COMMAND_TYPE::STP:
        case COMMAND_TYPE::SPL:
        case COMMAND_TYPE::NONE:
            break;
    }
}

void model::doAction(const model::Command_Modified& command, Cluster& cluster, Level& level) {
    doAction(Command_Simple{command.type}, cluster, level);
}

void model::doAction(const model::Command_RepeatWrapper& command, Cluster& cluster, Level& level) {
    std::visit(D_FUNC(command, doAction(command, cluster, level)), command.command);
}
