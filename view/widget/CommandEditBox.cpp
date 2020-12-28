//
// Created by pc on 23-12-20.
//

#include "CommandEditBox.h"

#include "../../global/cst.h"
#include "../../global/fns.h"
#include "../../model/Cluster.h"
#include "../Assets.h"

#include <algorithm>

view::widget::CommandEditBox::CommandEditBox(
    int x, int y, Uint32 w, Uint32 h, const view::Assets* assetHandler, const model::Cluster& cluster)
    : LineEditBox(x, y, w, h, assetHandler, cluster.name() + " " + std::to_string(cluster.index())), m_clusterIndex(cluster.index()) {
    if (cluster.commands().empty()) {
        m_strings.emplace_back("");
    } else {
        for (const auto& action : cluster.commands()) {
            m_strings.emplace_back(model::Command::stringFromModifier(action.m_modifier) + " " +
                                   model::Command::stringFromType(action.m_type));
        }
    }
}

view::widget::CommandEditBox::CommandEditBox(const view::widget::CommandEditBox& other)
    : LineEditBox(other.m_rect.x, other.m_rect.y, other.m_rect.w, other.m_rect.h, other.m_assets, other.m_title),
      m_clusterIndex(other.m_clusterIndex) {
    m_strings = other.m_strings;
}

void view::widget::CommandEditBox::updateClusterCommands(model::Cluster& cluster) {
    cluster.clearCommands();
    for (const auto& str : m_strings) {
        if (model::Command::canParse(str)) {
            if (fns::trimWhiteSpace(str).empty()) {
                continue;
            }
            cluster.addCommand(model::Command::fromString(str));
        }
    }
}

size_t view::widget::CommandEditBox::clusterIndex() const {
    return m_clusterIndex;
}

void view::widget::CommandEditBox::update(SDL_Renderer* renderer) {
    if (!m_needsUpdate) {
        return;
    }
    LineEditBox::update(renderer);

    int yOffset = cst::LINE_EDIT_TITLE_HEIGHT;
    for (auto& str : m_strings) {
        m_yOffsets.push_back(yOffset);
        const auto text     = str.length() == 0 ? std::string(" ") : str;
        bool       canParse = model::Command::canParse(str);
        m_textures.emplace_back(Texture::createFromText(
            text, canParse ? cst::color::BLACK : cst::color::TEXT_ERROR, renderer, m_assets->font(Assets::FONT_ENUM::MAIN)->font()));
        yOffset += m_textures.back()->height();
    }
    m_yOffsets.push_back(yOffset);
    m_rect.h      = yOffset;
    m_needsUpdate = false;
}

bool view::widget::CommandEditBox::canParse() const {
    return std::all_of(m_strings.begin(), m_strings.end(), &model::Command::canParse);
}

void view::widget::CommandEditBox::loseFocus() {
    for (auto& str : m_strings) {
        if (model::Command::canParse(str)) {
            str = model::Command::formatCommandString(str);
        }
    }

    LineEditBox::loseFocus();
}
