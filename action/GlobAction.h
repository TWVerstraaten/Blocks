//
// Created by teunv on 1/17/2021.
//

#ifndef BLOCKS_GLOBACTION_H
#define BLOCKS_GLOBACTION_H

#include "Action.h"

#include <memory>
#include <vector>

namespace action {

    class GlobAction : public Action {

      public:
        void undo() override;
        void redo() override;

        void                                    add(std::unique_ptr<Action>&& action);
        [[nodiscard]] ACTION_TYPE               type() const override;
        [[nodiscard]] bool                      isEmpty() const;
        [[nodiscard]] bool                      single() const;
        [[nodiscard]] std::unique_ptr<Action>&& first();

      private:
        std::vector<std::unique_ptr<Action>> m_actions;
    };
} // namespace action

#endif // BLOCKS_GLOBACTION_H
