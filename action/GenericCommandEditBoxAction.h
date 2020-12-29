//
// Created by pc on 28-12-20.
//

#ifndef BLOCKS_GENERICCOMMANDEDITBOXACTION_H
#define BLOCKS_GENERICCOMMANDEDITBOXACTION_H

#include "../view/widget/CommandEditBox.h"
#include "Action.h"

namespace action {

    class GenericCommandEditBoxAction : public Action {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        GenericCommandEditBoxAction(const view::widget::CommandEditBox& oldCommandEditBox,
                                    const view::widget::CommandEditBox& newCommandEditBox);

        /****** PUBLIC VIRTUAL FUNCTIONS  ******/
        void        undoAction(app::Application_Edit& applicationEdit) override;
        void        redoAction(app::Application_Edit& applicationEdit) override;
        ACTION_TYPE type() const override;

      private:
        /****** DATA MEMBERS  ******/
        const size_t                       m_index;
        const view::widget::CommandEditBox m_old;
        const view::widget::CommandEditBox m_new;
    };
} // namespace action

#endif // BLOCKS_GENERICCOMMANDEDITBOXACTION_H
