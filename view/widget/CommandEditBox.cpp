//
// Created by pc on 23-12-20.
//

#include "CommandEditBox.h"

#include "../../global/cst.h"
#include "../../global/fns.h"
#include "../../model/Cluster.h"
#include "../Assets.h"
#include "../color.h"

#include <algorithm>

view::widget::CommandEditBox::CommandEditBox(
    int x, int y, Uint32 w, Uint32 h, const view::Assets* assetHandler, const model::Cluster& cluster)
    : LineEditBox(x, y, w, h, assetHandler, cluster.name() + " " + std::to_string(cluster.index())),
      m_index(cluster.index()) {
    if (cluster.commands().empty()) {
        m_strings.emplace_back("");
    } else {
        for (const auto& command : cluster.commands()) {
            m_strings.emplace_back(model::Command::stringFromModifier(command.m_modifier) + " " +
                                   model::Command::stringFromType(command.m_type));
        }
    }
}

view::widget::CommandEditBox::CommandEditBox(const view::widget::CommandEditBox& other)
    : LineEditBox(other.m_rect.x, other.m_rect.y, other.m_rect.w, other.m_rect.h, other.m_assets, other.m_title),
      m_index(other.m_index) {
    m_strings       = other.m_strings;
    m_selectionData = other.m_selectionData;
}

void view::widget::CommandEditBox::updateClusterCommands(model::Cluster& cluster) const {
    cluster.clearCommands();
    for (const auto& str : m_strings) {
        if (model::Command::canParse(str)) {
            if (fns::trimWhiteSpace(str).empty()) {
                continue;
            }
            cluster.addCommand(model::Command::fromString(str));
        }
    }
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
    return std::all_of(m_strings.begin(), m_strings.end(), &model::Command::canParse);
}

void view::widget::CommandEditBox::loseFocus() {
    for (auto& str : m_strings) {
        if (model::Command::canParse(str) && (not model::Command::isFormatted(str))) {
            str           = model::Command::formatCommandString(str);
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
    m_titleTexture =
        Texture::createFromText(m_title, view::color::BLACK, renderer, m_assets->font(Assets::FONT_ENUM::MAIN)->font());
}

void view::widget::CommandEditBox::createStringTextures(SDL_Renderer* renderer) {
    m_textures.clear();
    m_yOffsets.clear();

    int yOffset = cst::LINE_EDIT_TITLE_HEIGHT;
    for (auto& str : m_strings) {
        m_yOffsets.push_back(yOffset);
        const auto text     = str.length() == 0 ? std::string(" ") : str;
        bool       canParse = model::Command::canParse(str);
        m_textures.emplace_back(Texture::createFromText(text,
                                                        canParse ? view::color::BLACK : view::color::TEXT_ERROR,
                                                        renderer,
                                                        m_assets->font(Assets::FONT_ENUM::MAIN)->font()));
        yOffset += m_textures.back()->height();
    }
    m_yOffsets.push_back(yOffset);
}
