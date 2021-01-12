//
// Created by pc on 29-12-20.
//

#include "BlockSelectWidget.h"

#include "../../global/defines.h"
#include "../../global/geom.h"
#include "../../global/overloaded.h"
#include "../Assets.h"
#include "../Mouse.h"
#include "../Rectangle.h"
#include "../View.h"
#include "../toColor.h"
#include "RectWidget_constants.h"

#include <cassert>

namespace view::widget {

    BlockSelectWidget::BlockSelectWidget(const SDL_Rect& rect) : RectWidget(rect) {
    }

    void BlockSelectWidget::render(SDL_Renderer* renderer) {
        using namespace model;
        Rectangle::render(geom::pad(m_rect, RECT_WIDGET_PADDING), color::EDIT_BOX_BACKGROUND, renderer);

        for (const BlockType type : s_allTypes) {
            std::visit(overloaded{[renderer, this](const model::FLOOR_BLOCK_TYPE type) {
                                      view::View::drawSquare(screenXY(type), app::BLOCK_SIZE_IN_WORLD, toColor(type), renderer);
                                  },
                                  [renderer, this](const CLUSTER_TYPE type) {
                                      Assets::renderTexture(
                                          TEXTURE_ENUM::CLUSTER, screenXY(type), app::BLOCK_SIZE_IN_WORLD, app::BLOCK_SIZE_IN_WORLD, renderer);
                                  },
                                  [renderer, this](const auto type) {
                                      Assets::renderTexture(
                                          Assets::getTextureEnum(type), screenXY(type), app::BLOCK_SIZE_IN_WORLD, app::BLOCK_SIZE_IN_WORLD, renderer);
                                  }},
                       type);
            if (type == selectedBlockType()) {
                const auto xy = screenXY(type);
                view::View::drawSquareOutline(xy, app::BLOCK_SIZE_IN_WORLD, 4, view::color::BLACK, renderer);
            }
        }
    }

    void BlockSelectWidget::leftClickEvent([[maybe_unused]] const SDL_Event& event) {
        setSelectedBlock(Mouse::mouseXY());
    }

    ScreenXY BlockSelectWidget::screenXY(const model::BlockType& type) {
        const auto it = std::find_if(D_CIT(model::s_allTypes), D_FUNC(blockType, blockType == type));
        assert(it != model::s_allTypes.end());
        return indexToScreenXY(std::distance(model::s_allTypes.begin(), it));
    }

    void BlockSelectWidget::setSelectedBlock(const ScreenXY& screenXY) {
        const auto       x            = screenXY.x() - m_rect.x;
        const auto       y            = screenXY.y() - m_rect.y;
        static const int margin       = 10;
        const int        blocksPerRow = (m_rect.w - margin) / (app::BLOCK_SIZE_IN_WORLD + margin);
        int index = ((x - margin) / (app::BLOCK_SIZE_IN_WORLD + margin)) + blocksPerRow * ((y - margin) / (app::BLOCK_SIZE_IN_WORLD + margin));
        if (index >= 0 && static_cast<size_t>(index) < model::s_allTypes.size()) {
            m_selected = index;
        }
    }

    ScreenXY BlockSelectWidget::indexToScreenXY(size_t index) const {
        assert(index < model::s_allTypes.size());
        static const int margin       = 10;
        const int        blocksPerRow = (m_rect.w - margin) / (app::BLOCK_SIZE_IN_WORLD + margin);

        return ScreenXY{static_cast<int>(m_rect.x + margin + (index % blocksPerRow) * (app::BLOCK_SIZE_IN_WORLD + margin)),
                        static_cast<int>(m_rect.y + margin + (index / blocksPerRow) * (app::BLOCK_SIZE_IN_WORLD + margin))};
    }

    model::BlockType BlockSelectWidget::selectedBlockType() const {
        assert(m_selected < model::s_allTypes.size());
        return model::s_allTypes.at(m_selected);
    }
} // namespace view::widget