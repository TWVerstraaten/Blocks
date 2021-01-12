//
// Created by pc on 29-12-20.
//

#ifndef BLOCKS_BLOCKSELECTWIDGET_H
#define BLOCKS_BLOCKSELECTWIDGET_H

#include "../../model/BlockType_typedef.h"
#include "../../model/Level_enums.h"
#include "RectWidget.h"

#include <vector>

namespace view {
    class Assets;

    namespace widget {
        class BlockSelectWidget : public RectWidget {

          public:
            /****** CONSTRUCTORS / DESTRUCTORS  ******/
            explicit BlockSelectWidget(const SDL_Rect& rect);

            /****** PUBLIC VIRTUAL FUNCTIONS  ******/
            void render(SDL_Renderer* renderer) override;
            void leftClickEvent(const SDL_Event& event) override;

            /****** CONST GETTERS  ******/
            [[nodiscard]] model::BlockType selectedBlockType() const;

            /****** CONST FUNCTIONS  ******/
            ScreenXY screenXY(const model::BlockType& type);
            void     setSelectedBlock(const ScreenXY& screenXY);

          private:
            /****** PRIVATE CONST FUNCTIONS  ******/
            [[nodiscard]] ScreenXY indexToScreenXY(size_t index) const;

            /****** DATA MEMBERS  ******/
            size_t        m_selected = 0;
        };
    } // namespace widget
} // namespace view

#endif // BLOCKS_BLOCKSELECTWIDGET_H
