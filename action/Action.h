//
// Created by pc on 27-12-20.
//

#ifndef BLOCKS_ACTION_H
#define BLOCKS_ACTION_H

#include "Action_enums.h"

#include <QUndoCommand>

namespace action {
    class Action : public QUndoCommand {

      public:
        /****** VIRTUAL FUNCTIONS  ******/
        void                              undo() override = 0;
        void                              redo() override = 0;
        [[nodiscard]] virtual ACTION_TYPE type() const    = 0;

      protected:
        bool m_blockInitial = true;
    };
} // namespace action

#endif // BLOCKS_ACTION_H
