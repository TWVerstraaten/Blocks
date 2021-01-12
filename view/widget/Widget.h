//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_WIDGET_H
#define BLOCKS_WIDGET_H

union SDL_Event;
struct SDL_Renderer;

namespace view ::widget {
    class Widget {

      public:
        /****** PUBLIC VIRTUAL FUNCTIONS  ******/
        virtual void render(SDL_Renderer* renderer) = 0;
        virtual void keyEvent([[maybe_unused]] const SDL_Event& event){};
        virtual void leftClickEvent([[maybe_unused]] const SDL_Event& event){};
        virtual void mouseDragEvent([[maybe_unused]] const SDL_Event& event){};
        virtual void mouseWheelEvent([[maybe_unused]] const SDL_Event& event){};
    };
} // namespace view::widget

#endif // BLOCKS_WIDGET_H
