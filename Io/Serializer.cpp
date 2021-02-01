//
// Created by pc on 01-02-21.
//

#include "Serializer.h"

#include "../model/Level.h"
#include "../model/Model.h"

#include <cassert>

namespace Io {

    using namespace model;

    typedef std::variant<DYNAMIC_BLOCK_TYPE, INSTANT_BLOCK_TYPE, FLOOR_BLOCK_TYPE> AllTypes;

    static const std::map<AllTypes, std::string> s_stringToEnum{{DYNAMIC_BLOCK_TYPE::ROTATE_CCW, "rccw"},
                                                                {DYNAMIC_BLOCK_TYPE::ROTATE_CW, "rcw"},
                                                                {FLOOR_BLOCK_TYPE::LEVEL, "lvl"},
                                                                {FLOOR_BLOCK_TYPE::START, "str"},
                                                                {FLOOR_BLOCK_TYPE::SPLICE, "spl"},
                                                                {FLOOR_BLOCK_TYPE::CONWAY, "cnw"},
                                                                {INSTANT_BLOCK_TYPE::KILL, "kll"}};

    std::string enumToString(AllTypes type) {
        assert(s_stringToEnum.find(type) != s_stringToEnum.end());
        return s_stringToEnum.at(type);
    }

    AllTypes stringToEnum(const std::string& string) {
        for (const auto& [type, str] : s_stringToEnum) {
            if (str == string) {
                return type;
            }
        }
        assert(false);
        return INSTANT_BLOCK_TYPE::KILL;
    }

    std::ostream& operator<<(std::ostream& out, const model::Model& model) {
        out << model.level();
        out << model.clusters().size() << " " << Cluster::maxClusterIndex() << '\n';
        for (const auto& cluster : model.clusters()) {
            out << cluster << '\n';
        }
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const model::Cluster& cluster) {
        out << cluster.name() << " " << cluster.index() << '\n';
        out << cluster.gridXyVector().size() << '\n';
        for (const auto& point : cluster.gridXyVector()) {
            out << point.x() << " " << point.y() << " ";
        }
        out << '\n';
        const auto& commandStrings = cluster.commandVector().strings();
        out << commandStrings.size() << '\n';
        if (not commandStrings.empty()) {
            for (const auto& str : commandStrings) {
                out << str << '\n';
            }
        }
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const model::Level& level) {
        {
            const auto& floorBlocks = level.floorBlocks();
            out << floorBlocks.size() << " ";
            for (const auto& [point, type] : floorBlocks) {
                out << point.x() << " " << point.y() << " " << enumToString(type) << " ";
            }
            out << '\n';
        }
        {
            const auto& dynamicBlocks = level.dynamicBlocks();
            out << dynamicBlocks.size() << " ";
            for (const auto& [point, type] : dynamicBlocks) {
                out << point.x() << " " << point.y() << " " << enumToString(type) << " ";
            }
            out << '\n';
        }
        {
            const auto& instantBlocks = level.instantBlocks();
            out << instantBlocks.size() << " ";
            for (const auto& [point, type] : instantBlocks) {
                out << point.x() << " " << point.y() << " " << enumToString(type) << " ";
            }
            out << '\n';
        }
        {
            const auto& stoppedClusters = level.stoppedClusters();
            out << stoppedClusters.size() << " ";
            for (const auto& cluster : stoppedClusters) {
                out << cluster << "\n";
            }
            out << '\n';
        }
        return out;
    }

    std::istream& operator>>(std::istream& in, Level& level) {
        level.clear();

        int         x, y;
        std::string name;
        {
            size_t floorBlockCount;
            in >> floorBlockCount;
            for (size_t i = 0; i != floorBlockCount; ++i) {
                in >> x >> y >> name;
                std::visit([&](const auto& a) { level.addBlock({x, y}, a); }, stringToEnum(name));
            }
        }
        {
            size_t dynamicBlockCount;
            in >> dynamicBlockCount;
            for (size_t i = 0; i != dynamicBlockCount; ++i) {
                in >> x >> y >> name;
                std::visit([&](const auto& a) { level.addBlock({x, y}, a); }, stringToEnum(name));
            }
        }
        {
            size_t instantBlockCount;
            in >> instantBlockCount;
            for (size_t i = 0; i != instantBlockCount; ++i) {
                in >> x >> y >> name;
                std::visit([&](const auto& a) { level.addBlock({x, y}, a); }, stringToEnum(name));
            }
        }
        {
            size_t stoppedClusterCount;
            in >> stoppedClusterCount;
            level.m_stoppedClusters.reserve(stoppedClusterCount);
            Cluster c;
            for (size_t i = 0; i != stoppedClusterCount; ++i) {
                in >> c;
                level.m_stoppedClusters.emplace_back(std::move(c));
            }
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, Model& model) {
        model.clear();
        in >> model.level();
        size_t clusterCount;
        size_t maxClusterIndex;
        in >> clusterCount >> maxClusterIndex;
        for (size_t i = 0; i != clusterCount; ++i) {
            Cluster c;
            in >> c;
            model.m_clusters.emplace_back(std::move(c));
        }
        Cluster::setMaxClusterIndex(maxClusterIndex);
        return in;
    }

    std::istream& operator>>(std::istream& in, Cluster& cluster) {
        cluster.m_gridXyVector.clear();
        cluster.commandVector().clear();
        std::string name;
        size_t      index;
        size_t      gridXyCount;
        in >> name >> index >> gridXyCount;
        cluster.m_name  = name;
        cluster.m_index = index;
        cluster.m_gridXyVector.reserve(gridXyCount);
        int x, y;
        for (size_t i = 0; i != gridXyCount; ++i) {
            in >> x >> y;
            cluster.m_gridXyVector.emplace_back(GridXy{x, y});
        }
        size_t commandStringCount;
        in >> commandStringCount;
        std::vector<std::string> commandStrings;
        commandStrings.reserve(commandStringCount);
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        for (size_t i = 0; i != commandStringCount; ++i) {
            commandStrings.emplace_back("");
            getline(in, commandStrings.back());
        }
        cluster.m_commandVector.set(commandStrings);
        return in;
    }
} // namespace Io