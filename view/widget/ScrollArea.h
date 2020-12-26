//
// Created by pc on 26-12-20.
//

#ifndef BLOCKS_SCROLLAREA_H
#define BLOCKS_SCROLLAREA_H

#include "../Assets.h"
#include "RectWidget.h"

#include <memory>
#include <vector>

namespace view::widget {
    class ScrollArea : public RectWidget {

      public:
        ScrollArea(const SDL_Rect& rect);

        void update(SDL_Renderer* renderer) override;
        void init(const view::Assets* assets);
        void render(SDL_Renderer* renderer) override;
        void loseFocus() override;
        void keyEvent(const SDL_Event& event) override;
        void leftClickEvent(const SDL_Event& event) override;
        void mouseDragEvent(const SDL_Event& event) override;
        void mouseWheelEvent(const SDL_Event& event) override;

      private:
        void        setHeightAndPositions();
        RectWidget* widgetUnderMouse() const;
        RectWidget* focusedWidget() const;

        bool                                     m_needsUpdate    = true;
        bool                                     m_firstRender    = true;
        double                                   m_scrollFraction = 0.0;
        size_t                                   m_height         = 0;
        const Assets*                            m_assets;
        std::vector<std::unique_ptr<RectWidget>> m_children;
    };
} // namespace view::widget

#endif // BLOCKS_SCROLLAREA_H
