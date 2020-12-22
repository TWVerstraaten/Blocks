//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_VIEW_H
#define BLOCKS_VIEW_H

#include "AssetHandler.h"
#include "Grid.h"
#include "ScreenCoordinates.h"
#include "ScreenVector.h"
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
        void drawRectangle(const ScreenCoordinates& point, int width, int height, const SDL_Color& color) const;
        void drawRectangle(const model::WorldCoordinates& point, int widthInWorld, int heightInWorld, const SDL_Color& color) const;
        void drawPoint(const ScreenCoordinates& point, const SDL_Color& color, int pointSize) const;
        void drawPoint(const model::WorldCoordinates& point, const SDL_Color& color, int pointSize) const;
        void drawHorizontalLine(const ScreenCoordinates& point, int length, const SDL_Color& color, size_t lineThickness) const;
        void drawHorizontalLine(int lengthInWorld, const SDL_Color& color, const model::WorldCoordinates& point,
                                size_t lineThickness) const;
        void drawVerticalLine(const ScreenCoordinates& point, int length, const SDL_Color& color, size_t lineThickness) const;
        void drawVerticalLine(const model::WorldCoordinates& point, int lengthInWorld, const SDL_Color& color, size_t lineThickness) const;
        const Grid& grid() const;

        void clear();

      private:
        SDL_Point windowSize() const;
        void      drawClusters(const std::__cxx11::list<model::Cluster>& clusters) const;
        void      drawLevelBlocks(const model::Level& level) const;
        void      drawBlocks(const model::Level& level) const;
        void      drawGridLines() const;
        void      setDrawColor(const SDL_Color& color) const;

        int                                                 m_zoomParameter = 0;
        SDL_Window*                                         m_window        = nullptr;
        SDL_Renderer*                                       m_renderer      = nullptr;
        Grid                                                m_grid;
        std::unique_ptr<AssetHandler>                       m_assetsHandler{new AssetHandler()};
        std::vector<std::unique_ptr<widget::ActionEditBox>> m_actionEditBoxes;
    };
} // namespace view

#endif // BLOCKS_VIEW_H
