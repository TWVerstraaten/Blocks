//
// Created by teunv on 1/17/2021.
//

#ifndef BLOCKS_GENERICTEXTEDITACTION_H
#define BLOCKS_GENERICTEXTEDITACTION_H

#include "Action.h"

namespace view2 {
    class TextEdit;
}

namespace action {

    class GenericTextEditAction : public Action {

      public:
        GenericTextEditAction(view2::TextEdit* mTextEdit);

        void                      undoAction(app::ApplicationEdit& applicationEdit) override;
        void                      redoAction(app::ApplicationEdit& applicationEdit) override;
        void                      undoAction() override;
        void                      redoAction() override;

        [[nodiscard]] ACTION_TYPE type() const override;

      private:
        view2::TextEdit* m_textEdit;
    };
} // namespace action
#endif // BLOCKS_GENERICTEXTEDITACTION_H
