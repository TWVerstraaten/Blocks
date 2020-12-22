//
// Created by pc on 15-12-20.
//

#include "View.h"

#include "../model/Model.h"
#include "../model/WorldCoordinates.h"
#include "Color.h"
#include "Rectangle.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cassert>

namespace view {

    View::View() {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
            exit(255);
        }
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            std::cout << "Warning: Linear texture filtering not enabled!\n";
        }

        size_t initialWidth  = 900;
        size_t initialHeight = 700;

        m_window = SDL_CreateWindow(
            "SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, initialWidth, initialHeight, SDL_WINDOW_RESIZABLE);
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

        m_assetsHandler->init(m_renderer);
        SDL_StartTextInput();
    }

    View::~View() {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        m_assetsHandler.reset(nullptr); // Release font before we close subsystems
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    void View::draw(const model::Model& model) {
        setDrawColor(color::BACKGROUND);
        SDL_RenderClear(m_renderer);

        drawLevelBlocks(model.level());
        drawGridLines();
        drawClusters(model.clusters());
        drawBlocks(model.level());

        for (const auto& cluster : model.clusters()) {
            const auto points = cluster.cornerPoints(0);
            for (const auto& it : points) {
                //                        drawRectangle(model::GridCoordinates::fromWorldCoordinates(it),
                //                        model::WorldCoordinates::m_blockSizeInWorld,
                //                                      model::WorldCoordinates::m_blockSizeInWorld, color::BLUE);
                drawPoint(it, color::RED, 24);
            }
        }

        for (auto& actionEditBox : m_actionEditBoxes) {
            actionEditBox.render(m_renderer);
        }
    }

    void View::drawClusters(const std::vector<model::Cluster>& clusters) const {
        for (const auto& cluster : clusters) {
            const ScreenVector offset = ScreenVector::fromWorldVector(cluster.dynamicWorldOffset(), m_grid);
            for (auto it = cluster.gridCoordinates().begin(); it != cluster.gridCoordinates().end(); ++it) {
                const SDL_Point center = {static_cast<int>(m_grid.blockSizeInScreen() * (0.5 + cluster.rotationPivot().x() - it->x())),
                                          static_cast<int>(m_grid.blockSizeInScreen() * (0.5 + cluster.rotationPivot().y() - it->y()))};
                assert(m_assetsHandler->renderTexture(AssetHandler::TEXTURE_ENUM::CLUSTER,
                                                      ScreenCoordinates::fromGridCoordinates(*it, m_grid) + offset,
                                                      m_grid.blockSizeInScreen(),
                                                      m_grid.blockSizeInScreen(),
                                                      m_renderer,
                                                      cluster.angle(),
                                                      &center));
            }
        }
    }

    void View::drawGridLines() const {
        const auto size = windowSize();
        setDrawColor(color::GRID_LINE_COLOR);

        int x = m_grid.firstColumnInView();
        int y = m_grid.firstRowInView();
        while (true) {
            const auto currentCoordinate = ScreenCoordinates::fromGridCoordinates({x, y - 1}, m_grid);
            if (currentCoordinate.x() > size.x) {
                break;
            }
            drawVerticalLine(currentCoordinate, size.y + 2 * m_grid.blockSizeInScreen(), color::GRID_LINE_COLOR, 1);
            ++x;
        }
        x = m_grid.firstColumnInView();
        while (true) {
            const auto currentCoordinate = ScreenCoordinates::fromGridCoordinates({x - 1, y}, m_grid);
            if (currentCoordinate.y() > size.y) {
                break;
            }
            drawHorizontalLine(currentCoordinate, size.x + 2 * m_grid.blockSizeInScreen(), color::GRID_LINE_COLOR, 1);
            ++y;
        }
    }

    void View::drawLevelBlocks(const model::Level& level) const {
        for (const auto& block : level.levelBlocks()) {
            drawRectangle(block, model::WorldCoordinates::m_blockSizeInWorld, model::WorldCoordinates::m_blockSizeInWorld, color::WHITE);
        }
    }

    void View::drawBlocks(const model::Level& level) const {
        for (const auto& block : level.dynamicBlocks()) {
            assert(m_assetsHandler->renderTexture(AssetHandler::getTextureEnum(block.second),
                                                  ScreenCoordinates::fromGridCoordinates(block.first, m_grid),
                                                  m_grid.blockSizeInScreen(),
                                                  m_grid.blockSizeInScreen(),
                                                  m_renderer,
                                                  0));
        }

        for (const auto& block : level.instantBlocks()) {
            assert(m_assetsHandler->renderTexture(AssetHandler::getTextureEnum(block.second),
                                                  ScreenCoordinates::fromGridCoordinates(block.first, m_grid),
                                                  m_grid.blockSizeInScreen(),
                                                  m_grid.blockSizeInScreen(),
                                                  m_renderer,
                                                  0));
        }
    }

    void View::zoom(int amount) {
        m_zoomParameter += amount;
        m_zoomParameter = std::max(m_zoomParameter, -28);
        m_zoomParameter = std::min(m_zoomParameter, 6);
        m_grid.setScale(m_zoomParameter);
    }

    void View::translate(int dx, int dy) {
        m_grid.translate(dx, dy);
    }

    SDL_Point View::windowSize() const {
        int windowWidth, windowHeight;
        SDL_GetWindowSize(m_window, &windowWidth, &windowHeight);
        return {windowWidth, windowHeight};
    }

    void View::addActionEditBox(const model::Cluster& cluster) {
        m_actionEditBoxes.emplace_back(widget::ActionEditBox(0, m_actionEditBoxes.size() * 200, 200, 200, m_assetsHandler.get(), cluster));
    }

    const std::vector<widget::ActionEditBox>& View::actionEditBoxes() const {
        return m_actionEditBoxes;
    }

    std::vector<widget::ActionEditBox>& View::actionEditBoxes() {
        return m_actionEditBoxes;
    }

    SDL_Renderer* View::renderer() const {
        return m_renderer;
    }

    void View::drawRectangle(const ScreenCoordinates& point, int width, int height, const SDL_Color& color) const {
        setDrawColor(color);
        const SDL_Rect rect{point.x(), point.y(), width, height};
        SDL_RenderFillRect(m_renderer, &rect);
    }

    void View::drawRectangle(const model::WorldCoordinates& point, int widthInWorld, int heightInWorld, const SDL_Color& color) const {
        drawRectangle(ScreenCoordinates::fromWorldCoordinates(point, m_grid),
                      m_grid.worldToScreenLength(widthInWorld),
                      m_grid.worldToScreenLength(heightInWorld),
                      color);
    }

    void View::drawPoint(const ScreenCoordinates& point, const SDL_Color& color, int pointSize) const {
        drawRectangle(ScreenCoordinates{point.x() - pointSize / 2, point.y() - pointSize / 2}, pointSize, pointSize, color);
    }

    void View::drawPoint(const model::WorldCoordinates& point, const SDL_Color& color, int pointSize) const {
        drawRectangle(model::WorldCoordinates{point.x() - pointSize / 2, point.y() - pointSize / 2}, pointSize, pointSize, color);
    }

    void View::drawHorizontalLine(const ScreenCoordinates& point, int length, const SDL_Color& color, size_t lineThickness) const {
        drawRectangle(ScreenCoordinates{point.x(), static_cast<int>(point.y() - lineThickness / 2)}, length, lineThickness, color);
    }

    void View::drawHorizontalLine(int                            lengthInWorld,
                                  const SDL_Color&               color,
                                  const model::WorldCoordinates& point,
                                  size_t                         lineThickness) const {
        drawRectangle(
            model::WorldCoordinates{point.x(), static_cast<int>(point.y() - lineThickness / 2)}, lengthInWorld, lineThickness, color);
    }

    void View::drawVerticalLine(const ScreenCoordinates& point, int length, const SDL_Color& color, size_t lineThickness) const {
        drawRectangle(ScreenCoordinates{static_cast<int>(point.x() - lineThickness / 2), point.y()}, lineThickness, length, color);
    }

    void View::drawVerticalLine(const model::WorldCoordinates& point,
                                int                            lengthInWorld,
                                const SDL_Color&               color,
                                size_t                         lineThickness) const {
        drawRectangle(
            model::WorldCoordinates{point.x(), static_cast<int>(point.y() - lineThickness / 2)}, lineThickness, lengthInWorld, color);
    }

    void View::setDrawColor(const SDL_Color& color) const {
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, 255);
    }
    const Grid& View::grid() const {
        return m_grid;
    }

    void View::clear() {
        m_actionEditBoxes.clear();
    }

} // namespace view
