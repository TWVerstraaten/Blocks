//
// Created by pc on 23-12-20.
//

#include "ActionEditBox.h"

#include "../../global/cst.h"
#include "../../global/fns.h"
#include "../../model/Cluster.h"
#include "../../model/Command.h"
#include "../Assets.h"

#include <algorithm>

view::widget::ActionEditBox::ActionEditBox(
    int x, int y, Uint32 w, Uint32 h, const view::Assets* assetHandler, const model::Cluster& cluster)
    : LineEditBox(x, y, w, h, assetHandler, cluster.name() + " " + std::to_string(cluster.index())), m_clusterIndex(cluster.index()) {
    if (cluster.actions().empty()) {
        m_strings.emplace_back("");
    } else {
        for (const auto& action : cluster.actions()) {
            m_strings.emplace_back(model::Command::stringFromModifier(action.m_modifier) + " " +
                                   model::Command::stringFromAction(action.m_value));
        }
    }
}

view::widget::ActionEditBox::ActionEditBox(const view::widget::ActionEditBox& other)
    : LineEditBox(other.m_rect.x, other.m_rect.y, other.m_rect.w, other.m_rect.h, other.m_assets, other.m_title),
      m_clusterIndex(other.m_clusterIndex) {
    m_strings = other.m_strings;
}

void view::widget::ActionEditBox::updateClusterActions(model::Cluster& cluster) {
    cluster.clearActions();
    for (const auto& str : m_strings) {
        if (model::Command::canParse(str)) {
            if (fns::trimWhiteSpace(str).empty()) {
                continue;
            }
            cluster.addAction(model::Command::fromString(str));
        }
    }
}

size_t view::widget::ActionEditBox::clusterIndex() const {
    return m_clusterIndex;
}

void view::widget::ActionEditBox::update(SDL_Renderer* renderer) {
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

bool view::widget::ActionEditBox::canParse() const {
    return std::all_of(m_strings.begin(), m_strings.end(), &model::Command::canParse);
}

void view::widget::ActionEditBox::loseFocus() {
    for (auto& str : m_strings) {
        if (model::Command::canParse(str)) {
            str = model::Command::formatActionString(str);
        }
    }

    LineEditBox::loseFocus();
}
