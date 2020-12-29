//
// Created by pc on 29-12-20.
//

#include "BlockSelectWidget.h"

#include "../../global/cst.h"
#include "../../global/geom.h"
#include "../../global/overloaded.h"
#include "../Assets.h"
#include "../Mouse.h"
#include "../Rectangle.h"
#include "../View.h"
#include "../color.h"

#include <SDL.h>
#include <algorithm>
#include <cassert>

namespace view::widget {

    BlockSelectWidget::BlockSelectWidget(const SDL_Rect& rect) : RectWidget(rect) {
    }

    void BlockSelectWidget::render(SDL_Renderer* renderer) {
        using namespace cst;
        using namespace model;
        Rectangle::render(geom::pad(m_rect, LINE_EDIT_PADDING), color::EDIT_BOX_BACKGROUND, renderer);

        for (const BlockType type : s_allTypes) {
            std::visit(overloaded{[renderer, this](const model::FLOOR_BLOCK type) {
                                      switch (type) {
                                          case model::FLOOR_BLOCK::CLUSTER:
                                              m_assets->renderTexture(TextureWrapper::TEXTURE_ENUM::CLUSTER,
                                                                      screenXY(type),
                                                                      BLOCK_SIZE_IN_WORLD,
                                                                      BLOCK_SIZE_IN_WORLD,
                                                                      renderer);
                                              break;
                                          default:
                                              view::View::drawSquare(screenXY(type),
                                                                     BLOCK_SIZE_IN_WORLD,
                                                                     type == model::FLOOR_BLOCK::LEVEL
                                                                         ? view::color::BACKGROUND_PLAYABLE
                                                                         : view::color::BACKGROUND_START,
                                                                     renderer);
                                              break;
                                      }
                                  },
                                  [renderer, this](const auto type) {
                                      m_assets->renderTexture(Assets::getTextureEnum(type),
                                                              screenXY(type),
                                                              BLOCK_SIZE_IN_WORLD,
                                                              BLOCK_SIZE_IN_WORLD,
                                                              renderer);
                                  }},
                       type);
            if (type == selectedBlockType()) {
                const auto xy = screenXY(type);
                view::View::drawSquareOutline(xy, cst::BLOCK_SIZE_IN_WORLD, 4, view::color::BLACK, renderer);
            }
        }
    }

    void BlockSelectWidget::leftClickEvent(const SDL_Event& event) {
        setSelectedBlock(Mouse::mouseXY());
    }

    void BlockSelectWidget::init(const Assets* assets) {
        m_assets = assets;
    }

    ScreenXY BlockSelectWidget::screenXY(const model::BlockType& type) {
        const auto it = std::find_if(model::s_allTypes.begin(),
                                     model::s_allTypes.end(),
                                     [&](const model::BlockType& blockType) { return blockType == type; });
        assert(it != model::s_allTypes.end());
        return indexToScreenXY(std::distance(model::s_allTypes.begin(), it));
    }

    void BlockSelectWidget::setSelectedBlock(const ScreenXY& screenXY) {
        const auto       x            = screenXY.x() - m_rect.x;
        const auto       y            = screenXY.y() - m_rect.y;
        static const int margin       = 10;
        const int        blocksPerRow = (m_rect.w - margin) / (cst::BLOCK_SIZE_IN_WORLD + margin);
        int              index        = ((x - margin) / (cst::BLOCK_SIZE_IN_WORLD + margin)) +
                    blocksPerRow * ((y - margin) / (cst::BLOCK_SIZE_IN_WORLD + margin));
        if (index >= 0 && static_cast<size_t>(index) < model::s_allTypes.size()) {
            m_selected = index;
        }
    }

    ScreenXY BlockSelectWidget::indexToScreenXY(size_t index) const {
        assert(index < model::s_allTypes.size());
        static const int margin       = 10;
        const int        blocksPerRow = (m_rect.w - margin) / (cst::BLOCK_SIZE_IN_WORLD + margin);

        return ScreenXY{
            static_cast<int>(m_rect.x + margin + (index % blocksPerRow) * (cst::BLOCK_SIZE_IN_WORLD + margin)),
            static_cast<int>(m_rect.y + margin + (index / blocksPerRow) * (cst::BLOCK_SIZE_IN_WORLD + margin))};
    }

    model::BlockType BlockSelectWidget::selectedBlockType() const {
        assert(m_selected < model::s_allTypes.size());
        return model::s_allTypes.at(m_selected);
    }
} // namespace view::widget