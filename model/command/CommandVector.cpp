//
// Created by pc on 30-12-20.
//

#include "CommandVector.h"

#include "../../global/defines.h"
#include "../../global/overloaded.h"
#include "CommandParser.h"

#include <algorithm>
#include <cassert>

model::CommandVector::CommandVector(const std::vector<std::string>& strings) {
    set(strings);
}

bool model::CommandVector::wellFormed() const {
    return std::all_of(__CIT(m_commands), [](const Command& command) {
        return std::visit(overloaded{[](Command_Error e) { return false; }, [](auto) { return true; }}, command);
    });
}

model::Command model::CommandVector::currentCommand() const {
    if (m_commands.empty()) {
        return Command_Error{};
    }
    assert(m_commandIndex < m_commands.size());
    return m_commands[m_commandIndex];
}

bool model::CommandVector::isEmpty() const {
    return m_commands.empty();
}

void model::CommandVector::increment() {
    if (isEmpty()) {
        return;
    }
    if (std::holds_alternative<Command_Jump>(m_commands.at(m_commandIndex))) {
        increment(std::get<Command_Jump>(m_commands.at(m_commandIndex)));
    } else if (std::holds_alternative<Command_RepeatWrapper>(m_commands.at(m_commandIndex))) {
        increment(std::get<Command_RepeatWrapper>(m_commands.at(m_commandIndex)));
    } else if (std::holds_alternative<Command_Simple>(m_commands.at(m_commandIndex))) {
        increment(std::get<Command_Simple>(m_commands.at(m_commandIndex)));
    } else if (std::holds_alternative<Command_Modified>(m_commands.at(m_commandIndex))) {
        increment(Command_Simple{std::get<Command_Modified>(m_commands.at(m_commandIndex)).type});
    }
    skipOverLabels();
}

void model::CommandVector::set(const std::vector<std::string>& strings) {
    m_strings.clear();
    m_commands.clear();
    for (const auto& string : strings) {
        if (not CommandParser::isCommentOrEmpty(string)) {
            m_commands.emplace_back(CommandParser::parseString(string));
        }
    }
    assert(wellFormed());
    if (m_commands.empty()) {
        return;
    }
    m_repeatCount =
        std::visit(overloaded{[](Command_RepeatWrapper c) { return c.repeatCount - 1; }, [](auto c) { return 0; }}, m_commands[m_commandIndex]);
    std::transform(__CIT(strings), std::back_inserter(m_strings), [](const std::string& str) { return CommandParser::format(str); });
}

void model::CommandVector::clear() {
    m_commandIndex = 0;
    m_repeatCount  = 0;
    m_commands.clear();
}

size_t model::CommandVector::commandIndex() const {
    return m_commandIndex;
}

const std::vector<model::Command>& model::CommandVector::commands() const {
    return m_commands;
}

model::COMMAND_TYPE model::CommandVector::currentType() const {
    return getType(currentCommand());
}

model::COMMAND_MODIFIER model::CommandVector::currentModifier() const {
    return getModifier(currentCommand());
}

model::COMMAND_TYPE model::CommandVector::getType(const model::Command& c) {
    return std::visit(overloaded{[](const model::Command_RepeatWrapper& e) { return getType(toCommand(e)); },
                                 [](const model::Command_Simple e) { return e.type; },
                                 [](const model::Command_Modified e) { return e.type; },
                                 [](const auto& e) { return model::COMMAND_TYPE::NONE; }},
                      c);
}

model::COMMAND_MODIFIER model::CommandVector::getModifier(const model::Command& c) {
    return std::visit(overloaded{[](const model::Command_RepeatWrapper& e) { return getModifier(toCommand(e)); },
                                 [](const model::Command_Modified e) { return e.modifier; },
                                 [](const auto& e) { return model::COMMAND_MODIFIER::NONE; }},
                      c);
}

size_t model::CommandVector::repeatCount() const {
    return m_repeatCount;
}

size_t model::CommandVector::size() const {
    return m_commands.size();
}

const std::vector<std::string>& model::CommandVector::strings() const {
    return m_strings;
}

bool model::CommandVector::currentIsRepeat() const {
    return (std::holds_alternative<Command_RepeatWrapper>(m_commands.at(m_commandIndex)));
}

std::string model::CommandVector::toString() const {
    std::string result;
    for (const auto& c : m_commands) {
        result += CommandParser::toString(c) + "\n";
    }
    return result;
}

void model::CommandVector::skipOverLabels() {
    while (std::holds_alternative<Command_Label>(m_commands.at(m_commandIndex))) {
        ++m_commandIndex;
        m_commandIndex %= m_commands.size();
        m_repeatCount = getCurrentRepeatCount();
    }
}

size_t model::CommandVector::getCurrentRepeatCount() const {
    return std::visit(overloaded{[](const Command_RepeatWrapper& c) { return c.repeatCount - 1; }, [](auto) { return 0; }},
                      m_commands.at(m_commandIndex));
}

void model::CommandVector::increment(const model::Command_Jump& j) {
    const auto& jumpLabel = j.label;
    const auto  it =
        std::find_if(__CIT(m_commands),
                     __FUNC(command, std::visit(overloaded{[&](const Command_Label& l) { return l.label == jumpLabel; }, __FUNC(, false)}, command)));
    assert(it != m_commands.end());
    m_commandIndex = std::distance(m_commands.cbegin(), it);
    m_repeatCount  = getCurrentRepeatCount();
}

void model::CommandVector::increment(const model::Command_Simple& j) {
    ++m_commandIndex;
    m_commandIndex %= m_commands.size();
    m_repeatCount = getCurrentRepeatCount();
}

void model::CommandVector::increment(const model::Command_RepeatWrapper& j) {
    if (m_repeatCount == 0) {
        ++m_commandIndex;
        m_commandIndex %= m_commands.size();
        m_repeatCount = getCurrentRepeatCount();
    } else {
        --m_repeatCount;
    }
}
