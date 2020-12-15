//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_VIEW_H
#define BLOCKS_VIEW_H

#include "Grid.h"

#include <iostream>
#include <vector>

namespace model {
    class Model;
    class Cluster;
    class Level;
} // namespace model

class SDL_Window;
class SDL_Renderer;

namespace view {
    class View {
      public:
        View();
        ~View();

        void draw(const model::Model& model);
        void zoom(int amount);

      private:
        void drawClusters(const std::vector<model::Cluster>& clusters) const;
        void drawLevel(const model::Level& level) const;
        void drawGridLines() const;

        int           m_zoomParameter = 0;
        SDL_Window*   m_window        = nullptr;
        SDL_Renderer* m_renderer      = nullptr;
        Grid          m_grid;
    };
} // namespace view

#endif // BLOCKS_VIEW_H
