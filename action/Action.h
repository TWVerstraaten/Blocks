//
// Created by pc on 27-12-20.
//

#ifndef BLOCKS_ACTION_H
#define BLOCKS_ACTION_H

namespace app {
    class Application_Edit;
}

namespace action {
    class Action {
      public:
        /****** VIRTUAL FUNCTIONS  ******/
        virtual void undoAction(app::Application_Edit& applicationEdit) = 0;
        virtual void redoAction(app::Application_Edit& applicationEdit) = 0;
    };
} // namespace action

#endif // BLOCKS_ACTION_H
