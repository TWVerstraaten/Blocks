//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_VIEW_H
#define BLOCKS_VIEW_H

#include "Assets.h"
#include "Grid.h"
#include "widget/ActionEditBox.h"

#include <iostream>
#include <list>
#include <vector>

namespace model {
    class Model;
    class Cluster;
    class Level;
    class WorldXY;
} // namespace model

class SDL_Window;
class SDL_Renderer;

namespace view {
    class ScreenXY;

    class View {
      public:
        View();
        ~View();
        View(const View& other) = delete;

        void                                draw(const model::Model& model);
        void                                zoom(int amount);
        void                                translate(int dx, int dy);
        void                                addActionEditBox(const model::Cluster& cluster);
        std::vector<widget::ActionEditBox>& actionEditBoxes();
        SDL_Renderer*                       renderer() const;
        void drawRectangle(const ScreenXY& point, int width, int height, const SDL_Color& color) const;
        void drawRectangle(const model::WorldXY& point, int widthInWorld, int heightInWorld, const SDL_Color& color) const;
        void drawPoint(const ScreenXY& point, const SDL_Color& color, int pointSize) const;
        void drawPoint(const model::WorldXY& point, const SDL_Color& color, int pointSize) const;
        void drawHorizontalLine(const ScreenXY& point, int length, const SDL_Color& color, size_t lineThickness) const;
        void drawHorizontalLine(const model::WorldXY& point,
                                int                            lengthInWorld,
                                const SDL_Color&               color,
                                size_t                         lineThickness) const;
        void drawVerticalLine(const ScreenXY& point, int length, const SDL_Color& color, size_t lineThickness) const;
        void drawVerticalLine(const model::WorldXY& point, int lengthInWorld, const SDL_Color& color, Uint16 lineThickness) const;
        const Grid& grid() const;
        void        clear();

      private:
        SDL_Point windowSize() const;
        void      drawClusters(const std::vector<model::Cluster>& clusters) const;
        void      drawLevelBlocks(const model::Level& level) const;
        void      drawBlocks(const model::Level& level) const;
        void      drawGridLines() const;
        void      setDrawColor(const SDL_Color& color) const;

        int                                m_zoomParameter = 0;
        SDL_Window*                        m_window        = nullptr;
        SDL_Renderer*                      m_renderer      = nullptr;
        Grid                               m_grid;
        std::unique_ptr<Assets>      m_assetsHandler{new Assets()};
        std::vector<widget::ActionEditBox> m_actionEditBoxes;
    };
} // namespace view

#endif // BLOCKS_VIEW_H
