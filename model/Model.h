//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_MODEL_H
#define BLOCKS_MODEL_H

#include "Cluster.h"
#include "Level.h"

#include <vector>

namespace io {
    std::istream& operator>>(std::istream& in, model::Model& model);
}

namespace model {

    class Model {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Model()                   = default;
        Model(const Model& other) = default;
        Model(Model&& other)      = default;
        Model& operator=(const Model& other) = default;

        /****** CONST GETTERS  ******/
        [[nodiscard]] const Level&                level() const;
        [[nodiscard]] const std::vector<Cluster>& clusters() const;

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] bool noLiveOrStoppedClusterOnBlock(const GridXy& gridXy) const;

        /****** NON CONST FUNCTIONS  ******/
        void                           init(const std::string& path);
        void                           clear();
        void                           resetPhase();
        void                           update(double dPhase);
        void                           clearEmpty();
        void                           clearStopped();
        void                           handleStoppedAndSplitClusters();
        void                           splitDisconnectedClusters();
        Level&                         level();
        std::vector<Cluster>&          clusters();
        std::vector<Cluster>::iterator clusterWithIndex(size_t index);
        std::vector<Cluster>::iterator clusterContaining(const GridXy& point);
        std::vector<Cluster>::iterator stoppedClusterContaining(const GridXy& point);

      private:
        friend std::istream& io::operator>>(std::istream& in, model::Model& model);

        /****** PRIVATE NON CONST FUNCTIONS  ******/
        void intersectWithLevel();
        void intersectClusters();
        void initEmpty();
        void updateInternal(double dPhase);

        /****** DATA MEMBERS  ******/
        double               m_phaseFraction = 0.0;
        Level                m_level;
        std::vector<Cluster> m_clusters;
    };

} // namespace model

#endif // BLOCKS_MODEL_H
