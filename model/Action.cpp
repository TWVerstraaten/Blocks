//
// Created by pc on 16-12-20.
//

#include "Action.h"

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

const std::map<std::string, model::Action::VALUE> model::Action::m_actionMap = {{"RHT", model::Action::VALUE::MOVE_RIGHT},
                                                                                {"LFT", model::Action::VALUE::MOVE_LEFT},
                                                                                {"FWD", model::Action::VALUE::MOVE_UP},
                                                                                {"BCK", model::Action::VALUE::MOVE_DOWN}};

const std::map<std::string, model::Action::MODIFIER> model::Action::m_modifierMap = {
    {"+", model::Action::MODIFIER::IGNORE}, {"-", model::Action::MODIFIER::SKIP}, {".", model::Action::MODIFIER::NONE}};

model::Action::MODIFIER model::Action::modifierFromString(const std::string& string) {
    const auto trimmed = trimWhiteSpace(string);
    assert(m_modifierMap.find(trimmed) != m_modifierMap.end());
    return m_modifierMap.at(trimmed);
}

model::Action::VALUE model::Action::actionFromString(const std::string& string) {
    const auto trimmed = trimWhiteSpace(string);
    assert(m_actionMap.find(trimmed) != m_actionMap.end());
    return m_actionMap.at(trimmed);
}

std::string model::Action::stringFromModifier(model::Action::MODIFIER modifier) {
    for (const auto& it : m_modifierMap) {
        if (it.second == modifier) {
            return it.first;
        }
    }
    return "ERROR";
}

std::string model::Action::stringFromAction(model::Action::VALUE modifier) {
    for (const auto& it : m_actionMap) {
        if (it.second == modifier) {
            return it.first;
        }
    }
    return "ERROR";
}

model::Action model::Action::fromString(const std::string& string) {
    const auto trimmed = trimWhiteSpace(string);
    assert(canParse(trimmed));
    const auto str1 = trimmed.substr(0, 1);
    const auto str2 = trimmed.substr(1);
    return {actionFromString(str2), modifierFromString(str1)};
}

bool model::Action::canParse(const std::string& string) {
    const auto trimmed = trimWhiteSpace(string);
    if (trimmed.empty()) {
        return false;
    }
    const auto str1 = trimWhiteSpace(trimmed.substr(0, 1));
    const auto str2 = trimWhiteSpace(trimmed.substr(1));
    return (m_modifierMap.find(str1) != m_modifierMap.end()) && (m_actionMap.find(str2) != m_actionMap.end());
}
