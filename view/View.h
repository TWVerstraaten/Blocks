//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_VIEW_H
#define BLOCKS_VIEW_H

#include "../model/Model_typedefs.h"
#include "Assets.h"
#include "ViewPort.h"
#include "widget/CommandEditBox.h"

#include <functional>
#include <iostream>
#include <list>
#include <set>

namespace model {
    class Model;
    class Cluster;
    class Level;
    class WorldXY;
} // namespace m_model

namespace view {
    namespace widget {
        class ScrollArea;
        class BlockSelectWidget;
    } // namespace widget

    class ScreenXY;

    class View {
      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        View();
        View(const View& other) = delete;
        ~View();

        /****** PUBLIC STATIC FUNCTIONS  ******/
        static void drawRectangle(const ScreenXY& point, int width, int height, const SDL_Color& color, SDL_Renderer* renderer);
        static void drawSquare(const ScreenXY& point, int length, const SDL_Color& color, SDL_Renderer* renderer);
        static void drawSquareOutline(const ScreenXY& point, int length, int lineThickness, const SDL_Color& color, SDL_Renderer* renderer);

        /****** CONST GETTERS  ******/
        [[nodiscard]] ScreenXY        windowSize() const;
        [[nodiscard]] int             windowHeight() const;
        [[nodiscard]] int             windowWidth() const;
        [[nodiscard]] const ViewPort& viewPort() const;
        [[nodiscard]] SDL_Renderer*   renderer() const;

        /****** CONST FUNCTIONS  ******/
        void renderPresent() const;
        void drawRectangle(const ScreenXY& point, int width, int height, const SDL_Color& color) const;
        void drawSquare(const ScreenXY& point, int length, const SDL_Color& color) const;
        void drawRectangle(const model::WorldXY& point, int widthInWorld, int heightInWorld, const SDL_Color& color) const;
        void drawPoint(const ScreenXY& point, const SDL_Color& color, int pointSize) const;
        void drawPoint(const model::WorldXY& point, const SDL_Color& color, int pointSize) const;
        void drawHorizontalLine(const ScreenXY& point, int length, const SDL_Color& color, size_t lineThickness) const;
        void drawHorizontalLine(const model::WorldXY& point, int lengthInWorld, const SDL_Color& color, size_t lineThickness) const;
        void drawVerticalLine(const ScreenXY& point, int length, const SDL_Color& color, size_t lineThickness) const;
        void drawVerticalLine(const model::WorldXY& point, int lengthInWorld, const SDL_Color& color, Uint32 lineThickness) const;

        /****** NON CONST FUNCTIONS  ******/
        void draw(const model::Model& model);
        void drawScrollArea(widget::ScrollArea* scrollArea);
        void drawBlockSelectWidget(widget::BlockSelectWidget& widget);
        void zoom(int amount);
        void translate(int dx, int dy);

      private:
        /****** PRIVATE STATIC FUNCTIONS  ******/
        static void setDrawColor(const SDL_Color& color, SDL_Renderer* renderer);

        /****** PRIVATE CONST FUNCTIONS  ******/
        void drawLevel(const model::Level& level) const;
        void drawBlocks(const model::Level& level) const;
        void setDrawColor(const SDL_Color& color) const;
        void drawDisconnected(const model::GridXYSet& blocks, const SDL_Color& color, TEXTURE_ENUM textureEnum = TEXTURE_ENUM::WHITE) const;
        void drawConnected(const model::GridXYSet& blocks, const SDL_Color& color, TEXTURE_ENUM textureEnum = TEXTURE_ENUM::WHITE) const;
        void drawConnected(const model::GridXYSet&                                     blocks,
                           const std::function<model::WorldXY(const model::WorldXY&)>& fun,
                           double                                                      angle,
                           const SDL_Color&                                            color,
                           TEXTURE_ENUM                                                textureEnum = TEXTURE_ENUM::WHITE) const;
        template <typename ENUM>
        void                                   draw(const std::map<model::GridXY, ENUM>& blocks) const;
        [[nodiscard]] std::unique_ptr<Texture> getTextureInWorld(const model::GridXYSet& blocks,
                                                                 TEXTURE_ENUM            textureEnum,
                                                                 const SDL_Color&        color) const;

        /****** PRIVATE NON CONST FUNCTIONS  ******/
        void drawClusters(const std::list<model::Cluster>& clusters);
        void renderClusterName(const model::Cluster& cluster);

        /****** DATA MEMBERS  ******/
        SDL_Window*                                m_window   = nullptr;
        SDL_Renderer*                              m_renderer = nullptr;
        ViewPort                                   m_viewPort;
        std::map<size_t, std::unique_ptr<Texture>> m_nameTextures;
    };

    template <typename ENUM>
    void View::draw(const std::map<model::GridXY, ENUM>& blocks) const {
        const auto blockShrinkInScreen = m_viewPort.worldToScreen(app::BLOCK_SHRINK_IN_WORLD);
        const auto shrinkInScreenXY    = ScreenXY{blockShrinkInScreen, blockShrinkInScreen};
        const auto shrunkBlockSize     = m_viewPort.blockSizeInScreen() - 2 * blockShrinkInScreen;
        for (const auto& [point, type] : blocks) {
            Assets::renderTexture(Assets::getTextureEnum(type),
                                  ScreenXY::fromGridXY(point, m_viewPort) + shrinkInScreenXY,
                                  shrunkBlockSize,
                                  shrunkBlockSize,
                                  m_renderer);
        }
    }

} // namespace view

#endif // BLOCKS_VIEW_H
