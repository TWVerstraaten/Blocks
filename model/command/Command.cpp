//
// Created by pc on 31-12-20.
//

#include "Command.h"

#include "../../global/cst.h"
#include "../../global/overloaded.h"
#include "../Cluster.h"
#include "../Level.h"

static void setTranslating(model::GridXY::DIRECTION direction, model::Cluster& cluster) {
    std::set<model::GridXY> newGridXYSet;
    cluster.setPhase(model::PHASE::TRANSLATING);
    for (auto& idx : cluster.gridXY()) {
        newGridXYSet.emplace(idx.neighbor(direction));
    }
    std::swap(cluster.gridXY(), newGridXYSet);
    switch (direction) {
        case model::GridXY::DIRECTION::UP:
            cluster.setWorldOffset({0, cst::BLOCK_SIZE_IN_WORLD});
            break;
        case model::GridXY::DIRECTION::DOWN:
            cluster.setWorldOffset({0, -cst::BLOCK_SIZE_IN_WORLD});
            break;
        case model::GridXY::DIRECTION::LEFT:
            cluster.setWorldOffset({cst::BLOCK_SIZE_IN_WORLD, 0});
            break;
        case model::GridXY::DIRECTION::RIGHT:
            cluster.setWorldOffset({-cst::BLOCK_SIZE_IN_WORLD, 0});
            break;
    }
}

model::Command model::toCommand(const model::Command_RepeatWrapper& e) {
    return std::visit(overloaded{[](const auto& c) { return static_cast<Command>(c); }}, e.command);
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
        case COMMAND_TYPE::SKP:
            break;
        case COMMAND_TYPE::STP:
            break;
        case COMMAND_TYPE::GRB:
            cluster.grabAdjacentStoppedClusters(level);
            break;
        case COMMAND_TYPE::SPL:
            break;
        case COMMAND_TYPE::NONE:
            break;
    }
}

void model::doAction(const model::Command_Modified& command, Cluster& cluster, Level& level) {
    doAction(Command_Simple{command.type}, cluster, level);
}

void model::doAction(const model::Command_RepeatWrapper& command, Cluster& cluster, Level& level) {
    std::visit(overloaded{[&](const auto& c) { doAction(c, cluster, level); }}, command.command);
}
