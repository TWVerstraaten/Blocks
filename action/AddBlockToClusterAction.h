//
// Created by pc on 28-12-20.
//

#ifndef BLOCKS_ADDBLOCKTOCLUSTERACTION_H
#define BLOCKS_ADDBLOCKTOCLUSTERACTION_H

#include "../model/GridXY.h"
#include "Action.h"
namespace action {
    class AddBlockToClusterAction : public Action {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        AddBlockToClusterAction(size_t clusterIndex, const model::GridXY& gridXy);

        /****** VIRTUAL FUNCTIONS  ******/
        void undoAction(app::Application_Edit& applicationEdit) override;
        void redoAction(app::Application_Edit& applicationEdit) override;
        ACTION_TYPE type() const override;

      private:
        /****** DATA MEMBERS  ******/
        size_t        m_clusterIndex;
        model::GridXY m_gridXY;
    };
} // namespace action
#endif // BLOCKS_ADDBLOCKTOCLUSTERACTION_H
