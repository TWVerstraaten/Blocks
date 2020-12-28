//
// Created by pc on 28-12-20.
//

#ifndef BLOCKS_MODELVIEWINTERFACE_H
#define BLOCKS_MODELVIEWINTERFACE_H

#include "../model/Cluster.h"
#include "../view/widget/CommandEditBox.h"
#include "Application_enums.h"

#include <memory>
#include <stack>

namespace view {
    class View;
    namespace widget {
        class ScrollArea;
    }
} // namespace view

namespace model {
    class Model;
    class GridXY;
} // namespace model

namespace action {
    class Action;
}

namespace app {
    class Application_Edit;

    class ModelViewInterface {

      public:
        /****** PUBLIC STATIC FUNCTIONS  ******/
        static void                            updateCommandScrollArea(const std::list<model::Cluster>& clusters,
                                                                       view::widget::ScrollArea&        scrollArea,
                                                                       APP_MODE                         mode);
        static std::unique_ptr<action::Action> clearBlockStatic(model::Model&             model,
                                                                view::widget::ScrollArea& scrollArea,
                                                                const model::GridXY&      point);
        static void interactWithInstantBlocks(model::Model& model, view::widget::ScrollArea& scrollArea);
        static void interactWithDynamicBlocks(model::Model& model, view::widget::ScrollArea& scrollArea);

        /****** NON CONST FUNCTIONS  ******/
        void handleKeyEvent(const SDL_Event& event, view::widget::ScrollArea& scrollArea, model::Model& model);
        void leftMouseDrag(model::Model&             model,
                           view::View&               view,
                           view::widget::ScrollArea& scrollArea,
                           const model::GridXY&      point,
                           const model::GridXY&      previousPoint);
        void clearBlock(model::Model&             model,
                        view::View&               view,
                        view::widget::ScrollArea& scrollArea,
                        const model::GridXY&      point);
        void leftMouseClick(model::Model&             model,
                            view::View&               view,
                            view::widget::ScrollArea& scrollArea,
                            const model::GridXY&      point);
        void undo(Application_Edit& applicationEdit);
        void redo(Application_Edit& applicationEdit);

      private:
        /****** PRIVATE NON CONST FUNCTIONS  ******/
        void addAction(std::unique_ptr<action::Action> action);

        /****** PUBLIC STATIC DATA MEMBERS  ******/
        std::stack<std::unique_ptr<action::Action>> m_undoStack;
        std::stack<std::unique_ptr<action::Action>> m_redoStack;
    };
} // namespace app

#endif // BLOCKS_MODELVIEWINTERFACE_H
