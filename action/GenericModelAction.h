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
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        GenericModelAction(const model::Model& oldModel, const model::Model& newModel);

        /****** VIRTUAL FUNCTIONS  ******/
        void undoAction(app::Application_Edit& applicationEdit) override;
        void redoAction(app::Application_Edit& applicationEdit) override;

      private:
        /****** DATA MEMBERS  ******/
        const model::Model m_old;
        const model::Model m_new;
    };

} // namespace action

#endif // BLOCKS_GENERICMODELACTION_H
