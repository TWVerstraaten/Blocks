//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_ACTIONEDITBOX_H
#define BLOCKS_ACTIONEDITBOX_H

#include "../../model/Cluster.h"
#include "../Texture.h"
#include "../Widget.h"
#include "SelectionData.h"

#include <SDL2/SDL.h>
#include <limits>
#include <memory>
#include <string>
#include <vector>

class SDL_Renderer;

namespace view {
    class AssetHandler;
    namespace widget {

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
            void insertEmptyBeforeLine(size_t lineNumber);
            void handleKeyDown(const SDL_Event& event);
            void handleTextInput(const SDL_Event& event);
            void handleKeyDownControlPressed(const SDL_Event& event);
            void getSelectionFromLocalMousePoint(int xMouse, int yMouse, SelectionData::Data& data) const;
            void renderSelection(SDL_Renderer* renderer);
            void drawDashAt(const SelectionData::Data& data, SDL_Renderer* renderer) const;
            enum class HIGHLIGHT_MODE { SOFT, HARD };
            void highlightString(size_t stringIndex, SDL_Renderer* renderer, HIGHLIGHT_MODE mode) const;
            void highlightStrings(size_t firstStringIndex, size_t lastStringIndex, SDL_Renderer* renderer, HIGHLIGHT_MODE mode) const;
            void highlightStringPartial(size_t stringIndex, size_t firstCharIndex, size_t lastCharIndex, SDL_Renderer* renderer, HIGHLIGHT_MODE mode) const;
            void highlightRange(const SelectionData::Data& first, const SelectionData::Data& last, SDL_Renderer* renderer, HIGHLIGHT_MODE mode) const;
            int  widthOfString(const std::string& string) const;
            void doBackSpace();
            void doDelete();
            void doReturn(bool shiftPressed = false);
            void deleteRange(const SelectionData::Data& first, const SelectionData::Data& last);
            void splitAt(SelectionData::Data& data);
            void moveUp();
            void moveDown();
            void moveLeft(bool shiftPressed = false);
            void moveRight(bool shiftPressed = false);
            void swapIfRangeIsSelected();
            std::string selectionToString(const SelectionData::Data& first, const SelectionData::Data& last) const;
            void        copyIfSelectionNotEmpty();
            void        insertText(const std::string& text);
            void        concatAt(size_t index);

            Uint32 m_blinkTimeOffset;

            SelectionData                         m_selectionData;
            bool                                  m_needsUpdate = true;
            const AssetHandler*                   m_assetHandler;
            std::vector<std::string>              m_strings;
            std::vector<std::unique_ptr<Texture>> m_textures;
            std::vector<int>                      m_yOffsets;
            bool                                  m_isBeingEdited = false;
        };
    } // namespace widget
} // namespace view
#endif // BLOCKS_ACTIONEDITBOX_H
