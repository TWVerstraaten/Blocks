//
// Created by pc on 15-12-20.
//

#include "View.h"

#include "../global/geom.h"
#include "../model/Model.h"
#include "View_constants.h"
#include "toColor.h"
#include "widget/BlockSelectWidget.h"
#include "widget/ScrollArea.h"

#include <SDL_image.h>
#include <cassert>

namespace view {
    using namespace model;

    View::View() {
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
        m_window = SDL_CreateWindow(
            "Blocks", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
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
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);

        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
            std::cout << "SDL_image could not initialize, SDL_image Error: " << IMG_GetError() << '\n';
        }

        if (TTF_Init() == -1) {
            std::cout << "SDL_ttf could not initialize, SDL_ttf Error: " << TTF_GetError() << '\n';
        }

        Assets::init(m_renderer);
        SDL_StartTextInput();
    }

    View::~View() {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        Assets::release();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    void View::draw(const Model& model) {
        setDrawColor(view::color::BACKGROUND);
        SDL_RenderClear(m_renderer);

        drawLevel(model.level());
        drawClusters(model.clusters());
        drawBlocks(model.level());

#ifdef DEBUG
        for (auto& cluster : model.clusters()) {
            const auto points = cluster.cornerPoints(app::BLOCK_SHRINK_IN_WORLD);
            for (const auto& it : points) {
                drawPoint(it, view::color::RED, 8);
            }
        }
#endif
    }

    void View::drawClusters(const std::list<Cluster>& clusters) {
        for (const auto& cluster : clusters) {
            drawConnected(cluster.gridXY(), cluster.phaseTransformation(), cluster.angle(), cluster.isAlive() ? color::CLUSTER : color::CLUSTER_DEAD);
        }
        for (const auto& cluster : clusters) {
            renderClusterName(cluster);
        }
    }

    void View::drawLevel(const Level& level) const {
        const auto blockShrinkInScreen = m_viewPort.worldToScreenLength(app::BLOCK_SHRINK_IN_WORLD);
        const auto shrinkInScreenXY    = ScreenXY{blockShrinkInScreen, blockShrinkInScreen};
        const auto shrunkBlockSize     = m_viewPort.blockSizeInScreen() - 2 * blockShrinkInScreen;

        for (const auto& [block, type] : level.floorBlocks()) {
            const auto position = ScreenXY::fromGridXY(block, m_viewPort) + shrinkInScreenXY;
            drawSquare(position, shrunkBlockSize, toColor(type));
        }

        for (auto stoppedCluster : level.stoppedClusters()) {
            drawConnected(stoppedCluster.gridXY(), view::color::DARK_GREY);
        }
#ifdef DEBUG
        for (const auto& line : level.boundaries()) {
            setDrawColor(view::color::WHITE);
            const auto p1 = ScreenXY::fromWorldXY(line.start(), m_viewPort);
            const auto p2 = ScreenXY::fromWorldXY(line.end(), m_viewPort);
            SDL_RenderDrawLine(m_renderer, p1.x(), p1.y(), p2.x(), p2.y());
        }
#endif
    }

    void View::drawBlocks(const Level& level) const {
        draw(level.dynamicBlocks());
        draw(level.instantBlocks());
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

    void View::drawSquare(const ScreenXY& point, int length, const SDL_Color& color) const {
        drawRectangle(point, length, length, color);
    }

    void View::drawRectangle(const WorldXY& point, int widthInWorld, int heightInWorld, const SDL_Color& color) const {
        drawRectangle(ScreenXY::fromWorldXY(point, m_viewPort),
                      m_viewPort.worldToScreenLength(widthInWorld),
                      m_viewPort.worldToScreenLength(heightInWorld),
                      color);
    }

    void View::drawPoint(const ScreenXY& point, const SDL_Color& color, int pointSize) const {
        drawRectangle(ScreenXY{point.x() - pointSize / 2, point.y() - pointSize / 2}, pointSize, pointSize, color);
    }

    void View::drawPoint(const WorldXY& point, const SDL_Color& color, int pointSize) const {
        drawRectangle(WorldXY{point.x() - pointSize / 2, point.y() - pointSize / 2}, pointSize, pointSize, color);
    }

    void View::drawHorizontalLine(const ScreenXY& point, int length, const SDL_Color& color, size_t lineThickness) const {
        if (lineThickness == 1) {
            setDrawColor(color);
            SDL_RenderDrawLine(m_renderer, point.x(), point.y(), point.x() + length, point.y());
        } else {
            drawRectangle(ScreenXY{point.x(), static_cast<int>(point.y() - lineThickness / 2)}, length, lineThickness, color);
        }
    }

    void View::drawHorizontalLine(const WorldXY& point, int lengthInWorld, const SDL_Color& color, size_t lineThickness) const {
        drawHorizontalLine(ScreenXY::fromWorldXY(point, m_viewPort), lengthInWorld, color, lineThickness);
    }

    void View::drawVerticalLine(const ScreenXY& point, int length, const SDL_Color& color, size_t lineThickness) const {
        if (lineThickness == 1) {
            setDrawColor(color);
            SDL_RenderDrawLine(m_renderer, point.x(), point.y(), point.x(), point.y() + length);
        } else {
            drawRectangle(ScreenXY{static_cast<int>(point.x() - lineThickness / 2), point.y()}, lineThickness, length, color);
        }
    }

    void View::drawVerticalLine(const WorldXY& point, int lengthInWorld, const SDL_Color& color, Uint32 lineThickness) const {
        drawRectangle(WorldXY{point.x(), static_cast<int>(point.y() - lineThickness / 2)}, lineThickness, lengthInWorld, color);
    }

    void View::setDrawColor(const SDL_Color& color) const {
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, 255);
    }

    const ViewPort& View::viewPort() const {
        return m_viewPort;
    }

    void View::renderPresent() const {
        SDL_RenderPresent(m_renderer);
    }

    SDL_Renderer* View::renderer() const {
        return m_renderer;
    }

    void View::renderClusterName(const Cluster& cluster) {
        const auto worldPosition  = cluster.approximateCenter();
        const auto screenPosition = ScreenXY::fromWorldXY(worldPosition, m_viewPort);

        if (m_nameTextures.find(cluster.index()) == m_nameTextures.end()) {
            const std::string name = cluster.name() + " " + std::to_string(cluster.index());
            m_nameTextures.insert(
                {cluster.index(), view::Texture::createFromText(name, view::color::BLACK, m_renderer, Assets::font(FONT_ENUM::MAIN)->font())});
        }
        const auto& texture = m_nameTextures.at(cluster.index());
        drawRectangle(screenPosition, texture->width(), texture->height(), SDL_Color{105, 115, 133, 255});

        Assets::renderTexture(texture.get(), screenPosition, texture->width(), texture->height(), m_renderer);
    }

    void View::drawScrollArea(widget::ScrollArea* scrollArea) {
        scrollArea->render(m_renderer);
    }

    void View::drawBlockSelectWidget(widget::BlockSelectWidget& widget) {
        widget.render(m_renderer);
    }

    int View::windowHeight() const {
        return windowSize().y();
    }

    int View::windowWidth() const {
        return windowSize().x();
    }

    void View::drawRectangle(const ScreenXY& point, int width, int height, const SDL_Color& color, SDL_Renderer* renderer) {
        setDrawColor(color, renderer);
        const SDL_Rect rect{point.x(), point.y(), width, height};
        SDL_RenderFillRect(renderer, &rect);
    }

    void View::drawSquare(const ScreenXY& point, int length, const SDL_Color& color, SDL_Renderer* renderer) {
        drawRectangle(point, length, length, color, renderer);
    }

    void View::setDrawColor(const SDL_Color& color, SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    }

    void View::drawSquareOutline(const ScreenXY& point, int length, int lineThickness, const SDL_Color& color, SDL_Renderer* renderer) {
        drawRectangle(point - ScreenXY{lineThickness, lineThickness}, length + 2 * lineThickness, lineThickness, color, renderer);
        drawRectangle(point + ScreenXY{-lineThickness, length}, length + 2 * lineThickness, lineThickness, color, renderer);
        drawRectangle(point - ScreenXY{lineThickness, lineThickness}, lineThickness, length + 2 * lineThickness, color, renderer);
        drawRectangle(point + ScreenXY{length, -lineThickness}, lineThickness, length + 2 * lineThickness, color, renderer);
    }

    void View::drawDisconnected(const GridXYSet& blocks, const SDL_Color& color, TEXTURE_ENUM textureEnum) const {
        const auto blockShrinkInScreen = m_viewPort.worldToScreenLength(app::BLOCK_SHRINK_IN_WORLD);
        const auto shrinkInScreenXY    = ScreenXY{blockShrinkInScreen, blockShrinkInScreen};
        const auto shrunkBlockSize     = m_viewPort.blockSizeInScreen() - 2 * blockShrinkInScreen;

        auto texture = Assets::getTexture(textureEnum, shrunkBlockSize, shrunkBlockSize);
        texture->setColor(color);
        for (const auto& block : blocks) {
            const auto position = ScreenXY::fromGridXY(block, m_viewPort) + shrinkInScreenXY;
            texture->render({position.x(), position.y(), shrunkBlockSize, shrunkBlockSize}, m_renderer);
        }
    }

    void View::drawConnected(const GridXYSet&                              blocks,
                             const std::function<WorldXY(const WorldXY&)>& fun,
                             double                                        angle,
                             const SDL_Color&                              color,
                             TEXTURE_ENUM                                  textureEnum) const {
        static const SDL_Point origin{0, 0};
        const auto             texture = getTextureInWorld(blocks, textureEnum, color);
        const auto             topLeft = ScreenXY::fromWorldXY(fun(WorldXY(GridXY{geom::minX(blocks), geom::minY(blocks)})), m_viewPort);
        texture->render(
            {topLeft.x(), topLeft.y(), m_viewPort.worldToScreenLength(texture->width()), m_viewPort.worldToScreenLength(texture->height())},
            m_renderer,
            angle,
            &origin);
    }

    void View::drawConnected(const GridXYSet& blocks, const SDL_Color& color, TEXTURE_ENUM textureEnum) const {
        if (blocks.empty()) {
            return;
        }
        const auto texture = getTextureInWorld(blocks, textureEnum, color);
        const auto topLeft = ScreenXY::fromWorldXY((WorldXY(GridXY{geom::minX(blocks), geom::minY(blocks)})), m_viewPort);
        texture->render(
            {topLeft.x(), topLeft.y(), m_viewPort.worldToScreenLength(texture->width()), m_viewPort.worldToScreenLength(texture->height())},
            m_renderer);
    }

    std::unique_ptr<Texture> View::getTextureInWorld(const GridXYSet& blocks, const TEXTURE_ENUM textureEnum, const SDL_Color& color) const {
        const auto shrunkSize  = app::BLOCK_SIZE_IN_WORLD - 2 * app::BLOCK_SHRINK_IN_WORLD;
        const auto minX        = geom::minX(blocks);
        const auto minY        = geom::minY(blocks);
        const auto maxX        = geom::maxX(blocks);
        const auto maxY        = geom::maxY(blocks);
        const auto width       = (maxX - minX + 1) * app::BLOCK_SIZE_IN_WORLD;
        const auto height      = (maxY - minY + 1) * app::BLOCK_SIZE_IN_WORLD;
        const auto twiceShrink = 2 * app::BLOCK_SHRINK_IN_WORLD;

        assert(width > 0);
        assert(height > 0);

        SDL_Texture* clusterTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
        SDL_SetRenderTarget(m_renderer, clusterTexture);
        SDL_SetTextureBlendMode(clusterTexture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
        SDL_RenderClear(m_renderer);

        auto texture = Assets::getTexture(textureEnum, shrunkSize, shrunkSize);
        texture->setColor(color);

        for (const auto block : blocks) {
            const auto position = block - GridXY{minX, minY} + WorldXY{app::BLOCK_SHRINK_IN_WORLD, app::BLOCK_SHRINK_IN_WORLD};
            texture->render({position.x(), position.y(), shrunkSize, shrunkSize}, m_renderer);
            const bool l = blocks.find(block.neighbor(GridXY::DIRECTION::LEFT)) != blocks.end();
            const bool u = blocks.find(block.neighbor(GridXY::DIRECTION::UP)) != blocks.end();
            if (u && l) {
                const bool ul = blocks.find(block.neighbor(GridXY::DIRECTION::UP).neighbor(GridXY::DIRECTION::LEFT)) != blocks.end();
                if (ul) {
                    texture->render({position.x() - twiceShrink - 1, position.y() - twiceShrink - 1, twiceShrink + 1, twiceShrink + 1}, m_renderer);
                }
            }
            if (u) {
                texture->render({position.x(), position.y() - twiceShrink - 1, shrunkSize, twiceShrink + 1}, m_renderer);
            }
            if (l) {
                texture->render({position.x() - twiceShrink - 1, position.y(), twiceShrink + 1, shrunkSize}, m_renderer);
            }
        }
        SDL_SetRenderTarget(m_renderer, nullptr);
        return std::make_unique<Texture>(clusterTexture, width, height);
    }

} // namespace view
