//
// Created by pc on 27-12-20.
//

#ifndef BLOCKS_ACTION_H
#define BLOCKS_ACTION_H

#include "Action_enums.h"

namespace app {
    class ApplicationEdit;
}

namespace action {
    class Action {

      public:
        /****** VIRTUAL FUNCTIONS  ******/
        virtual void                      undoAction(app::ApplicationEdit& applicationEdit) = 0;
        virtual void                      redoAction(app::ApplicationEdit& applicationEdit) = 0;
        virtual void                      undoAction(){};
        virtual void                      redoAction(){};
        [[nodiscard]] virtual ACTION_TYPE type() const = 0;
    };
} // namespace action

#endif // BLOCKS_ACTION_H
