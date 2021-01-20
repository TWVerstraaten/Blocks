//
// Created by pc on 31-12-20.
//

#include "Command.h"

#include "../../app/Application_constants.h"
#include "../../global/defines.h"
#include "../Cluster.h"
#include "../Level.h"

namespace model {

    static void translate(GridXy::DIRECTION direction, Cluster& cluster) {
        GridXySet newGridXySet;
        cluster.setPhase(PHASE::TRANSLATING);
        for (auto& idx : cluster.gridXy()) {
            newGridXySet.emplace(idx.neighbor(direction));
        }
        cluster.gridXy().swap(newGridXySet);
        switch (direction) {
            case GridXy::DIRECTION::UP:
                cluster.setWorldOffset({0, app::BLOCK_SIZE_IN_WORLD});
                break;
            case GridXy::DIRECTION::DOWN:
                cluster.setWorldOffset({0, -app::BLOCK_SIZE_IN_WORLD});
                break;
            case GridXy::DIRECTION::LEFT:
                cluster.setWorldOffset({app::BLOCK_SIZE_IN_WORLD, 0});
                break;
            case GridXy::DIRECTION::RIGHT:
                cluster.setWorldOffset({-app::BLOCK_SIZE_IN_WORLD, 0});
                break;
        }
    }

    Command toCommand(const Command_RepeatWrapper& repeatWrapper) {
        return std::visit(D_FUNC(command, static_cast<Command>(command)), repeatWrapper.command);
    }

    void doAction([[maybe_unused]] const Command_Error& command, [[maybe_unused]] Cluster& cluster, [[maybe_unused]] Level& level) {
    }

    void doAction([[maybe_unused]] const Command_Label& command, [[maybe_unused]] Cluster& cluster, [[maybe_unused]] Level& level) {
    }

    void doAction([[maybe_unused]] const Command_Jump& command, [[maybe_unused]] Cluster& cluster, [[maybe_unused]] Level& level) {
    }

    void doAction(const Command_Simple& command, Cluster& cluster, Level& level) {
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

    void doAction(const Command_Modified& command, Cluster& cluster, Level& level) {
        doAction(Command_Simple{command.type}, cluster, level);
    }

    void doAction(const Command_RepeatWrapper& command, Cluster& cluster, Level& level) {
        std::visit(D_FUNC(command, doAction(command, cluster, level)), command.command);
    }
} // namespace model