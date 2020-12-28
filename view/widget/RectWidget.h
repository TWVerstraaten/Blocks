//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_RECTWIDGET_H
#define BLOCKS_RECTWIDGET_H

#include "../ScreenXY.h"
#include "Widget.h"

#include <SDL.h>

namespace view {
    namespace widget {
        class RectWidget : public Widget {

          public:
            /****** CONSTRUCTORS / DESTRUCTORS  ******/
            explicit RectWidget(SDL_Rect rect);

            /****** PUBLIC VIRTUAL FUNCTIONS  ******/
            virtual void loseFocus();
            virtual void update(SDL_Renderer* renderer) = 0;
            virtual void setHeight(int height);
            virtual void setX(int x);

            /****** CONST GETTERS  ******/
            bool hasFocus() const;
            int  height() const;

            /****** CONST FUNCTIONS  ******/
            bool pointIsOverWidget(const ScreenXY& point) const;

            /****** NON CONST FUNCTIONS  ******/
            void getFocus();
            void setWidth(int width);
            void setY(int y);

          protected:
            /****** DATA MEMBERS  ******/
            SDL_Rect m_rect;

          private:
            bool m_hasFocus = false;
        };
    } // namespace widget
} // namespace view

#endif // BLOCKS_RECTWIDGET_H
