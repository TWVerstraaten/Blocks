//
// Created by pc on 23-12-20.
//

#ifndef BLOCKS_COMMANDEDITBOX_H
#define BLOCKS_COMMANDEDITBOX_H

#include "LineEditBox.h"

namespace model {
    class Cluster;
    class CommandVector;
} // namespace m_model

namespace view::widget {

    class ScrollArea;

    class CommandEditBox : public LineEditBox {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        CommandEditBox(int x, int y, Uint32 w, Uint32 h, const model::Cluster& cluster);
        CommandEditBox(const CommandEditBox& other);
        CommandEditBox(CommandEditBox&& other) = default;
        CommandEditBox& operator               =(const CommandEditBox& other);

        /****** PUBLIC VIRTUAL FUNCTIONS  ******/
        void loseFocus() override;
        void setNeedsUpdate() override;
        void update(SDL_Renderer* renderer) override;

        /****** CONST GETTERS  ******/
        bool   clusterShouldBeUpdated() const;
        size_t index() const;

        /****** CONST FUNCTIONS  ******/
        void updateClusterCommands(model::Cluster& cluster) const;
        bool canParse() const;

        /****** NON CONST FUNCTIONS  ******/
        void setStrings(const std::vector<std::string>& strings);
        void createTitleTexture(SDL_Renderer* renderer);
        void createStringTextures(SDL_Renderer* renderer);
        void updateComments(const model::CommandVector& commandVector);
        void updateSelected(const model::CommandVector& commandVector);

      private:
        /****** PRIVATE CONST FUNCTIONS  ******/
        size_t findNthNonTransparent(size_t n)const;

        /****** DATA MEMBERS  ******/
        mutable bool m_clusterShouldBeUpdated = true;
        size_t       m_index;
    };

} // namespace view::widget

#endif // BLOCKS_COMMANDEDITBOX_H
