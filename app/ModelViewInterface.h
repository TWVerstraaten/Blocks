////
//// Created by pc on 28-12-20.
////
//
//#ifndef BLOCKS_MODELVIEWINTERFACE_H
//#define BLOCKS_MODELVIEWINTERFACE_H
//
//#include "../model/BlockType_typedef.h"
//#include "../model/Cluster.h"
//#include "Application_enums.h"
//
//#include <memory>
//#include <stack>
//
//namespace view {
//    class View;
//    namespace widget {
//        class ScrollArea;
//    }
//} // namespace view
//
//namespace model {
//    class Model;
//    class GridXy;
//} // namespace m_model
//
//namespace action {
//    class Action;
//}
//
//union SDL_Event;
//
//namespace app {
//    class ApplicationEdit;
//
//    class ModelViewInterface {
//
//        /****** PRIVATE ENUMS / TYPEDEFS  ******/
//        typedef std::unique_ptr<action::Action> Action_u_ptr;
//
//      public:
//        /****** PUBLIC STATIC FUNCTIONS  ******/
//        static void updateCommandScrollArea(model::Model& model, view::widget::ScrollArea& scrollArea, APP_MODE mode);
//        static void interactWithInstantBlocks(model::Model& model, view::widget::ScrollArea& scrollArea);
//        static bool interactWithDynamicBlocks(model::Level& level, model::Cluster& cluster);
//        static void stopSpliceOrKillIfNeeded(model::Level& level, model::Cluster& cluster);
//        static void splitIfDisconnected(model::Model& model, view::widget::ScrollArea& scrollArea, model::Cluster& cluster);
//        static void split(model::Model& model, view::widget::ScrollArea& scrollArea, model::Cluster& cluster);
//        static void updateSelection(const std::list<model::Cluster>& clusters, view::widget::ScrollArea& scrollArea);
//        static void removeActionBoxesOfRemovedClusters(const std::list<model::Cluster>& clusters, view::widget::ScrollArea& scrollArea);
//
//        static Action_u_ptr clearBlockFromCluster_static(model::Model& model, view::widget::ScrollArea& scrollArea, const model::GridXy& point);
//
//        /****** NON CONST FUNCTIONS  ******/
//        void handleKeyEvent(const SDL_Event& event, view::widget::ScrollArea& scrollArea, model::Model& model);
//        void leftMouseDrag(model::Model&             model,
//                           view::widget::ScrollArea& scrollArea,
//                           const model::GridXy&      point,
//                           const model::GridXy&      previousPoint,
//                           const model::BlockType&   selectedBlockType);
//        void leftClickControl(model::Model&             model,
//                              view::widget::ScrollArea& scrollArea,
//                              const model::GridXy&      point,
//                              const model::BlockType&   selectedBlockType);
//        void leftMouseClick(model::Model&             model,
//                            view::widget::ScrollArea& scrollArea,
//                            const model::GridXy&      point,
//                            const model::BlockType&   selectedBlockType);
//        void undo(ApplicationEdit& applicationEdit);
//        void redo(ApplicationEdit& applicationEdit);
//
//      private:
//        /****** PRIVATE STATIC FUNCTIONS  ******/
//        static Action_u_ptr clearBlockFromCluster(model::Model&             model,
//                                                  view::widget::ScrollArea& scrollArea,
//                                                  const model::GridXy&      point,
//                                                  model::Cluster&           cluster);
//        static Action_u_ptr addSingleBlockToCluster(model::Model& model, view::widget::ScrollArea& scrollArea, const model::GridXy& point);
//        static Action_u_ptr linkBlocks(model::Model&             model,
//                                       view::widget::ScrollArea& scrollArea,
//                                       const model::GridXy&      point,
//                                       const model::GridXy&      previousPoint);
//
//        /****** PRIVATE NON CONST FUNCTIONS  ******/
//        void addAction(Action_u_ptr&& action);
//        void leftMouseClick(model::Model& model, view::widget::ScrollArea& scrollArea, const model::GridXy& point);
//        void clearBlockFromCluster(model::Model& model, view::widget::ScrollArea& scrollArea, const model::GridXy& point);
//        void clusterDrag(model::Model& model, view::widget::ScrollArea& scrollArea, const model::GridXy& point, const model::GridXy& previousPoint);
//
//        /****** PUBLIC STATIC DATA MEMBERS  ******/
//        std::stack<Action_u_ptr> m_undoStack;
//        std::stack<Action_u_ptr> m_redoStack;
//    };
//} // namespace app
//
//#endif // BLOCKS_MODELVIEWINTERFACE_H
