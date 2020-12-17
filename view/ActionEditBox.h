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
        void handleMouseClickEvent(const SDL_Event& event, bool leftClicked) override;
        void handleMouseMoveEvent(const SDL_Point& mousePointInWorld, bool leftClicked);

        void loseFocus() override;

        const std::vector<std::string>& strings() const;

      private:
        void incrementHighlightIndex();
        void deleteHighlightedLine();
        void insertBeforeHighlightedLine();
        void handleKeyDown(const SDL_Event& event);
        void handleKeyDownControlPressed(const SDL_Event& event);
        void getSelectionFromLocalMousePoint(const int xMouse, const int yMouse, size_t& stringIndex,
                                             size_t& charIndex) const;
        void renderSelection(SDL_Renderer* renderer);
        void drawDashAt(size_t stringIndex, size_t charIndex, SDL_Renderer* renderer) const;
        void highlightString(size_t stringIndex, SDL_Renderer* renderer) const;
        void highlightStringPartial(size_t stringIndex, size_t firstChar, size_t lastChar,
                                    SDL_Renderer* renderer) const;
        void highlightRange(size_t firstStringIndex, size_t firstCharIndex, size_t secondStringIndex,
                            size_t secondCharIndex, SDL_Renderer* renderer) const;

        struct SelectionData {
            void reset() {
                m_stringIndex       = std::numeric_limits<size_t>::max();
                m_charIndex         = std::numeric_limits<size_t>::max();
                m_secondStringIndex = std::numeric_limits<size_t>::max();
                m_secondCharIndex   = std::numeric_limits<size_t>::max();
                m_mode              = MODE::SINGLE;
            }

            void fix() {
                if (m_stringIndex == m_secondStringIndex && m_charIndex == m_secondCharIndex) {
                    m_mode = MODE::SINGLE;
                }
            }

            bool empty() const {
                return m_stringIndex == std::numeric_limits<size_t>::max();
            }

            size_t m_stringIndex = std::numeric_limits<size_t>::max();
            size_t m_charIndex   = std::numeric_limits<size_t>::max();

            size_t m_secondStringIndex = std::numeric_limits<size_t>::max();
            size_t m_secondCharIndex   = std::numeric_limits<size_t>::max();

            enum class MODE { SINGLE, DOUBLE };
            MODE m_mode = MODE::SINGLE;
        };

        SelectionData                         m_selectionData;
        bool                                  m_needsUpdate = true;
        const AssetHandler*                   m_assetHandler;
        std::vector<std::string>              m_strings;
        std::vector<std::unique_ptr<Texture>> m_textures;
        std::vector<size_t>                   m_verticalDelimiters;
        bool                                  m_isBeingEdited = false;
    };
} // namespace view

#endif // BLOCKS_ACTIONEDITBOX_H
