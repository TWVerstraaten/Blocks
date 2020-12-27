//
// Created by pc on 26-12-20.
//

#ifndef BLOCKS_SCROLLAREA_H
#define BLOCKS_SCROLLAREA_H

#include "../Assets.h"
#include "ActionEditBox.h"
#include "RectWidget.h"

#include <list>
#include <memory>

namespace view { namespace widget {
    class ScrollArea : public RectWidget {

      public:
        ScrollArea(const SDL_Rect& rect);

        void                                    update(SDL_Renderer* renderer) override;
        void                                    init(const view::Assets* assets);
        void                                    render(SDL_Renderer* renderer) override;
        void                                    loseFocus() override;
        void                                    keyEvent(const SDL_Event& event) override;
        void                                    leftClickEvent(const SDL_Event& event) override;
        void                                    mouseDragEvent(const SDL_Event& event) override;
        void                                    mouseWheelEvent(const SDL_Event& event) override;
        void                                    addActionBox(const model::Cluster& cluster);
        std::list<view::widget::ActionEditBox>& children();

      private:
        void           setHeightAndPositions();
        ActionEditBox* widgetUnderMouse();
        ActionEditBox* focusedWidget();
        void           renderScrollBar(SDL_Renderer* renderer);

        bool                     m_needsUpdate    = true;
        bool                     m_firstRender    = true;
        double                   m_scrollFraction = 0.0;
        size_t                   m_height         = 0;
        const Assets*            m_assets;
        std::list<ActionEditBox> m_children;
    };
} } // namespace view::widget

#endif // BLOCKS_SCROLLAREA_H
