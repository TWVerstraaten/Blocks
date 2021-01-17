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
        void                      undoAction(app::ApplicationEdit& applicationEdit) override;
        void                      redoAction(app::ApplicationEdit& applicationEdit) override;
        void                      undoAction() override;
        void                      redoAction() override;
        [[nodiscard]] ACTION_TYPE type() const override;

        void                      add(std::unique_ptr<Action>&& action);
        bool                      isEmpty();
        bool                      single() const;
        std::unique_ptr<Action>&& first();

      private:
        std::vector<std::unique_ptr<Action>> m_actions;
    };
} // namespace action

#endif // BLOCKS_GLOBACTION_H
