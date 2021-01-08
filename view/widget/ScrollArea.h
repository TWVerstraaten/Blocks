//
// Created by pc on 26-12-20.
//

#ifndef BLOCKS_SCROLLAREA_H
#define BLOCKS_SCROLLAREA_H

#include "../Assets.h"
#include "CommandEditBox.h"

#include <list>

namespace view::widget {
    class ScrollArea : public RectWidget {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        explicit ScrollArea(const SDL_Rect& rect);
        ScrollArea(const ScrollArea& other);

        /****** PUBLIC VIRTUAL FUNCTIONS  ******/
        void loseFocus() override;
        void update(SDL_Renderer* renderer) override;
        void render(SDL_Renderer* renderer) override;
        void keyEvent(const SDL_Event& event) override;
        void leftClickEvent(const SDL_Event& event) override;
        void mouseDragEvent(const SDL_Event& event) override;
        void mouseWheelEvent(const SDL_Event& event) override;
        void setHeight(int height) override;
        void setX(int x) override;

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] bool hasChildWithFocus() const;

        /****** NON CONST FUNCTIONS  ******/
        void                                              init(const view::Assets* assets);
        void                                              addCommandEditBox(const model::Cluster& cluster);
        void                                              setHeightAndPositions();
        CommandEditBox*                                   focusedWidget();
        std::list<view::widget::CommandEditBox>::iterator findCommandEditBox(size_t index);
        std::list<view::widget::CommandEditBox>&          children();

      private:
        /****** PRIVATE NON CONST FUNCTIONS  ******/
        CommandEditBox* widgetUnderMouse();
        void            renderScrollBar(SDL_Renderer* renderer);

        /****** DATA MEMBERS  ******/
        bool                      m_needsUpdate    = true;
        bool                      m_firstRender    = true;
        double                    m_scrollFraction = 0.0;
        size_t                    m_height         = 0;
        const Assets*             m_assets         = nullptr;
        std::list<CommandEditBox> m_children;
    };
} // namespace view::widget

#endif // BLOCKS_SCROLLAREA_H
