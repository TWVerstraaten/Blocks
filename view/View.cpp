//
// Created by pc on 15-12-20.
//

#include "View.h"

#include "../model/Model.h"
#include "Rectangle.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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

        m_window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, initialWidth,
                                    initialHeight, SDL_WINDOW_RESIZABLE);
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
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 0);
        SDL_RenderClear(m_renderer);

        drawGridLines();
        drawClusters(model.clusters());
        drawLevel(model.level());

        for (auto& actionEditBox : m_actionEditBoxes) {
            actionEditBox->render(m_renderer);
        }

        SDL_RenderPresent(m_renderer);
    }

    void View::drawClusters(const std::vector<model::Cluster>& clusters) const {
        for (const auto& cluster : clusters) {
            for (auto it = cluster.localIndexPairs().begin(); it != cluster.localIndexPairs().end(); ++it) {
                const SDL_Point center = {
                    static_cast<int>(m_grid.blockSize() * (0.5 + cluster.rotationPivot().column() - it->column())),
                    static_cast<int>(m_grid.blockSize() * (0.5 + cluster.rotationPivot().row() - it->row()))};
                if (m_assetsHandler->renderTexture(AssetHandler::TEXTURE_ENUM::CLUSTER,
                                                   {m_grid.xAt(it->column() + cluster.dynamicColumnOffset()),
                                                    m_grid.yAt(it->row() + cluster.dynamicRowOffset()),
                                                    m_grid.blockSize(), m_grid.blockSize()},
                                                   m_renderer, cluster.angle(), &center)) {
                    continue;
                } else {
                    Rectangle::render(m_grid.xAt(it->column() + cluster.dynamicColumnOffset()),
                                      m_grid.yAt(it->row() + cluster.dynamicRowOffset()), m_grid.blockSize(),
                                      m_grid.blockSize(), {82, 122, 28, 100}, m_renderer);
                }
            }
        }
    }

    void View::drawGridLines() const {
        int w, h;
        SDL_GetWindowSize(m_window, &w, &h);
        SDL_SetRenderDrawColor(m_renderer, 180, 180, 180, 255);

        int x = m_grid.xAt(m_grid.firstColumnInView());
        while (x < w) {
            SDL_RenderDrawLine(m_renderer, x, 0, x, h);
            x += m_grid.blockSize();
        }
        int y = m_grid.yAt(m_grid.firstRowInView());
        while (y < h) {
            SDL_RenderDrawLine(m_renderer, 0, y, w, y);
            y += m_grid.blockSize();
        }
    }

    void View::drawLevel(const model::Level& level) const {
        for (const auto& block : level.dynamicBlocks()) {
            if (m_assetsHandler->renderTexture(AssetHandler::getTextureEnum(block.second),
                                               {m_grid.xAt(block.first.column()), m_grid.yAt(block.first.row()),
                                                m_grid.blockSize(), m_grid.blockSize()},
                                               m_renderer, 0)) {
                continue;
            } else {
                Rectangle rect = {m_grid.xAt(block.first.column()),
                                  m_grid.yAt(block.first.row()),
                                  m_grid.blockSize(),
                                  m_grid.blockSize(),
                                  {0, 0, 0, 255}};
                switch (block.second) {
                    case model::Level::DYNAMIC_BLOCK_TYPE::ROTATE_CW:
                        rect.setColor({100, 255, 255, 100});
                        break;
                    case model::Level::DYNAMIC_BLOCK_TYPE::ROTATE_CCW:
                        rect.setColor({255, 100, 255, 100});
                        break;
                    case model::Level::DYNAMIC_BLOCK_TYPE::NONE:
                        break;
                }
                rect.render(m_renderer);
            }
        }

        for (const auto& block : level.instantBlocks()) {
            if (m_assetsHandler->renderTexture(AssetHandler::getTextureEnum(block.second),
                                               {m_grid.xAt(block.first.column()), m_grid.yAt(block.first.row()),
                                                m_grid.blockSize(), m_grid.blockSize()},
                                               m_renderer, 0)) {
                continue;
            } else {
                Rectangle rect = {m_grid.xAt(block.first.column()),
                                  m_grid.yAt(block.first.row()),
                                  m_grid.blockSize(),
                                  m_grid.blockSize(),
                                  {0, 0, 0, 255}};

                switch (block.second) {
                    case model::Level::INSTANT_BLOCK_TYPE::NONE:
                        break;
                    case model::Level::INSTANT_BLOCK_TYPE::KILL:
                        rect.setColor({90, 90, 90, 180});
                        break;
                }
                rect.render(m_renderer);
            }
        }
    }

    void View::zoom(int amount) {
        m_zoomParameter += amount;
        m_grid.setBlockSize(m_zoomParameter);
    }
    void View::translate(int dx, int dy) {
        m_grid.translate(dx, dy);
    }

    std::pair<int, int> View::windowSize() const {
        int windowWidth, windowHeight;
        SDL_GetWindowSize(m_window, &windowWidth, &windowHeight);
        return {windowWidth, windowHeight};
    }

    void View::addActionEditBox(const model::Cluster& cluster) {
        m_actionEditBoxes.emplace_back(
            new widget::ActionEditBox(0, m_actionEditBoxes.size() * 200, 200, 200, m_assetsHandler.get(), cluster));
    }

    const std::vector<std::unique_ptr<widget::ActionEditBox>>& View::actionEditBoxes() const {
        return m_actionEditBoxes;
    }

} // namespace view