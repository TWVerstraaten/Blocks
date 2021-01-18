//
// Created by teunv on 1/17/2021.
//

#ifndef BLOCKS_TEXTEDITACTION_H
#define BLOCKS_TEXTEDITACTION_H

#include "Action.h"

namespace view {
    class TextEdit;
}

namespace action {

    class TextEditAction : public Action {

      public:
        TextEditAction(view::TextEdit* textEdit);

        void undo() override;
        void redo() override;

        [[nodiscard]] ACTION_TYPE type() const override;

      private:
        view::TextEdit* m_textEdit;
    };
} // namespace action
#endif // BLOCKS_TEXTEDITACTION_H
