//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_WIDGET_H
#define BLOCKS_WIDGET_H

union SDL_Event;
struct SDL_Renderer;

namespace view {
    namespace widget {
        class Widget {

          public:
            virtual void render(SDL_Renderer* renderer) = 0;

            virtual void keyEvent(const SDL_Event& event) = 0;
            virtual void mouseWheelEvent(const SDL_Event& event){};
            virtual void leftClickEvent(const SDL_Event& event) = 0;
            virtual void mouseDragEvent(const SDL_Event& event) = 0;
        };
    } // namespace widget
} // namespace view

#endif // BLOCKS_WIDGET_H
