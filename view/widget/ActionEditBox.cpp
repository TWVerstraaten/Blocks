//
// Created by pc on 23-12-20.
//

#include "ActionEditBox.h"

#include "../../global/Global.h"
#include "../../model/Action.h"
#include "../../model/Cluster.h"

view::widget::ActionEditBox::ActionEditBox(
    int x, int y, Uint32 w, Uint32 h, const view::Assets* assetHandler, const model::Cluster& cluster)
    : LineEditBox(x, y, w, h, assetHandler, cluster.name() + " " + std::to_string(cluster.index())), m_clusterIndex(cluster.index()) {
    if (cluster.actions().empty()) {
        m_strings.emplace_back(" ");
    } else {
        for (const auto& action : cluster.actions()) {
            m_strings.emplace_back(model::Action::stringFromModifier(action.m_modifier) + " " +
                                   model::Action::stringFromAction(action.m_action));
        }
    }
}

void view::widget::ActionEditBox::updateClusterActions(model::Cluster& cluster) {
    cluster.clearActions();
    for (const auto& str : m_strings) {
        if (model::Action::canParse(str)) {
            if (global::trimWhiteSpace(str).empty()) {
                continue;
            }
            cluster.addAction(model::Action::fromString(str));
        }
    }
}

size_t view::widget::ActionEditBox::clusterIndex() const {
    return m_clusterIndex;
}
