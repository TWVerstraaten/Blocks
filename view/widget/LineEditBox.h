//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_LINEEDITBOX_H
#define BLOCKS_LINEEDITBOX_H

#include "../Texture.h"
#include "RectWidget.h"
#include "SelectionData.h"

#include <SDL.h>
#include <memory>
#include <string>
#include <vector>

class SDL_Renderer;

namespace view {
    class Assets;
    namespace widget {

        class LineEditBox : public RectWidget {
          public:

            LineEditBox(int x, int y, Uint32 w, Uint32 h, const Assets* assetHandler, std::string title);

            void render(SDL_Renderer* renderer);
            void update(SDL_Renderer* renderer);
            void keyEvent(const SDL_Event& event) override;
            void leftClickEvent(const SDL_Event& event) override;
            void mouseDragEvent(const SDL_Event& event) override;
            void loseFocus() override;
            void setHighLightedLine(size_t index);
            bool canParse() const;
            int  width() const;
            int  height() const;
            void setWidth(int width);
            void setHeight(int height);
            void setX(int x);
            void setY(int y);
            void setActive(bool active);

          private:
            enum class HIGHLIGHT_MODE { SOFT, HARD };

            void renderStrings(SDL_Renderer* renderer);
            void renderHighlightIfSelected(SDL_Renderer* renderer);
            void insertEmptyBeforeLine(size_t lineNumber);
            void handleKeyDown(const SDL_Event& event);
            void handleKeyDownControlPressed(const SDL_Event& event);
            void getSelectionFromMousePoint(SelectionData::Data& data, const SDL_Point& mousePoint) const;
            void renderSelection(SDL_Renderer* renderer);
            void drawDashAt(const SelectionData::Data& data, SDL_Renderer* renderer) const;
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
            void copyIfSelectionNotEmpty();
            void insertText(const std::string& text);
            void concatAt(size_t index);
            void potentiallyDecrementFirstCharIndex();
            void highlightString(size_t stringIndex, SDL_Renderer* renderer, HIGHLIGHT_MODE mode) const;
            void highlightStrings(size_t firstStringIndex, size_t lastStringIndex, SDL_Renderer* renderer, HIGHLIGHT_MODE mode) const;
            void highlightStringPartial(
                size_t stringIndex, size_t firstCharIndex, size_t lastCharIndex, SDL_Renderer* renderer, HIGHLIGHT_MODE mode) const;
            void                highlightRange(const SelectionData::Data& first,
                                               const SelectionData::Data& last,
                                               SDL_Renderer*              renderer,
                                               HIGHLIGHT_MODE             mode) const;
            int                 widthOfString(const std::string& string) const;
            std::string         selectionToString(const SelectionData::Data& first, const SelectionData::Data& last) const;
            SelectionData::Data moveSelectionOneDown(const SelectionData::Data& data) const;
            const SDL_Color&    getHighlightColor(HIGHLIGHT_MODE mode) const;

            bool                                  m_active          = true;
            bool                                  m_needsUpdate     = true;
            Uint32                                m_blinkTimeOffset = 0;
            SelectionData                         m_selectionData;
            const Assets*                         m_assetHandler;
            std::vector<int>                      m_yOffsets;
            std::vector<std::unique_ptr<Texture>> m_textures;
            const std::string                     m_title;
            std::unique_ptr<Texture>              m_titleTexture;

          protected:
            std::vector<std::string> m_strings;
        };
    } // namespace widget
} // namespace view
#endif // BLOCKS_LINEEDITBOX_H
