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

namespace view {
    class Assets;
    class ScreenXY;

    namespace widget {

        class LineEditBox : public RectWidget {

            /****** PRIVATE STATICS  ******/
            enum class HIGHLIGHT_MODE { SOFT, HARD };

          public:
            /****** CONSTRUCTORS / DESTRUCTORS  ******/
            LineEditBox(int x, int y, Uint32 w, Uint32 h, const Assets* assetHandler, std::string title);

            /****** PUBLIC VIRTUAL FUNCTIONS  ******/
            virtual void setNeedsUpdate();
            void         update(SDL_Renderer* renderer) override;
            void         render(SDL_Renderer* renderer) override;
            void         keyEvent(const SDL_Event& event) override;
            void         leftClickEvent(const SDL_Event& event) override;
            void         mouseDragEvent(const SDL_Event& event) override;
            void         loseFocus() override;

            /****** CONST GETTERS  ******/
            [[nodiscard]] int                             width() const;
            [[nodiscard]] const std::vector<std::string>& strings() const;

            void setActive(bool active);

          private:
            /****** PRIVATE CONST FUNCTIONS  ******/
            void renderContents(SDL_Renderer* renderer) const;
            void renderStrings(SDL_Renderer* renderer) const;
            void renderComments(SDL_Renderer* renderer) const;
            void renderLineNumbers(SDL_Renderer* renderer) const;
            void getSelectionFromMousePoint(SelectionData::Data& data, const view::ScreenXY& mousePoint) const;
            void drawDashAt(const SelectionData::Data& data, SDL_Renderer* renderer) const;
            void highlightString(size_t stringIndex, SDL_Renderer* renderer, HIGHLIGHT_MODE mode) const;
            void highlightStrings(size_t firstStringIndex, size_t lastStringIndex, SDL_Renderer* renderer, HIGHLIGHT_MODE mode) const;
            void highlightStringPartial(
                size_t stringIndex, size_t firstCharIndex, size_t lastCharIndex, SDL_Renderer* renderer, HIGHLIGHT_MODE mode) const;
            void renderSelection(SDL_Renderer* renderer) const;
            void renderHighlightIfSelected(SDL_Renderer* renderer) const;
            void highlightRange(const SelectionData::Data& first, const SelectionData::Data& last, SDL_Renderer* renderer, HIGHLIGHT_MODE mode) const;
            [[nodiscard]] int               widthOfString(const std::string& string) const;
            [[nodiscard]] const SDL_Color&  getHighlightColor(HIGHLIGHT_MODE mode) const;
            [[nodiscard]] const std::string selectionToString(const SelectionData::Data& first, const SelectionData::Data& last) const;
            [[nodiscard]] std::string prefixOfString(const SelectionData::Data& data) const;
            [[nodiscard]] std::string suffixOfString(const SelectionData::Data& data) const;

            /****** PRIVATE NON CONST FUNCTIONS  ******/
            void moveFirstSelectionOneDown();
            void insertEmptyBeforeLine(size_t lineNumber);
            void handleKeyDown(const SDL_Event& event);
            void handleKeyDownControlPressed(const SDL_Event& event);
            void doBackSpace();
            void doDelete();
            void doReturn(bool shiftPressed = false);
            void deleteRange(const SelectionData::Data& first, const SelectionData::Data& last);
            void splitAtFirstSelectionData();
            void moveUp();
            void moveDown();
            void moveLeft(bool shiftPressed = false);
            void moveRight(bool shiftPressed = false);
            void swapIfRangeIsSelected();
            void copyIfSelectionNotEmpty();
            void insertText(const std::string& text);
            void concatAt(size_t index);
            void potentiallyDecrementFirstCharIndex();
            void removeCharacterAfter(const SelectionData::Data& data);
            void decrementFirstSelectionData();
            void incrementFirstSelectionData();
            void setFirstCharIndexToMaximum();
            void moveFirstSelectionDataToStartOfNextLine();
            void setFirstSelection(size_t stringIndex, size_t charIndex);

          protected:
            /****** DATA MEMBERS  ******/
            bool                                        m_needsUpdate = true;
            const Assets*                               m_assets;
            std::string                                 m_title;
            SelectionData                               m_selectionData;
            std::unique_ptr<Texture>                    m_titleTexture;
            std::vector<std::string>                    m_strings;
            std::vector<int>                            m_yOffsets;
            std::vector<std::unique_ptr<Texture>>       m_textures;
            std::vector<std::pair<size_t, std::string>> m_comments;

          private:
            bool   m_active          = true;
            Uint32 m_blinkTimeOffset = 0;
        };
    } // namespace widget
} // namespace view
#endif // BLOCKS_LINEEDITBOX_H
