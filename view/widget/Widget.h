//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_WIDGET_H
#define BLOCKS_WIDGET_H

union SDL_Event;

namespace view {
    class Widget {

      public:
        virtual void keyEvent(const SDL_Event& event)       = 0;
        virtual void leftClickEvent(const SDL_Event& event) = 0;
        virtual void mouseDragEvent(const SDL_Event& event) = 0;
    };
} // namespace view

#endif // BLOCKS_WIDGET_H
