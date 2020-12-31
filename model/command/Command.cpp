//
// Created by pc on 31-12-20.
//

#include "Command.h"

#include "../../global/cst.h"
#include "../../global/overloaded.h"
#include "../Cluster.h"
#include "../Level.h"

model::Command model::toCommand(const model::Command_RepeatWrapper& e) {
    return std::visit(overloaded{[](const auto& c) { return static_cast<Command>(c); }}, e.command);
}

void model::doAction(const model::Command_Error& command, Cluster& cluster, Level& level) {
}

void model::doAction(const model::Command_Simple& command, Cluster& cluster, Level& level) {
    std::set<GridXY> newGridXYSet;
    switch (command.type) {
        case COMMAND_TYPE::FWD:
            cluster.m_phase = PHASE::TRANSLATING;
            for (auto& idx : cluster.m_gridXYVector) {
                newGridXYSet.emplace(idx.neighbor(model::GridXY::DIRECTION::UP));
            }
            std::swap(cluster.m_gridXYVector, newGridXYSet);
            cluster.m_worldOffset = {0, cst::BLOCK_SIZE_IN_WORLD};
            break;
        case COMMAND_TYPE::BCK:
            cluster.m_phase = PHASE::TRANSLATING;
            for (auto& idx : cluster.m_gridXYVector) {
                newGridXYSet.emplace(idx.neighbor(model::GridXY::DIRECTION::DOWN));
            }
            std::swap(cluster.m_gridXYVector, newGridXYSet);
            cluster.m_worldOffset = {0, -cst::BLOCK_SIZE_IN_WORLD};
            break;
        case COMMAND_TYPE::LFT:
            cluster.m_phase = PHASE::TRANSLATING;
            for (auto& idx : cluster.m_gridXYVector) {
                newGridXYSet.emplace(idx.neighbor(model::GridXY::DIRECTION::LEFT));
            }
            std::swap(cluster.m_gridXYVector, newGridXYSet);
            cluster.m_worldOffset = {cst::BLOCK_SIZE_IN_WORLD, 0};
            break;
        case COMMAND_TYPE::RHT:
            cluster.m_phase = PHASE::TRANSLATING;
            for (auto& idx : cluster.m_gridXYVector) {
                newGridXYSet.emplace(idx.neighbor(model::GridXY::DIRECTION::RIGHT));
            }
            std::swap(cluster.m_gridXYVector, newGridXYSet);
            cluster.m_worldOffset = {-cst::BLOCK_SIZE_IN_WORLD, 0};
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

