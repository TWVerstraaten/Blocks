//
// Created by pc on 30-12-20.
//

#include "CommandVector.h"

#include "../../global/fns.h"
#include "../../global/overloaded.h"
#include "CommandParser.h"

#include <algorithm>

model::CommandVector::CommandVector(const std::vector<std::string>& strings) {
    set(strings);
}

bool model::CommandVector::wellFormed() const {
    return std::all_of(m_commands.begin(), m_commands.end(), [](const Command& command) {
        return std::visit(overloaded{[](Command_Error e) { return false; }, [](auto e) { return true; }}, command);
    });
}

model::Command model::CommandVector::currentCommand() const {
    if (m_commands.empty()) {
        return Command_Error{};
    }
    assert(m_commandIndex < m_commands.size());
    return std::visit(overloaded{[](Command_RepeatWrapper c) { return toCommand(c); }, [](auto c) { return static_cast<Command>(c); }},
                      m_commands[m_commandIndex]);
}

bool model::CommandVector::empty() const {
    return m_commands.empty();
}

void model::CommandVector::increment() {
    if (m_repeatCount == 0) {
        ++m_commandIndex;
        m_commandIndex %= m_commands.size();
        m_repeatCount =
            std::visit(overloaded{[](Command_RepeatWrapper c) { return c.repeatCount - 1; }, [](auto c) { return 0; }}, m_commands[m_commandIndex]);
    } else {
        --m_repeatCount;
    }
}

void model::CommandVector::set(const std::vector<std::string>& strings) {
    m_commands.clear();
    for (const auto& string : strings) {
        if (not CommandParser::isCommentOrEmpty(string)) {
            m_commands.emplace_back(CommandParser::parseString(string));
        }
    }
    if (m_commands.empty()) {
        return;
    }
    m_repeatCount =
        std::visit(overloaded{[](Command_RepeatWrapper c) { return c.repeatCount - 1; }, [](auto c) { return 0; }}, m_commands[m_commandIndex]);
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
    return type(currentCommand());
}

model::COMMAND_MODIFIER model::CommandVector::currentModifier() const {
    return modifier(currentCommand());
}

model::COMMAND_TYPE model::CommandVector::type(const model::Command& c) {
    return std::visit(overloaded{[](const model::Command_RepeatWrapper& e) { return type(toCommand(e)); },
                                 [](const model::Command_Simple e) { return e.type; },
                                 [](const model::Command_Modified e) { return e.type; },
                                 [](const auto& e) { return model::COMMAND_TYPE::NONE; }},
                      c);
}

model::COMMAND_MODIFIER model::CommandVector::modifier(const model::Command& c) {
    return std::visit(overloaded{[](const model::Command_RepeatWrapper& e) { return modifier(toCommand(e)); },
                                 [](const model::Command_Modified e) { return e.modifier; },
                                 [](const auto& e) { return model::COMMAND_MODIFIER::NONE; }},
                      c);
}
