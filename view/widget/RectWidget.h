//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_RECTWIDGET_H
#define BLOCKS_RECTWIDGET_H

#include "Widget.h"

#include <SDL.h>

namespace view {
    namespace widget {
        class RectWidget : public Widget {

          public:
            explicit RectWidget(SDL_Rect rect);

            virtual void loseFocus();
            virtual void update(SDL_Renderer* renderer) = 0;

            bool pointIsOverWidget(const SDL_Point& point) const;
            bool hasFocus() const;
            void getFocus();
            void setWidth(int width);
            void setHeight(int height);
            void setX(int x);
            void setY(int y);
            int  height() const;

          protected:
            SDL_Rect m_rect;

          private:
            bool m_hasFocus = false;
        };
    } // namespace widget
} // namespace view
#endif // BLOCKS_RECTWIDGET_H
