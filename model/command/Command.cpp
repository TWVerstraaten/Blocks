//
// Created by pc on 31-12-20.
//

#include "Command.h"

#include "../../app/Application_constants.h"
#include "../../global/defines.h"
#include "../Cluster.h"
#include "../Level.h"

static void setTranslating(model::GridXY::DIRECTION direction, model::Cluster& cluster) {
    model::GridXYSet newGridXYSet;
    cluster.setPhase(model::PHASE::TRANSLATING);
    for (auto& idx : cluster.gridXY()) {
        newGridXYSet.emplace(idx.neighbor(direction));
    }
    std::swap(cluster.gridXY(), newGridXYSet);
    switch (direction) {
        case model::GridXY::DIRECTION::UP:
            cluster.setWorldOffset({0, app::BLOCK_SIZE_IN_WORLD});
            break;
        case model::GridXY::DIRECTION::DOWN:
            cluster.setWorldOffset({0, -app::BLOCK_SIZE_IN_WORLD});
            break;
        case model::GridXY::DIRECTION::LEFT:
            cluster.setWorldOffset({app::BLOCK_SIZE_IN_WORLD, 0});
            break;
        case model::GridXY::DIRECTION::RIGHT:
            cluster.setWorldOffset({-app::BLOCK_SIZE_IN_WORLD, 0});
            break;
    }
}

model::Command model::toCommand(const model::Command_RepeatWrapper& e) {
    return std::visit(__FUNC(c, static_cast<Command>(c)), e.command);
}

void model::doAction(const model::Command_Error& command, Cluster& cluster, Level& level) {
}

void model::doAction(const model::Command_Simple& command, Cluster& cluster, Level& level) {
    switch (command.type) {
        case COMMAND_TYPE::FWD:
            setTranslating(GridXY::DIRECTION::UP, cluster);
            break;
        case COMMAND_TYPE::BCK:
            setTranslating(GridXY::DIRECTION::DOWN, cluster);
            break;
        case COMMAND_TYPE::LFT:
            setTranslating(GridXY::DIRECTION::LEFT, cluster);
            break;
        case COMMAND_TYPE::RHT:
            setTranslating(GridXY::DIRECTION::RIGHT, cluster);
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
    std::visit(__FUNC(c, doAction(c, cluster, level)), command.command);
}
