//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_RECTWIDGET_H
#define BLOCKS_RECTWIDGET_H

#include "Widget.h"

#include <SDL2/SDL.h>

namespace view {

    class RectWidget : public Widget {

      public:
        explicit RectWidget(SDL_Rect rect);

        virtual void loseFocus();

        void getFocus();
        bool pointIsOverWidget(const SDL_Point& point) const;
        bool hasFocus() const;

      protected:
        SDL_Rect m_rect;

      private:
        bool m_hasFocus = false;
    };
} // namespace view

#endif // BLOCKS_RECTWIDGET_H
