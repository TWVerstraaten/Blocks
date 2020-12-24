//
// Created by pc on 15-12-20.
//

#include "View.h"

#include "../model/Model.h"
#include "../model/WorldXY.h"
#include "Color.h"
#include "ScreenXY.h"

#include <SDL_image.h>
#include <algorithm>
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

        Uint32 initialWidth  = 1000;
        Uint32 initialHeight = 800;

        m_window =
            SDL_CreateWindow("Blocks", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, initialWidth, initialHeight, SDL_WINDOW_RESIZABLE);
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
        setDrawColor(color::BACKGROUND);
        SDL_RenderClear(m_renderer);

        drawLevelBlocks(model.level());
        drawClusters(model.clusters());
        drawBlocks(model.level());

        //        for (const auto& cluster : model.clusters()) {
        //            const auto points = cluster.cornerPoints(0);
        //            for (const auto& it : points) {
        //                                        drawRectangle(model::GridXY::fromWorldXY(actionEditIt),
        //                                        model::WorldXY::m_blockSizeInWorld,
        //                                                      model::WorldXY::m_blockSizeInWorld, color::BLUE);
        //                drawPoint(it, color::RED, 8);
        //            }
        //        }
        drawActionEditBoxes();
    }

    void View::drawActionEditBoxes() {
        int yOffset = 2.4 * cst::s_padding;
        for (auto& actionEditBox : m_actionEditBoxes) {
            actionEditBox.setY(yOffset);
            actionEditBox.render(m_renderer);
            yOffset += actionEditBox.height() + 2.4 * cst::s_padding;
        }
    }

    void View::drawClusters(const std::vector<model::Cluster>& clusters) const {
        for (const auto& cluster : clusters) {
            const ScreenXY offset = ScreenXY::fromWorldXYAsVector(cluster.dynamicWorldOffset(), m_viewPort);
            for (auto it = cluster.gridXY().begin(); it != cluster.gridXY().end(); ++it) {
                const SDL_Point center = {static_cast<int>(m_viewPort.blockSizeInScreen() * (0.5 + cluster.rotationPivot().x() - it->x())),
                                          static_cast<int>(m_viewPort.blockSizeInScreen() * (0.5 + cluster.rotationPivot().y() - it->y()))};
                assert(m_assets->renderTexture(TextureWrapper::TEXTURE_ENUM::CLUSTER,
                                               ScreenXY::fromGridXY(*it, m_viewPort) + offset + ScreenXY{2, 2},
                                               m_viewPort.blockSizeInScreen() - 4,
                                               m_viewPort.blockSizeInScreen() - 4,
                                               m_renderer,
                                               cluster.angle(),
                                               &center));
            }
            m_assets->renderText(cluster.name() + " " + std::to_string(cluster.index()),
                                 ScreenXY::fromGridXY(cluster.gridXY().front(), m_viewPort),
                                 m_renderer);
        }
    }

    void View::drawLevelBlocks(const model::Level& level) const {
        for (const auto& block : level.levelBlocks()) {
            drawRectangle(ScreenXY::fromGridXY(block, m_viewPort) + ScreenXY{2, 2},
                          m_viewPort.blockSizeInScreen() - 4,
                          m_viewPort.blockSizeInScreen() - 4,
                          color::BACKGROUND_PLAYABLE);
        }
    }

    void View::drawBlocks(const model::Level& level) const {
        for (const auto& block : level.dynamicBlocks()) {
            assert(m_assets->renderTexture(Assets::getTextureEnum(block.second),
                                           ScreenXY::fromGridXY(block.first, m_viewPort),
                                           m_viewPort.blockSizeInScreen(),
                                           m_viewPort.blockSizeInScreen(),
                                           m_renderer));
        }

        for (const auto& block : level.instantBlocks()) {
            assert(m_assets->renderTexture(Assets::getTextureEnum(block.second),
                                           ScreenXY::fromGridXY(block.first, m_viewPort),
                                           m_viewPort.blockSizeInScreen(),
                                           m_viewPort.blockSizeInScreen(),
                                           m_renderer));
        }
    }

    void View::zoom(int amount) {
        m_zoomParameter += amount;
        m_zoomParameter = std::max(m_zoomParameter, -28);
        m_zoomParameter = std::min(m_zoomParameter, 6);
        m_viewPort.setScale(m_zoomParameter);
    }

    void View::translate(int dx, int dy) {
        m_viewPort.translate(dx, dy);
    }

    SDL_Point View::windowSize() const {
        int windowWidth, windowHeight;
        SDL_GetWindowSize(m_window, &windowWidth, &windowHeight);
        return {windowWidth, windowHeight};
    }

    std::list<widget::ActionEditBox>& View::actionEditBoxes() {
        return m_actionEditBoxes;
    }

    void View::drawRectangle(const ScreenXY& point, int width, int height, const SDL_Color& color) const {
        setDrawColor(color);
        const SDL_Rect rect{point.x(), point.y(), width, height};
        SDL_RenderFillRect(m_renderer, &rect);
    }

    void View::drawRectangle(const model::WorldXY& point, int widthInWorld, int heightInWorld, const SDL_Color& color) const {
        drawRectangle(ScreenXY::fromWorldXY(point, m_viewPort),
                      m_viewPort.worldToScreenLength(widthInWorld),
                      m_viewPort.worldToScreenLength(heightInWorld),
                      color);
    }

    void View::drawPoint(const ScreenXY& point, const SDL_Color& color, int pointSize) const {
        drawRectangle(ScreenXY{point.x() - pointSize / 2, point.y() - pointSize / 2}, pointSize, pointSize, color);
    }

    void View::drawPoint(const model::WorldXY& point, const SDL_Color& color, int pointSize) const {
        drawRectangle(model::WorldXY{point.x() - pointSize / 2, point.y() - pointSize / 2}, pointSize, pointSize, color);
    }

    void View::drawHorizontalLine(const ScreenXY& point, int length, const SDL_Color& color, size_t lineThickness) const {
        if (lineThickness == 1) {
            setDrawColor(color);
            SDL_RenderDrawLine(m_renderer, point.x(), point.y(), point.x() + length, point.y());
        } else {
            drawRectangle(ScreenXY{point.x(), static_cast<int>(point.y() - lineThickness / 2)}, length, lineThickness, color);
        }
    }

    void View::drawHorizontalLine(const model::WorldXY& point, int lengthInWorld, const SDL_Color& color, size_t lineThickness) const {
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

    void View::drawVerticalLine(const model::WorldXY& point, int lengthInWorld, const SDL_Color& color, Uint32 lineThickness) const {
        drawRectangle(model::WorldXY{point.x(), static_cast<int>(point.y() - lineThickness / 2)}, lineThickness, lengthInWorld, color);
    }

    void View::setDrawColor(const SDL_Color& color) const {
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, 255);
    }
    const ViewPort& View::viewPort() const {
        return m_viewPort;
    }

    void View::clear() {
        m_actionEditBoxes.clear();
    }

    void View::initActionBoxes(const std::vector<model::Cluster>& clusters) {
        m_actionEditBoxes.clear();
        for (const auto& cluster : clusters) {
            addActionBox(cluster);
        }
    }

    void View::renderPresent() const {
        SDL_RenderPresent(m_renderer);
    }

    SDL_Renderer* View::renderer() const {
        return m_renderer;
    }

    Assets& View::assets() const {
        return *m_assets;
    }

    void View::addActionBox(const model::Cluster& cluster) {
        m_actionEditBoxes.emplace_back(view::widget::ActionEditBox(30, 0, cst::s_actionEditBoxWidth, 0, m_assets.get(), cluster));
        m_actionEditBoxes.back().setHighLightedLine(cluster.actionIndex());
        m_actionEditBoxes.back().setActive(cluster.isAlive());
    }

    void View::updateActionBoxes(const std::vector<model::Cluster>& clusters) {
        m_actionEditBoxes.remove_if([&](const widget::ActionEditBox& box) {
            return std::find_if(clusters.begin(), clusters.end(), [&](const auto& cluster) {
                       return cluster.index() == box.clusterIndex();
                   }) == clusters.end();
        });

        for (auto& actionBox : m_actionEditBoxes) {
            auto it = std::find_if(
                clusters.begin(), clusters.end(), [&](const auto& cluster) { return cluster.index() == actionBox.clusterIndex(); });
            assert(it != clusters.end());
            if (it->isAlive()) {
                actionBox.setHighLightedLine(it->actionIndex());
            }
            actionBox.setActive(it->isAlive());
        }
        auto it = std::find_if(clusters.begin(), clusters.end(), [&](const model::Cluster& cluster) {
            return std::find_if(m_actionEditBoxes.begin(), m_actionEditBoxes.end(), [&](const widget::ActionEditBox& box) {
                       return box.clusterIndex() == cluster.index();
                   }) == m_actionEditBoxes.end();
        });
        while (it != clusters.end()) {
            addActionBox(*it);
            it = std::find_if(clusters.begin(), clusters.end(), [&](const model::Cluster& cluster) {
                return std::find_if(m_actionEditBoxes.begin(), m_actionEditBoxes.end(), [&](const widget::ActionEditBox& box) {
                           return box.clusterIndex() == cluster.index();
                       }) == m_actionEditBoxes.end();
            });
        }
    }

} // namespace view
