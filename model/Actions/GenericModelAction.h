//
// Created by pc on 27-12-20.
//

#ifndef BLOCKS_GENERICMODELACTION_H
#define BLOCKS_GENERICMODELACTION_H

#include "../Model.h"
#include "Action.h"

namespace model {
    namespace action {
        class GenericModelAction : public Action {
          public:
            GenericModelAction(const Model& old, const Model& aNew);

            void undoAction(Application_Edit& applicationEdit) override;
            void redoAction(Application_Edit& applicationEdit) override;

          private:
            const Model m_old;
            const Model m_new;
        };

    } // namespace action
} // namespace model

#endif // BLOCKS_GENERICMODELACTION_H
