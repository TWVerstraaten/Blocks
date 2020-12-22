//
// Created by pc on 16-12-20.
//

#include "ClusterAction.h"

#include <cassert>

static std::string trimWhiteSpace(const std::string& string) {
    const auto startIndex = string.find_first_not_of(" \t");
    if (startIndex == std::string::npos) {
        return "";
    } else {
        const auto endIndex = string.find_last_not_of(" \t");
        return string.substr(startIndex, endIndex - startIndex + 1);
    }
}

const std::map<std::string, model::ClusterAction::ACTION> model::ClusterAction::m_actionMap = {
    {"RHT", model::ClusterAction::ACTION::MOVE_RIGHT},
    {"LFT", model::ClusterAction::ACTION::MOVE_LEFT},
    {"FWD", model::ClusterAction::ACTION::MOVE_UP},
    {"BCK", model::ClusterAction::ACTION::MOVE_DOWN}};

const std::map<std::string, model::ClusterAction::MODIFIER> model::ClusterAction::m_modifierMap = {
    {"+", model::ClusterAction::MODIFIER::IGNORE},
    {"-", model::ClusterAction::MODIFIER::SKIP},
    {".", model::ClusterAction::MODIFIER::NONE}};

model::ClusterAction::MODIFIER model::ClusterAction::modifierFromString(const std::string& string) {
    const auto trimmed = trimWhiteSpace(string);
    assert(m_modifierMap.find(trimmed) != m_modifierMap.end());
    return m_modifierMap.at(trimmed);
}

model::ClusterAction::ACTION model::ClusterAction::actionFromString(const std::string& string) {
    const auto trimmed = trimWhiteSpace(string);
    assert(m_actionMap.find(trimmed) != m_actionMap.end());
    return m_actionMap.at(trimmed);
}

std::string model::ClusterAction::stringFromModifier(model::ClusterAction::MODIFIER modifier) {
    for (const auto& it : m_modifierMap) {
        if (it.second == modifier) {
            return it.first;
        }
    }
    return "ERROR";
}

std::string model::ClusterAction::stringFromAction(model::ClusterAction::ACTION modifier) {
    for (const auto& it : m_actionMap) {
        if (it.second == modifier) {
            return it.first;
        }
    }
    return "ERROR";
}

model::ClusterAction model::ClusterAction::fromString(const std::string& string) {
    const auto trimmed = trimWhiteSpace(string);
    assert(canParse(trimmed));
    const auto str1 = trimmed.substr(0, 1);
    const auto str2 = trimmed.substr(1);
    return {actionFromString(str2), modifierFromString(str1)};
}

bool model::ClusterAction::canParse(const std::string& string) {
    const auto trimmed = trimWhiteSpace(string);
    if (trimmed.empty()) {
        return false;
    }
    const auto str1 = trimWhiteSpace(trimmed.substr(0, 1));
    const auto str2 = trimWhiteSpace(trimmed.substr(1));
    return (m_modifierMap.find(str1) != m_modifierMap.end()) && (m_actionMap.find(str2) != m_actionMap.end());
}
