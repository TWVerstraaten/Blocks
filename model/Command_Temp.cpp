//
// Created by pc on 16-12-20.
//

#include "Command_Temp.h"

#include "../global/fns.h"

model::COMMAND_MODIFIER model::Command_Temp::modifierFromString(const std::string& string) {
    const auto trimmed = fns::trimWhiteSpace(string);
    assert(s_modifierMap.find(trimmed) != s_modifierMap.end());
    return s_modifierMap.at(trimmed);
}

model::COMMAND_TYPE model::Command_Temp::typeFromString(const std::string& string) {
    const auto trimmed = fns::trimWhiteSpace(string);
    assert(s_typeMap.find(trimmed) != s_typeMap.end());
    return s_typeMap.at(trimmed);
}

std::string model::Command_Temp::stringFromModifier(model::COMMAND_MODIFIER modifier) {
    for (const auto& [name, mod] : s_modifierMap) {
        if (mod == modifier) {
            return name;
        }
    }
    return "ERROR";
}

std::string model::Command_Temp::stringFromType(model::COMMAND_TYPE type) {
    for (const auto& [name, tp] : s_typeMap) {
        if (tp == type) {
            return name;
        }
    }
    return "ERROR";
}

model::Command_Temp model::Command_Temp::fromString(const std::string& string) {
    const auto trimmed = fns::trimWhiteSpace(string);
    assert(canParse(trimmed));
    const auto str1 = trimmed.substr(0, 1);
    const auto str2 = trimmed.substr(1);
    return {typeFromString(str2), modifierFromString(str1)};
}

bool model::Command_Temp::canParse(const std::string& string) {
    const auto trimmed = fns::trimWhiteSpace(string);
    if (trimmed.empty()) {
        return true;
    }
    const auto str1 = fns::trimWhiteSpace(trimmed.substr(0, 1));
    const auto str2 = fns::trimWhiteSpace(trimmed.substr(1));
    return (s_modifierMap.find(str1) != s_modifierMap.end()) && (s_typeMap.find(str2) != s_typeMap.end());
}

model::Command_Temp::Command_Temp(model::COMMAND_TYPE type, model::COMMAND_MODIFIER modifier) : m_type(type), m_modifier(modifier) {
}

std::string model::Command_Temp::formatCommandString(const std::string& string) {
    assert(canParse(string));
    auto trimmed = fns::trimWhiteSpace(string);
    if (trimmed.empty()) {
        return trimmed;
    }
    const auto str1 = fns::trimWhiteSpace(trimmed.substr(0, 1));
    const auto str2 = fns::trimWhiteSpace(trimmed.substr(1));
    return str1 + " " + str2;
}

bool model::Command_Temp::isFormatted(const std::string& string) {
    assert(canParse(string));
    return string == formatCommandString(string);
}
