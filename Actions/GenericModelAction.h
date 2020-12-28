//
// Created by pc on 27-12-20.
//

#ifndef BLOCKS_GENERICMODELACTION_H
#define BLOCKS_GENERICMODELACTION_H

#include "../model/Model.h"
#include "Action.h"

namespace action {
    class GenericModelAction : public Action {
      public:
        GenericModelAction(const model::Model& old, const model::Model& aNew);

        void undoAction(Application_Edit& applicationEdit) override;
        void redoAction(Application_Edit& applicationEdit) override;

      private:
        const model::Model m_old;
        const model::Model m_new;
    };

} // namespace action

#endif // BLOCKS_GENERICMODELACTION_H
