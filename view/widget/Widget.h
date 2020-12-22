//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_WIDGET_H
#define BLOCKS_WIDGET_H

#include <SDL2/SDL.h>

namespace view {

    class Widget {

      public:
        Widget(SDL_Rect rect);

        virtual void handleKeyEvent(const SDL_Event& event)                          = 0;
        virtual void handleMouseClickEvent(const SDL_Event& event, bool leftClicked) = 0;
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

#endif // BLOCKS_WIDGET_H
