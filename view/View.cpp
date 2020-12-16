//
// Created by pc on 15-12-20.
//

#include "View.h"

#include "../model/Level.h"
#include "../model/Model.h"
#include "Rectangle.h"

#include <SDL2/SDL.h>

namespace view {

    View::View() {
        size_t initialWidth  = 900;
        size_t initialHeight = 700;
        m_grid.update(initialWidth, initialHeight);

        m_window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, initialWidth,
                                    initialHeight, SDL_WINDOW_RESIZABLE);
        if (!m_window) {
            std::cout << "Failed to create window\n";
            std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
            return;
        }
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (!m_renderer) {
            std::cout << "Failed to get window's surface\n";
            std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
            return;
        }
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    }

    View::~View() {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
    }

    void View::draw(const model::Model& model) {
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_RenderClear(m_renderer);

        drawGridLines();
        drawLevel(model.level());
        drawClusters(model.clusters());

        SDL_RenderPresent(m_renderer);
    }

    void View::drawClusters(const std::vector<model::Cluster>& clusters) const {
        for (const auto& cluster : clusters) {
            for (auto it = cluster.localIndexPairs().begin(); it != cluster.localIndexPairs().end(); ++it) {
                Rectangle rect = {m_grid.xAt(it->column() + cluster.dynamicColumnOffset()),
                                  m_grid.yAt(it->row() + cluster.dynamicRowOffset()),
                                  m_grid.blockSize(),
                                  m_grid.blockSize(),
                                  {0, 0, 0, 255},
                                  {210, 212, 210, 100}};
                rect.setLineThickNess(8);
                rect.render(m_renderer);
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
            Rectangle rect = {m_grid.xAt(block.first.column()),
                              m_grid.yAt(block.first.row()),
                              m_grid.blockSize(),
                              m_grid.blockSize(),
                              {0, 0, 0, 255}};
            rect.setLineThickNess(4);

            switch (block.second) {
                case model::Level::DYNAMIC_BLOCK_TYPE::ROTATE_CW:
                    rect.setFillColor({100, 255, 255, 100});
                    break;
                case model::Level::DYNAMIC_BLOCK_TYPE::ROTATE_CCW:
                    rect.setFillColor({255, 100, 255, 100});
                    break;
                case model::Level::DYNAMIC_BLOCK_TYPE::NONE:
                    break;
            }
            rect.render(m_renderer);
        }

        for (const auto& block : level.instantBlocks()) {
            Rectangle rect = {m_grid.xAt(block.first.column()),
                              m_grid.yAt(block.first.row()),
                              m_grid.blockSize(),
                              m_grid.blockSize(),
                              {0, 0, 0, 255}};
            rect.setLineThickNess(4);

            switch (block.second) {
                case model::Level::INSTANT_BLOCK_TYPE::NONE:
                    break;
                case model::Level::INSTANT_BLOCK_TYPE::KILL:
                    rect.setFillColor({90, 90, 90, 180});
                    break;
            }
            rect.render(m_renderer);
        }
    }

    void View::zoom(int amount) {
        m_zoomParameter += amount;
        m_grid.setBlockSize(m_zoomParameter);
    }
} // namespace view