//
// Created by pc on 28-12-20.
//

#ifndef BLOCKS_MODELVIEWINTERFACE_H
#define BLOCKS_MODELVIEWINTERFACE_H

#include "../model/BlockType_typedef.h"
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

        /****** PRIVATE ENUMS / TYPEDEFS  ******/
        typedef std::unique_ptr<action::Action> Action_u_ptr;

      public:
        /****** PUBLIC STATIC FUNCTIONS  ******/
        static void updateCommandScrollArea(const std::list<model::Cluster>& clusters, view::widget::ScrollArea& scrollArea, APP_MODE mode);
        static void interactWithInstantBlocks(model::Model& model, view::widget::ScrollArea& scrollArea);
        static void interactWithDynamicBlocks(model::Model& model, view::widget::ScrollArea& scrollArea);
        static Action_u_ptr clearBlockStatic(model::Model& model, view::widget::ScrollArea& scrollArea, const model::GridXY& point);

        /****** NON CONST FUNCTIONS  ******/
        void handleKeyEvent(const SDL_Event& event, view::widget::ScrollArea& scrollArea, model::Model& model);
        void leftMouseDrag(model::Model&             model,
                           view::View&               view,
                           view::widget::ScrollArea& scrollArea,
                           const model::GridXY&      point,
                           const model::GridXY&      previousPoint,
                           const model::BlockType&   selectedBlockType);
        void leftClickControl(model::Model&             model,
                              view::View&               view,
                              view::widget::ScrollArea& scrollArea,
                              const model::GridXY&      point,
                              const model::BlockType&   selectedBlockType);
        void leftMouseClick(model::Model&             model,
                            view::View&               view,
                            view::widget::ScrollArea& scrollArea,
                            const model::GridXY&      point,
                            const model::BlockType&   selectedBlockType);
        void undo(Application_Edit& applicationEdit);
        void redo(Application_Edit& applicationEdit);

      private:
        /****** PRIVATE STATIC FUNCTIONS  ******/
        static Action_u_ptr clearBlockFromCluster(model::Model&                       model,
                                                  view::widget::ScrollArea&           scrollArea,
                                                  const model::GridXY&                point,
                                                  std::list<model::Cluster>::iterator clusterIt);
        static Action_u_ptr addCluster(model::Model& model, view::widget::ScrollArea& scrollArea, const model::GridXY& point);
        static Action_u_ptr linkBlocks(model::Model&             model,
                                       view::widget::ScrollArea& scrollArea,
                                       const model::GridXY&      point,
                                       const model::GridXY&      previousPoint);

        /****** PRIVATE NON CONST FUNCTIONS  ******/
        void addAction(Action_u_ptr&& action);
        void leftMouseClick(model::Model& model, view::View& view, view::widget::ScrollArea& scrollArea, const model::GridXY& point);
        void clearBlock(model::Model& model, view::View& view, view::widget::ScrollArea& scrollArea, const model::GridXY& point);
        void clusterDrag(model::Model&             model,
                         view::View&               view,
                         view::widget::ScrollArea& scrollArea,
                         const model::GridXY&      point,
                         const model::GridXY&      previousPoint);

        /****** PUBLIC STATIC DATA MEMBERS  ******/
        std::stack<Action_u_ptr> m_undoStack;
        std::stack<Action_u_ptr> m_redoStack;
    };
} // namespace app

#endif // BLOCKS_MODELVIEWINTERFACE_H
