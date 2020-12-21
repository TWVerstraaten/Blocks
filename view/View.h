//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_VIEW_H
#define BLOCKS_VIEW_H

#include "AssetHandler.h"
#include "Grid.h"
#include "ScreenCoordinates.h"
#include "widget/ActionEditBox.h"

#include <iostream>
#include <list>
#include <vector>

namespace model {
    class Model;
    class Cluster;
    class Level;
    class WorldCoordinates;
} // namespace model

class SDL_Window;
class SDL_Renderer;

namespace view {
    class View {
      public:
        View();
        ~View();
        View(const View& other) = delete;

        void                                                       draw(const model::Model& model);
        void                                                       zoom(int amount);
        void                                                       translate(int dx, int dy);
        void                                                       addActionEditBox(const model::Cluster& cluster);
        const std::vector<std::unique_ptr<widget::ActionEditBox>>& actionEditBoxes() const;
        SDL_Renderer*                                              renderer() const;
        void                                                       render(const ScreenCoordinates& point, int width, int height, const SDL_Color& color);

        void render(const model::WorldCoordinates& point, int widthInWorld, int heightInWorld, const SDL_Color& color);

      private:
        SDL_Point windowSize() const;

        void drawClusters(const std::__cxx11::list<model::Cluster>& clusters) const;
        void drawLevel(const model::Level& level) const;
        void drawGridLines() const;

        int                                                 m_zoomParameter = 0;
        SDL_Window*                                         m_window        = nullptr;
        SDL_Renderer*                                       m_renderer      = nullptr;
        Grid                                                m_grid;
        std::unique_ptr<AssetHandler>                       m_assetsHandler{new AssetHandler()};
        std::vector<std::unique_ptr<widget::ActionEditBox>> m_actionEditBoxes;
    };
} // namespace view

#endif // BLOCKS_VIEW_H
