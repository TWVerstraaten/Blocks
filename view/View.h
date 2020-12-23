//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_VIEW_H
#define BLOCKS_VIEW_H

#include "Assets.h"
#include "ViewPort.h"
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
        void                                renderPresent() const;
        const ViewPort&                     viewPort() const;
        std::vector<widget::ActionEditBox>& actionEditBoxes();
        SDL_Renderer*                       renderer() const;
        Assets&                             assets() const;
        void                                drawRectangle(const ScreenXY& point, int width, int height, const SDL_Color& color) const;
        void      drawRectangle(const model::WorldXY& point, int widthInWorld, int heightInWorld, const SDL_Color& color) const;
        void      drawPoint(const ScreenXY& point, const SDL_Color& color, int pointSize) const;
        void      drawPoint(const model::WorldXY& point, const SDL_Color& color, int pointSize) const;
        void      drawHorizontalLine(const ScreenXY& point, int length, const SDL_Color& color, size_t lineThickness) const;
        void      drawHorizontalLine(const model::WorldXY& point, int lengthInWorld, const SDL_Color& color, size_t lineThickness) const;
        void      drawVerticalLine(const ScreenXY& point, int length, const SDL_Color& color, size_t lineThickness) const;
        void      drawVerticalLine(const model::WorldXY& point, int lengthInWorld, const SDL_Color& color, Uint32 lineThickness) const;
        void      clear();
        void      setActionEditBoxes(const std::vector<model::Cluster>& clusters);
        SDL_Point windowSize() const;

      private:
        void drawClusters(const std::vector<model::Cluster>& clusters) const;
        void drawLevelBlocks(const model::Level& level) const;
        void drawBlocks(const model::Level& level) const;
        void drawActionEditBoxes();
        void setDrawColor(const SDL_Color& color) const;

        int                                m_zoomParameter = 0;
        SDL_Window*                        m_window        = nullptr;
        SDL_Renderer*                      m_renderer      = nullptr;
        ViewPort                           m_viewPort;
        std::unique_ptr<Assets>            m_assets{new Assets()};
        std::vector<widget::ActionEditBox> m_actionEditBoxes;
    };
} // namespace view

#endif // BLOCKS_VIEW_H
