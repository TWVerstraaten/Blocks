//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_VIEW_H
#define BLOCKS_VIEW_H

#include "Assets.h"
#include "ViewPort.h"
#include "widget/CommandEditBox.h"

#include <iostream>
#include <list>

namespace model {
    class Model;
    class Cluster;
    class Level;
    class WorldXY;
} // namespace model

namespace view {
    namespace widget {
        class ScrollArea;
    }

    class ScreenXY;

    class View {
      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        View();
        View(const View& other) = delete;
        ~View();

        /****** CONST GETTERS  ******/
        ScreenXY        windowSize() const;
        const ViewPort& viewPort() const;
        SDL_Renderer*   renderer() const;
        Assets*         assets() const;

        /****** CONST FUNCTIONS  ******/
        void renderPresent() const;
        void drawRectangle(const ScreenXY& point, int width, int height, const SDL_Color& color) const;
        void drawRectangle(const model::WorldXY& point,
                           int                   widthInWorld,
                           int                   heightInWorld,
                           const SDL_Color&      color) const;
        void drawPoint(const ScreenXY& point, const SDL_Color& color, int pointSize) const;
        void drawPoint(const model::WorldXY& point, const SDL_Color& color, int pointSize) const;
        void drawHorizontalLine(const ScreenXY& point, int length, const SDL_Color& color, size_t lineThickness) const;
        void drawHorizontalLine(const model::WorldXY& point,
                                int                   lengthInWorld,
                                const SDL_Color&      color,
                                size_t                lineThickness) const;
        void drawVerticalLine(const ScreenXY& point, int length, const SDL_Color& color, size_t lineThickness) const;
        void drawVerticalLine(const model::WorldXY& point,
                              int                   lengthInWorld,
                              const SDL_Color&      color,
                              Uint32                lineThickness) const;

        /****** NON CONST FUNCTIONS  ******/
        void draw(const model::Model& model);
        void draw(widget::ScrollArea* scrollArea);
        void zoom(int amount);
        void translate(int dx, int dy);
        void clear();

      private:
        /****** PRIVATE CONST FUNCTIONS  ******/
        void drawLevel(const model::Level& level) const;
        void drawBlocks(const model::Level& level) const;
        void setDrawColor(const SDL_Color& color) const;
        void drawClusterNoPhase(const model::Cluster& cluster) const;
        void drawClusterTranslating(const model::Cluster& cluster) const;
        void drawClusterRotating(const model::Cluster& cluster) const;

        /****** PRIVATE NON CONST FUNCTIONS  ******/
        void drawClusters(const std::list<model::Cluster>& clusters);
        void renderClusterName(const model::Cluster& cluster);

        /****** DATA MEMBERS  ******/
        SDL_Window*                                m_window   = nullptr;
        SDL_Renderer*                              m_renderer = nullptr;
        ViewPort                                   m_viewPort;
        std::unique_ptr<Assets>                    m_assets{new Assets()};
        std::map<size_t, std::unique_ptr<Texture>> m_nameTextures;
    };
} // namespace view

#endif // BLOCKS_VIEW_H
