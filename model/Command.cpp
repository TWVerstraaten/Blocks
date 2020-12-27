//
// Created by pc on 16-12-20.
//

#include "Command.h"

#include "../global/fns.h"

#include <cassert>

const std::map<std::string, model::Command::TYPE> model::Command::m_typeMap = {{"RHT", model::Command::TYPE::MOVE_RIGHT},
                                                                               {"LFT", model::Command::TYPE::MOVE_LEFT},
                                                                               {"FWD", model::Command::TYPE::MOVE_UP},
                                                                               {"BCK", model::Command::TYPE::MOVE_DOWN},
                                                                               {"SKP", model::Command::TYPE::SKIP}};

const std::map<std::string, model::Command::MODIFIER> model::Command::m_modifierMap = {
    {"+", model::Command::MODIFIER::IGNORE}, {"-", model::Command::MODIFIER::INCREMENT}, {".", model::Command::MODIFIER::NONE}};

model::Command::MODIFIER model::Command::modifierFromString(const std::string& string) {
    const auto trimmed = fns::trimWhiteSpace(string);
    assert(m_modifierMap.find(trimmed) != m_modifierMap.end());
    return m_modifierMap.at(trimmed);
}

model::Command::TYPE model::Command::typeFromString(const std::string& string) {
    const auto trimmed = fns::trimWhiteSpace(string);
    assert(m_typeMap.find(trimmed) != m_typeMap.end());
    return m_typeMap.at(trimmed);
}

std::string model::Command::stringFromModifier(model::Command::MODIFIER modifier) {
    for (const auto& it : m_modifierMap) {
        if (it.second == modifier) {
            return it.first;
        }
    }
    return "ERROR";
}

std::string model::Command::stringFromType(model::Command::TYPE modifier) {
    for (const auto& it : m_typeMap) {
        if (it.second == modifier) {
            return it.first;
        }
    }
    return "ERROR";
}

model::Command model::Command::fromString(const std::string& string) {
    const auto trimmed = fns::trimWhiteSpace(string);
    assert(canParse(trimmed));
    const auto str1 = trimmed.substr(0, 1);
    const auto str2 = trimmed.substr(1);
    return {typeFromString(str2), modifierFromString(str1)};
}

bool model::Command::canParse(const std::string& string) {
    const auto trimmed = fns::trimWhiteSpace(string);
    if (trimmed.empty()) {
        return true;
    }
    const auto str1 = fns::trimWhiteSpace(trimmed.substr(0, 1));
    const auto str2 = fns::trimWhiteSpace(trimmed.substr(1));
    return (m_modifierMap.find(str1) != m_modifierMap.end()) && (m_typeMap.find(str2) != m_typeMap.end());
}

model::Command::Command(model::Command::TYPE type, model::Command::MODIFIER modifier) : m_type(type), m_modifier(modifier) {
}

std::string model::Command::formatCommandString(std::string& string) {
    assert(canParse(string));
    auto trimmed = fns::trimWhiteSpace(string);
    if (trimmed.empty()) {
        return trimmed;
    }
    const auto str1 = fns::trimWhiteSpace(trimmed.substr(0, 1));
    const auto str2 = fns::trimWhiteSpace(trimmed.substr(1));
    return str1 + " " + str2;
}
