//
// Created by pc on 23-12-20.
//

#ifndef BLOCKS_COMMANDEDITBOX_H
#define BLOCKS_COMMANDEDITBOX_H

#include "LineEditBox.h"

namespace model {
    class Cluster;
}

namespace view {
    namespace widget {
        class CommandEditBox : public LineEditBox {

          public:
            CommandEditBox(int x, int y, Uint32 w, Uint32 h, const Assets* assetHandler, const model::Cluster& cluster);
            CommandEditBox(const CommandEditBox& other);

            void   loseFocus() override;
            void   update(SDL_Renderer* renderer) override;
            void   updateClusterCommands(model::Cluster& cluster);
            size_t clusterIndex() const;
            bool   canParse() const;

          private:
            size_t m_clusterIndex;
        };

    } // namespace widget
} // namespace view

#endif // BLOCKS_COMMANDEDITBOX_H
