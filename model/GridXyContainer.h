//
// Created by pc on 08-02-21.
//

#ifndef BLOCKS_GRIDXYCONTAINER_H
#define BLOCKS_GRIDXYCONTAINER_H

#include "Model_typedefs.h"
#include "PhaseTransformation.h"

namespace model {

    class Level;

    class GridXyContainer {

      public:
        GridXyContainer() = default;
        GridXyContainer(const GridXy& gridXy);
        GridXyContainer(GridXyVector&& gridXy);

        [[nodiscard]] bool                gridXyIsAdjacent(const GridXy& point) const;
        [[nodiscard]] bool                contains(const GridXy& gridXy) const;
        [[nodiscard]] bool                noDuplicates() const;
        [[nodiscard]] bool                isEmpty() const;
        [[nodiscard]] bool                isAdjacent(const GridXyContainer& other) const;
        [[nodiscard]] bool                isConnected() const;
        [[nodiscard]] size_t              size() const;
        [[nodiscard]] const GridXyVector& gridXyVector() const;
        [[nodiscard]] WorldLineVector     sides(int shrinkInWorld, const PhaseTransformation& f) const;
        [[nodiscard]] GridXyContainer     grabAllButFirstComponent();

        void copyGridXy(const GridXyContainer& other);
        void addGridXy(const GridXy& gridXy);
        void clearGridXy();
        void removeGridXy(const GridXy& gridXy);
        void sortGridXy();
        void swapGridXy(GridXyVector& other);
        void addGridXy(const GridXyVector& other);
        void rotateClockWiseAbout(const GridXy& pivotGridXy);
        void rotateCounterClockWiseAbout(const GridXy& pivotGridXy);
        void buildSides();
        void grabAdjacentStoppedClusters(Level& level);

      protected:
        bool            m_gridXyAreSorted = false;
        bool            m_sidesAreCorrect = false;
        GridXyVector    m_gridXyVector;
        WorldLineVector m_sides;
    };

} // namespace model

#endif // BLOCKS_GRIDXYCONTAINER_H
