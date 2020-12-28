//
// Created by pc on 27-12-20.
//

#ifndef BLOCKS_ACTION_H
#define BLOCKS_ACTION_H

class Application_Edit;

namespace action {
    class Action {
      public:
        virtual void undoAction(Application_Edit& applicationEdit) = 0;
        virtual void redoAction(Application_Edit& applicationEdit) = 0;
    };
} // namespace action

#endif // BLOCKS_ACTION_H
