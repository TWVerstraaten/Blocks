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
    m_repeatCount =
        std::visit(overloaded{[](Command_RepeatWrapper c) { return c.repeatCount - 1; }, [](auto c) { return 0; }}, m_commands[m_commandIndex]);
}
