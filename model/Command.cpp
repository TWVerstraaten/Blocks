//
// Created by pc on 16-12-20.
//

#include "Command.h"

#include "../global/fns.h"

#include <cassert>

const std::map<std::string, model::Command::VALUE> model::Command::m_valueMap = {{"RHT", model::Command::VALUE::MOVE_RIGHT},
                                                                                {"LFT", model::Command::VALUE::MOVE_LEFT},
                                                                                {"FWD", model::Command::VALUE::MOVE_UP},
                                                                                {"BCK", model::Command::VALUE::MOVE_DOWN},
                                                                                {"SKP", model::Command::VALUE::SKIP}};

const std::map<std::string, model::Command::MODIFIER> model::Command::m_modifierMap = {
    {"+", model::Command::MODIFIER::IGNORE}, {"-", model::Command::MODIFIER::INCREMENT}, {".", model::Command::MODIFIER::NONE}};

model::Command::MODIFIER model::Command::modifierFromString(const std::string& string) {
    const auto trimmed = fns::trimWhiteSpace(string);
    assert(m_modifierMap.find(trimmed) != m_modifierMap.end());
    return m_modifierMap.at(trimmed);
}

model::Command::VALUE model::Command::valueFromString(const std::string& string) {
    const auto trimmed = fns::trimWhiteSpace(string);
    assert(m_valueMap.find(trimmed) != m_valueMap.end());
    return m_valueMap.at(trimmed);
}

std::string model::Command::stringFromModifier(model::Command::MODIFIER modifier) {
    for (const auto& it : m_modifierMap) {
        if (it.second == modifier) {
            return it.first;
        }
    }
    return "ERROR";
}

std::string model::Command::stringFromAction(model::Command::VALUE modifier) {
    for (const auto& it : m_valueMap) {
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
    return {valueFromString(str2), modifierFromString(str1)};
}

bool model::Command::canParse(const std::string& string) {
    const auto trimmed = fns::trimWhiteSpace(string);
    if (trimmed.empty()) {
        return true;
    }
    const auto str1 = fns::trimWhiteSpace(trimmed.substr(0, 1));
    const auto str2 = fns::trimWhiteSpace(trimmed.substr(1));
    return (m_modifierMap.find(str1) != m_modifierMap.end()) && (m_valueMap.find(str2) != m_valueMap.end());
}

model::Command::Command(model::Command::VALUE value, model::Command::MODIFIER modifier) : m_value(value), m_modifier(modifier) {
}

std::string model::Command::formatActionString(std::string& string) {
    assert(canParse(string));
    auto trimmed = fns::trimWhiteSpace(string);
    if (trimmed.empty()) {
        return trimmed;
    }
    const auto str1 = fns::trimWhiteSpace(trimmed.substr(0, 1));
    const auto str2 = fns::trimWhiteSpace(trimmed.substr(1));
    return str1 + " " + str2;
}
