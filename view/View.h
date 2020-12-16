//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_VIEW_H
#define BLOCKS_VIEW_H

#include "ActionEditBox.h"
#include "AssetHandler.h"
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
        View(const View& other) = delete;

        void draw(const model::Model& model);
        void zoom(int amount);
        void translate(int dx, int dy);
        void addActionEditBox(const model::Cluster& cluster);

        const std::vector<std::unique_ptr<ActionEditBox>>& actionEditBoxes() const;

        std::pair<int, int> windowSize() const;

      private:
        void drawClusters(const std::vector<model::Cluster>& clusters) const;
        void drawLevel(const model::Level& level) const;
        void drawGridLines() const;

        int                                         m_zoomParameter = 0;
        SDL_Window*                                 m_window        = nullptr;
        SDL_Renderer*                               m_renderer      = nullptr;
        Grid                                        m_grid;
        std::unique_ptr<AssetHandler>               m_assetsHandler{new AssetHandler()};
        std::vector<std::unique_ptr<ActionEditBox>> m_actionEditBoxes;
    };
} // namespace view

#endif // BLOCKS_VIEW_H
