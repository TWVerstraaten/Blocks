//
// Created by pc on 23-12-20.
//

#ifndef BLOCKS_ACTIONEDITBOX_H
#define BLOCKS_ACTIONEDITBOX_H

#include "LineEditBox.h"

namespace model {
    class Cluster;
}

namespace view {
    namespace widget {
        class ActionEditBox : public LineEditBox {

          public:
            ActionEditBox(int x, int y, Uint32 w, Uint32 h, const Assets* assetHandler, const model::Cluster& cluster);

            void updateClusterActions(model::Cluster& cluster);
        };

    } // namespace widget
} // namespace view

#endif // BLOCKS_ACTIONEDITBOX_H
