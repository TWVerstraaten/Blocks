//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_ACTIONEDITBOX_H
#define BLOCKS_ACTIONEDITBOX_H

#include "../model/Cluster.h"
#include "Texture.h"
#include "Widget.h"

#include <SDL2/SDL.h>
#include <limits>
#include <memory>
#include <string>
#include <vector>

class SDL_Renderer;

namespace view {
    class AssetHandler;

    class ActionEditBox : public Widget {
      public:
        ActionEditBox(int x, int y, int w, int h, const AssetHandler* assetHandler, const model::Cluster& cluster);
        ~ActionEditBox() = default;

        void render(SDL_Renderer* renderer);
        void init(SDL_Renderer* renderer);

        void handleKeyEvent(const SDL_Event& event) override;
        void handleMouseClickEvent(const SDL_Event& event) override;

        void loseFocus() override;

        const std::vector<std::string>& strings() const;

      private:
        void incrementHighlightIndex();
        void deleteHighlightedLine();
        void insertBeforeHighlightedLine();

        size_t                                m_stringIndex = std::numeric_limits<size_t>::max();
        size_t                                m_positionInString;
        bool                                  m_needsUpdate = true;
        const AssetHandler*                   m_assetHandler;
        std::vector<std::string>              m_strings;
        std::vector<std::unique_ptr<Texture>> m_textures;
        std::vector<size_t>                   m_verticalDelimiters;
        bool                                  m_isBeingEdited = false;
    };
} // namespace view

#endif // BLOCKS_ACTIONEDITBOX_H
