//
// Created by pc on 15-12-20.
//

#include "View.h"

#include "../global/fns.h"
#include "../global/geom.h"
#include "../model/Model.h"
#include "color.h"
#include "widget/ScrollArea.h"

#include <SDL_image.h>
#include <algorithm>

namespace view {

    View::View() {
        std::cout << "View constructor\n";
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
            exit(255);
        }
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            std::cout << "Warning: Linear texture filtering not enabled!\n";
        }

        //        SDL_DisplayMode DM;
        //        SDL_GetCurrentDisplayMode(0, &DM);
        //        auto Width = DM.w;
        //        auto Height = DM.h;
        m_window = SDL_CreateWindow("Blocks",
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    cst::INITIAL_SCREEN_WIDTH,
                                    cst::INITIAL_SCREEN_HEIGHT,
                                    SDL_WINDOW_RESIZABLE);
        if (!m_window) {
            std::cout << "Failed to create window, SDL2 Error: " << SDL_GetError() << "\n";
            return;
        }
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (!m_renderer) {
            std::cout << "Failed to get window's surface, "
                      << "SDL2 Error: " << SDL_GetError() << "\n";
            return;
        }
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
            std::cout << "SDL_image could not initialize, SDL_image Error: " << IMG_GetError() << '\n';
        }

        if (TTF_Init() == -1) {
            std::cout << "SDL_ttf could not initialize, SDL_ttf Error: " << TTF_GetError() << '\n';
        }

        m_assets->init(m_renderer);
        SDL_StartTextInput();
    }

    View::~View() {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        m_assets.reset(nullptr); // Release font before we close subsystems
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    void View::draw(const model::Model& model) {
        setDrawColor(view::color::BACKGROUND);
        SDL_RenderClear(m_renderer);

        drawLevel(model.level());
        drawClusters(model.clusters());
        drawBlocks(model.level());

        //        for (auto& cluster : model.clusters()) {
        //            const auto points = cluster.cornerPoints(cst::BLOCK_SHRINK_IN_WORLD);
        //            for (const auto& it : points) {
        //                drawPoint(it, view::color::RED, 2);
        //            }
        //        }
    }

    void View::drawClusters(const std::list<model::Cluster>& clusters) {
        for (const auto& cluster : clusters) {
            switch (cluster.phase()) {
                case model::Cluster::PHASE::NONE:
                    drawClusterNoPhase(cluster);
                    break;
                case model::Cluster::PHASE::TRANSLATING:
                    drawClusterTranslating(cluster);
                    break;
                case model::Cluster::PHASE::ROTATING:
                    drawClusterRotating(cluster);
                    break;
            }
            setDrawColor(view::color::RED);
            //            for (const auto& side : cluster.sides(cst::BLOCK_SHRINK_IN_WORLD)) {
            //                const auto p1 = ScreenXY::fromWorldXY(side.start(), m_viewPort);
            //                const auto p2 = ScreenXY::fromWorldXY(side.end(), m_viewPort);
            //
            //                SDL_RenderDrawLine(m_renderer, p1.x(), p1.y(), p2.x(), p2.y());
            //            }

            renderClusterName(cluster);
        }
    }

    void View::drawLevel(const model::Level& level) const {
        const auto blockShrinkInScreen = m_viewPort.worldToScreenLength(cst::BLOCK_SHRINK_IN_WORLD);
        const auto shrinkInScreenXY    = ScreenXY{blockShrinkInScreen, blockShrinkInScreen};
        const auto shrunkBlockSize     = m_viewPort.blockSizeInScreen() - 2 * blockShrinkInScreen;
        for (const auto& block : level.levelBlocks()) {
            drawRectangle(ScreenXY::fromGridXY(block, m_viewPort) + shrinkInScreenXY,
                          shrunkBlockSize,
                          shrunkBlockSize,
                          view::color::BACKGROUND_PLAYABLE);
        }
        for (const auto& block : level.startBlocks()) {
            drawRectangle(ScreenXY::fromGridXY(block, m_viewPort) + shrinkInScreenXY,
                          shrunkBlockSize,
                          shrunkBlockSize,
                          view::color::BACKGROUND_START);
        }

        for (const auto& line : level.boundaries()) {
            setDrawColor(view::color::WHITE);
            const auto p1 = ScreenXY::fromWorldXY(line.start(), m_viewPort);
            const auto p2 = ScreenXY::fromWorldXY(line.end(), m_viewPort);

            SDL_RenderDrawLine(m_renderer, p1.x(), p1.y(), p2.x(), p2.y());
        }
    }

    void View::drawBlocks(const model::Level& level) const {
        const auto blockShrinkInScreen = m_viewPort.worldToScreenLength(cst::BLOCK_SHRINK_IN_WORLD);
        const auto shrinkInScreenXY    = ScreenXY{blockShrinkInScreen, blockShrinkInScreen};
        const auto shrunkBlockSize     = m_viewPort.blockSizeInScreen() - 2 * blockShrinkInScreen;
        for (const auto& block : level.dynamicBlocks()) {
            assert(m_assets->renderTexture(Assets::getTextureEnum(block.second),
                                           ScreenXY::fromGridXY(block.first, m_viewPort) + shrinkInScreenXY,
                                           shrunkBlockSize,
                                           shrunkBlockSize,
                                           m_renderer));
        }
        for (const auto& block : level.instantBlocks()) {
            assert(m_assets->renderTexture(Assets::getTextureEnum(block.second),
                                           ScreenXY::fromGridXY(block.first, m_viewPort) + shrinkInScreenXY,
                                           shrunkBlockSize,
                                           shrunkBlockSize,
                                           m_renderer));
        }
    }

    void View::zoom(int amount) {
        m_viewPort.zoom(amount);
    }

    void View::translate(int dx, int dy) {
        m_viewPort.translate(dx, dy);
    }

    ScreenXY View::windowSize() const {
        int windowWidth, windowHeight;
        SDL_GetWindowSize(m_window, &windowWidth, &windowHeight);
        return {windowWidth, windowHeight};
    }

    void View::drawRectangle(const ScreenXY& point, int width, int height, const SDL_Color& color) const {
        setDrawColor(color);
        const SDL_Rect rect{point.x(), point.y(), width, height};
        SDL_RenderFillRect(m_renderer, &rect);
    }

    void View::drawRectangle(const model::WorldXY& point,
                             int                   widthInWorld,
                             int                   heightInWorld,
                             const SDL_Color&      color) const {
        drawRectangle(ScreenXY::fromWorldXY(point, m_viewPort),
                      m_viewPort.worldToScreenLength(widthInWorld),
                      m_viewPort.worldToScreenLength(heightInWorld),
                      color);
    }

    void View::drawPoint(const ScreenXY& point, const SDL_Color& color, int pointSize) const {
        drawRectangle(ScreenXY{point.x() - pointSize / 2, point.y() - pointSize / 2}, pointSize, pointSize, color);
    }

    void View::drawPoint(const model::WorldXY& point, const SDL_Color& color, int pointSize) const {
        drawRectangle(
            model::WorldXY{point.x() - pointSize / 2, point.y() - pointSize / 2}, pointSize, pointSize, color);
    }

    void View::drawHorizontalLine(const ScreenXY&  point,
                                  int              length,
                                  const SDL_Color& color,
                                  size_t           lineThickness) const {
        if (lineThickness == 1) {
            setDrawColor(color);
            SDL_RenderDrawLine(m_renderer, point.x(), point.y(), point.x() + length, point.y());
        } else {
            drawRectangle(
                ScreenXY{point.x(), static_cast<int>(point.y() - lineThickness / 2)}, length, lineThickness, color);
        }
    }

    void View::drawHorizontalLine(const model::WorldXY& point,
                                  int                   lengthInWorld,
                                  const SDL_Color&      color,
                                  size_t                lineThickness) const {
        drawHorizontalLine(ScreenXY::fromWorldXY(point, m_viewPort), lengthInWorld, color, lineThickness);
    }

    void View::drawVerticalLine(const ScreenXY& point, int length, const SDL_Color& color, size_t lineThickness) const {
        if (lineThickness == 1) {
            setDrawColor(color);
            SDL_RenderDrawLine(m_renderer, point.x(), point.y(), point.x(), point.y() + length);
        } else {
            drawRectangle(
                ScreenXY{static_cast<int>(point.x() - lineThickness / 2), point.y()}, lineThickness, length, color);
        }
    }

    void View::drawVerticalLine(const model::WorldXY& point,
                                int                   lengthInWorld,
                                const SDL_Color&      color,
                                Uint32                lineThickness) const {
        drawRectangle(model::WorldXY{point.x(), static_cast<int>(point.y() - lineThickness / 2)},
                      lineThickness,
                      lengthInWorld,
                      color);
    }

    void View::setDrawColor(const SDL_Color& color) const {
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, 255);
    }

    const ViewPort& View::viewPort() const {
        return m_viewPort;
    }

    void View::clear() {
        //        m_scrollArea.children().clear();
    }

    void View::renderPresent() const {
        SDL_RenderPresent(m_renderer);
    }

    SDL_Renderer* View::renderer() const {
        return m_renderer;
    }

    Assets* View::assets() const {
        return m_assets.get();
    }

    void View::renderClusterName(const model::Cluster& cluster) {
        const auto worldPosition  = cluster.approximateCenter();
        const auto screenPosition = ScreenXY::fromWorldXY(worldPosition, m_viewPort);

        if (m_nameTextures.find(cluster.index()) == m_nameTextures.end()) {
            const std::string name = cluster.name() + " " + std::to_string(cluster.index());
            m_nameTextures.insert(
                {cluster.index(),
                 view::Texture::createFromText(
                     name, view::color::BLACK, m_renderer, m_assets->font(Assets::FONT_ENUM::MAIN)->font())});
        }
        const auto& texture = m_nameTextures.at(cluster.index());
        drawRectangle(screenPosition, texture->width(), texture->height(), SDL_Color{105, 115, 133, 255});

        m_assets->renderTexture(texture.get(), screenPosition, texture->width(), texture->height(), m_renderer);
    }

    void View::drawClusterNoPhase(const model::Cluster& cluster) const {
        assert(cluster.phase() == model::Cluster::PHASE::NONE);
        const auto shrunkBlockSize =
            m_viewPort.worldToScreenLength(cst::BLOCK_SIZE_IN_WORLD - 2 * cst::BLOCK_SHRINK_IN_WORLD);
        const auto shrinkWorldXY = model::WorldXY{cst::BLOCK_SHRINK_IN_WORLD, cst::BLOCK_SHRINK_IN_WORLD};
        for (auto it : cluster.gridXY()) {
            assert(m_assets->renderTexture(TextureWrapper::TEXTURE_ENUM::CLUSTER,
                                           ScreenXY::fromWorldXY(model::WorldXY(it) + shrinkWorldXY, m_viewPort),
                                           shrunkBlockSize,
                                           shrunkBlockSize,
                                           m_renderer));
            if (m_viewPort.distanceBetweenBlocksInScreenXY() == 0) {
                continue;
            }
            if (cluster.contains(it.neighbor(model::GridXY::DIRECTION::LEFT))) {
                assert(m_assets->renderTexture(TextureWrapper::TEXTURE_ENUM::CLUSTER,
                                               ScreenXY::fromWorldXY(model::WorldXY(it) + shrinkWorldXY, m_viewPort),
                                               -m_viewPort.distanceBetweenBlocksInScreenXY(),
                                               shrunkBlockSize,
                                               m_renderer));
            }
            if (cluster.contains(it.neighbor(model::GridXY::DIRECTION::UP))) {
                assert(m_assets->renderTexture(TextureWrapper::TEXTURE_ENUM::CLUSTER,
                                               ScreenXY::fromWorldXY(model::WorldXY(it) + shrinkWorldXY, m_viewPort),
                                               shrunkBlockSize,
                                               -m_viewPort.distanceBetweenBlocksInScreenXY(),
                                               m_renderer));
            }
        }
    }

    void View::drawClusterTranslating(const model::Cluster& cluster) const {
        assert(cluster.phase() == model::Cluster::PHASE::TRANSLATING);
        const auto shrunkBlockSize =
            m_viewPort.worldToScreenLength(cst::BLOCK_SIZE_IN_WORLD - 2 * cst::BLOCK_SHRINK_IN_WORLD);
        const auto f             = cluster.phaseTransformation();
        const auto shrinkWorldXY = model::WorldXY{cst::BLOCK_SHRINK_IN_WORLD, cst::BLOCK_SHRINK_IN_WORLD};
        for (auto it : cluster.gridXY()) {
            assert(m_assets->renderTexture(TextureWrapper::TEXTURE_ENUM::CLUSTER,
                                           ScreenXY::fromWorldXY(f(model::WorldXY(it) + shrinkWorldXY), m_viewPort),
                                           shrunkBlockSize,
                                           shrunkBlockSize,
                                           m_renderer));
            if (m_viewPort.distanceBetweenBlocksInScreenXY() == 0) {
                continue;
            }
            if (cluster.contains(it.neighbor(model::GridXY::DIRECTION::LEFT))) {
                assert(m_assets->renderTexture(TextureWrapper::TEXTURE_ENUM::CLUSTER,
                                               ScreenXY::fromWorldXY(f(model::WorldXY(it) + shrinkWorldXY), m_viewPort),
                                               -m_viewPort.distanceBetweenBlocksInScreenXY(),
                                               shrunkBlockSize,
                                               m_renderer));
            }
            if (cluster.contains(it.neighbor(model::GridXY::DIRECTION::UP))) {
                assert(m_assets->renderTexture(TextureWrapper::TEXTURE_ENUM::CLUSTER,
                                               ScreenXY::fromWorldXY(f(model::WorldXY(it) + shrinkWorldXY), m_viewPort),
                                               shrunkBlockSize,
                                               -m_viewPort.distanceBetweenBlocksInScreenXY(),
                                               m_renderer));
            }
        }
    }

    void View::drawClusterRotating(const model::Cluster& cluster) const {
        assert(cluster.phase() == model::Cluster::PHASE::ROTATING);
        const model::WorldXY center = model::WorldXY(cluster.rotationPivot()) + cst::HALF_BLOCK_IN_WORLD;
        const double         theta  = cluster.angle();
        const auto           pivot  = SDL_Point{0, 0};
        const auto           shrunkBlockSize =
            m_viewPort.worldToScreenLength(cst::BLOCK_SIZE_IN_WORLD - 2 * cst::BLOCK_SHRINK_IN_WORLD);
        const auto shrinkWorldXY = model::WorldXY{cst::BLOCK_SHRINK_IN_WORLD, cst::BLOCK_SHRINK_IN_WORLD};
        for (auto it : cluster.gridXY()) {
            const auto topLeftWorldXY  = geom::rotateAboutPivot(model::WorldXY(it) + shrinkWorldXY, center, -theta);
            const auto topLeftScreenXY = ScreenXY::fromWorldXY(topLeftWorldXY, m_viewPort);
            m_assets->renderTexture(TextureWrapper::TEXTURE_ENUM::CLUSTER,
                                    topLeftScreenXY,
                                    shrunkBlockSize,
                                    shrunkBlockSize,
                                    m_renderer,
                                    theta,
                                    &pivot);
            if (m_viewPort.distanceBetweenBlocksInScreenXY() == 0) {
                continue;
            }
            if (cluster.contains(it.neighbor(model::GridXY::DIRECTION::LEFT))) {
                m_assets->renderTexture(
                    TextureWrapper::TEXTURE_ENUM::CLUSTER,
                    ScreenXY::fromWorldXY(
                        geom::rotateAboutPivot(model::WorldXY(it) + model::WorldXY{-cst::BLOCK_SHRINK_IN_WORLD,
                                                                                   cst::BLOCK_SHRINK_IN_WORLD},
                                               center,
                                               -theta),
                        m_viewPort),
                    m_viewPort.distanceBetweenBlocksInScreenXY(),
                    shrunkBlockSize,
                    m_renderer,
                    theta,
                    &pivot);
            }
            if (cluster.contains(it.neighbor(model::GridXY::DIRECTION::UP))) {
                m_assets->renderTexture(
                    TextureWrapper::TEXTURE_ENUM::CLUSTER,
                    ScreenXY::fromWorldXY(
                        geom::rotateAboutPivot(model::WorldXY(it) + model::WorldXY{cst::BLOCK_SHRINK_IN_WORLD,
                                                                                   -cst::BLOCK_SHRINK_IN_WORLD},
                                               center,
                                               -theta),
                        m_viewPort),
                    shrunkBlockSize,
                    m_viewPort.distanceBetweenBlocksInScreenXY(),
                    m_renderer,
                    theta,
                    &pivot);
            }
        }
    }

    void View::drawScrollArea(widget::ScrollArea* scrollArea) {
        scrollArea->render(m_renderer);
    }

} // namespace view
