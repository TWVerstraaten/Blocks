//
// Created by pc on 30-12-20.
//

#include "CommandVector.h"

#include "../../misc/defines.h"
#include "../../misc/overloaded.h"
#include "CommandParser.h"

#include <algorithm>
#include <cassert>

namespace model {

    CommandVector::CommandVector(const std::vector<std::string>& strings) {
        set(strings);
    }

    bool CommandVector::wellFormed() const {
        return std::all_of(D_CIT(m_commands), [](const Command& command) { return not std::holds_alternative<Command_Error>(command); });
    }

    Command CommandVector::currentCommand() const {
        if (m_commands.empty()) {
            return Command_Error{};
        }
        assert(m_commandIndex < m_commands.size());
        return m_commands[m_commandIndex];
    }

    bool CommandVector::isEmpty() const {
        return m_commands.empty();
    }

    void CommandVector::increment() {
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

    void CommandVector::set(const std::vector<std::string>& strings) {
        m_strings = strings;
        m_commands.clear();
        for (const auto& string : m_strings) {
            if (not CommandParser::isCommentOrEmpty(string)) {
                m_commands.emplace_back(CommandParser::parseString(string));
            }
        }
        if (m_commands.empty()) {
            return;
        }
        m_repeatCount =
            std::visit(overloaded{[](Command_RepeatWrapper c) { return c.repeatCount - 1; }, [](auto) { return 0; }}, m_commands[m_commandIndex]);
    }

    void CommandVector::clear() {
        m_commandIndex = 0;
        m_repeatCount  = 0;
        m_commands.clear();
    }

    size_t CommandVector::commandIndex() const {
        return m_commandIndex;
    }

    const std::vector<Command>& CommandVector::commands() const {
        return m_commands;
    }

    COMMAND_TYPE CommandVector::currentType() const {
        return getType(currentCommand());
    }

    COMMAND_MODIFIER CommandVector::currentModifier() const {
        return getModifier(currentCommand());
    }

    COMMAND_TYPE CommandVector::getType(const Command& c) {
        return std::visit(overloaded{[](const Command_RepeatWrapper& e) { return getType(toCommand(e)); },
                                     [](const Command_Simple e) { return e.type; },
                                     [](const Command_Modified e) { return e.type; },
                                     [](const auto&) { return COMMAND_TYPE::NONE; }},
                          c);
    }

    COMMAND_MODIFIER CommandVector::getModifier(const Command& c) {
        return std::visit(overloaded{[](const Command_RepeatWrapper& e) { return getModifier(toCommand(e)); },
                                     [](const Command_Modified e) { return e.modifier; },
                                     [](auto) { return COMMAND_MODIFIER::NONE; }},
                          c);
    }

    size_t CommandVector::repeatCount() const {
        return m_repeatCount;
    }

    size_t CommandVector::size() const {
        return m_commands.size();
    }

    const std::vector<std::string>& CommandVector::strings() const {
        return m_strings;
    }

    bool CommandVector::currentIsRepeat() const {
        return (std::holds_alternative<Command_RepeatWrapper>(m_commands.at(m_commandIndex)));
    }

    std::string CommandVector::toString() const {
        std::string result;
        for (const auto& c : m_commands) {
            result += CommandParser::toString(c) + "\n";
        }
        return result;
    }

    void CommandVector::skipOverLabels() {
        while (std::holds_alternative<Command_Label>(m_commands.at(m_commandIndex))) {
            ++m_commandIndex;
            m_commandIndex %= m_commands.size();
            m_repeatCount = getCurrentRepeatCount();
        }
    }

    size_t CommandVector::getCurrentRepeatCount() const {
        return std::visit(overloaded{[](const Command_RepeatWrapper& c) { return c.repeatCount - 1; }, [](auto) { return 0; }},
                          m_commands.at(m_commandIndex));
    }

    void CommandVector::increment(const Command_Jump& commandJump) {
        const auto& jumpLabel = commandJump.label;
        const auto  it        = std::find_if(
            D_CIT(m_commands),
            D_FUNC(command, std::visit(overloaded{[&](const Command_Label& l) { return l.label == jumpLabel; }, D_FUNC(, false)}, command)));
        assert(it != m_commands.end());
        m_commandIndex = static_cast<size_t>(std::distance(m_commands.cbegin(), it));
        m_repeatCount  = getCurrentRepeatCount();
    }

    void CommandVector::increment(const Command_Simple&) {
        ++m_commandIndex;
        m_commandIndex %= m_commands.size();
        m_repeatCount = getCurrentRepeatCount();
    }

    void CommandVector::increment(const Command_RepeatWrapper&) {
        if (m_repeatCount == 0) {
            ++m_commandIndex;
            m_commandIndex %= m_commands.size();
            m_repeatCount = getCurrentRepeatCount();
        } else {
            --m_repeatCount;
        }
    }

} // namespace model
