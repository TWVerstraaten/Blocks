//
// Created by pc on 23-12-20.
//

#include "CommandEditBox.h"

#include "../../global/defines.h"
#include "../../model/Cluster.h"
#include "../../model/command/CommandParser.h"
#include "../Assets.h"
#include "LineEditBox_constants.h"

#include <cassert>

view::widget::CommandEditBox::CommandEditBox(int x, int y, Uint32 w, Uint32 h, const view::Assets* assetHandler, const model::Cluster& cluster)
    : LineEditBox(x, y, w, h, assetHandler, cluster.name() + " " + std::to_string(cluster.index())), m_index(cluster.index()) {
    m_strings = cluster.commandVector().strings();
    if (m_strings.empty()) {
        m_strings = {""};
    }
}

view::widget::CommandEditBox::CommandEditBox(const view::widget::CommandEditBox& other)
    : LineEditBox(other.m_rect.x, other.m_rect.y, other.m_rect.w, other.m_rect.h, other.m_assets, other.m_title), m_index(other.m_index) {
    m_strings       = other.m_strings;
    m_selectionData = other.m_selectionData;
}

void view::widget::CommandEditBox::updateClusterCommands(model::Cluster& cluster) const {
    cluster.clearCommands();

    for (const auto& str : m_strings) {
        if (not model::CommandParser::canParse(str)) {
            return;
        }
    }
    cluster.commandVector().set(m_strings);
    m_clusterShouldBeUpdated = false;
}

size_t view::widget::CommandEditBox::index() const {
    return m_index;
}

void view::widget::CommandEditBox::update(SDL_Renderer* renderer) {
    if (!m_needsUpdate) {
        return;
    }
    if (not m_titleTexture) {
        createTitleTexture(renderer);
    }

    createStringTextures(renderer);
    m_rect.h      = m_yOffsets.back();
    m_needsUpdate = false;
}

bool view::widget::CommandEditBox::canParse() const {
    return std::all_of(__CIT(m_strings), &model::CommandParser::canParse);
}

void view::widget::CommandEditBox::loseFocus() {
    for (auto& str : m_strings) {
        if (model::CommandParser::canParse(str) && (not model::CommandParser::isFormatted(str))) {
            str           = model::CommandParser::format(str);
            m_needsUpdate = true;
        }
    }
    LineEditBox::loseFocus();
}

void view::widget::CommandEditBox::setNeedsUpdate() {
    LineEditBox::setNeedsUpdate();
    m_clusterShouldBeUpdated = true;
}

bool view::widget::CommandEditBox::clusterShouldBeUpdated() const {
    return m_clusterShouldBeUpdated;
}

void view::widget::CommandEditBox::setStrings(const std::vector<std::string>& strings) {
    m_strings = strings;
    setNeedsUpdate();
}

view::widget::CommandEditBox& view::widget::CommandEditBox::operator=(const view::widget::CommandEditBox& other) {
    m_selectionData = other.m_selectionData;
    m_strings       = other.m_strings;
    setNeedsUpdate();
    return *this;
}

void view::widget::CommandEditBox::createTitleTexture(SDL_Renderer* renderer) {
    m_titleTexture = Texture::createFromText(m_title, view::color::BLACK, renderer, m_assets->font(FONT_ENUM::MAIN)->font());
}

void view::widget::CommandEditBox::createStringTextures(SDL_Renderer* renderer) {
    m_textures.clear();
    m_yOffsets.clear();

    int yOffset = LINE_EDIT_TITLE_HEIGHT;
    for (auto& str : m_strings) {
        m_yOffsets.push_back(yOffset);
        switch (model::CommandParser::stringType(str)) {
            case model::CommandParser::STRING_TYPE::EMPTY:
                m_textures.emplace_back(Texture::createFromText(" ", view::color::DARK_GREY, renderer, m_assets->font(FONT_ENUM::MAIN)->font()));
                break;
            case model::CommandParser::STRING_TYPE::COMMENT:
                m_textures.emplace_back(Texture::createFromText(str, view::color::DARK_GREY, renderer, m_assets->font(FONT_ENUM::MAIN)->font()));
                break;
            case model::CommandParser::STRING_TYPE::ACTION:
                m_textures.emplace_back(Texture::createFromText(str, view::color::BLACK, renderer, m_assets->font(FONT_ENUM::MAIN)->font()));
                break;
            case model::CommandParser::STRING_TYPE::ERROR:
                m_textures.emplace_back(Texture::createFromText(str, view::color::TEXT_ERROR, renderer, m_assets->font(FONT_ENUM::MAIN)->font()));
                break;
        }
        yOffset += m_textures.back()->height();
    }
    m_yOffsets.push_back(yOffset);
}

void view::widget::CommandEditBox::updateComments(const model::CommandVector& commandVector) {
    if (commandVector.isEmpty()) {
        return;
    }
    m_comments.clear();
    if (commandVector.currentIsRepeat() && commandVector.repeatCount() != 0) {
        m_comments.emplace_back(
            std::make_pair(findNthNonTransparent(commandVector.commandIndex()), " #" + std::to_string(commandVector.repeatCount())));
    }
}

void view::widget::CommandEditBox::updateSelected(const model::CommandVector& commandVector) {
    if (not commandVector.isEmpty()) {
        m_selectionData.m_first.m_stringIndex = findNthNonTransparent(commandVector.commandIndex());
    }
}

size_t view::widget::CommandEditBox::findNthNonTransparent(size_t n) const {
    assert(std::count_if(__CIT(m_strings), __FUNC(str, not model::CommandParser::isCommentOrEmpty(str))) > n);
    for (size_t i = 0; i != m_strings.size(); ++i) {
        if (not model::CommandParser::isCommentOrEmpty(m_strings.at(i))) {
            if (n == 0) {
                return i;
            }
            --n;
        }
    }
    assert(false);
    return std::numeric_limits<size_t>::max();
}
