//
// Created by pc on 30-12-20.
//

#include "CommandParser.h"

#include "../../global/defines.h"
#include "../../global/overloaded.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>

namespace model {

    template <typename... A>
    Command f([[maybe_unused]] A... a) {
        return Command_Error{};
    }

    template <>
    Command f<>(COMMAND_TYPE t) {
        return Command_Simple{t};
    }

    template <>
    Command f<>(COMMAND_MODIFIER t, COMMAND_TYPE s) {
        return Command_Modified{s, t};
    }

    template <>
    Command f<>(COMMAND_TYPE s, int u) {
        return u <= 0 ? static_cast<Command>(Command_Error{}) : Command_RepeatWrapper{u, Command_Simple{s}};
    }

    template <>
    Command f<>(COMMAND_MODIFIER t, COMMAND_TYPE s, int u) {
        return u <= 0 ? static_cast<Command>(Command_Error{}) : Command_RepeatWrapper{u, Command_Modified{s, t}};
    }

    template <>
    Command f<>([[maybe_unused]] COMMAND_LABEL t, std::string s) {
        return Command_Label{std::move(s)};
    }

    template <>
    Command f<>([[maybe_unused]] COMMAND_JUMP t, std::string s) {
        return Command_Jump{std::move(s)};
    }

    Command CommandParser::parseString(const std::string& string) {
        const auto tokens = tokenize(string);
        if (tokens.empty() || tokens.size() > 3) {
            return Command_Error{};
        }
        switch (tokens.size()) {
            case 1:
                return std::visit([](auto t) { return f(t); }, tokens.front());
            case 2:
                return std::visit([](auto t, auto s) { return f(t, s); }, tokens[0], tokens[1]);
            case 3:
                return std::visit([](auto t, auto s, auto u) { return f(t, s, u); }, tokens[0], tokens[1], tokens[2]);
            default:
                return Command_Error{};
        }
        return Command_Error{};
    }

    CommandParser::Token CommandParser::tokenizeSingle(const std::string& string) {
        if (s_allTokens.find(string) != s_allTokens.end()) {
            return s_allTokens.at(string);
        }

        char* c;
        strtol(string.c_str(), &c, 10);
        if (*c != 0) {
            if (std::all_of(D_CIT(string), D_FUNC(c, std::isalnum(c) != 0))) {
                return string;
            }
            return model::CommandParser::ERROR_TOKEN::ERROR;
        }
        int result = std::stoi(string);
        return (result >= 100) ? static_cast<Token>(ERROR_TOKEN::ERROR) : result;
    }

    bool CommandParser::canParse(const std::string& string) {
        return stringType(string) != STRING_TYPE::ERROR;
    }

    std::string CommandParser::toString(const Token& token) {
        return std::visit(overloaded{[](int i) { return i == std::numeric_limits<int>::max() ? "INF" : std::to_string(i); },
                                     [](const std::string& str) { return std::all_of(D_CIT(str), D_FUNC(c, std::isalnum(c) != 0)) ? str : "Error"; },
                                     [&](auto) {
                                         const auto it = std::find_if(D_CIT(s_allTokens), D_FUNC(t, t.second == token));
                                         return it == s_allTokens.end() ? std::string("Error") : it->first;
                                     }},
                          token);
    }

    std::vector<CommandParser::Token> CommandParser::tokenize(const std::string& string) {
        std::vector<std::string> parts;
        const auto               trimmed = boost::trim_copy(string);
        boost::split(parts, trimmed, boost::is_any_of(" "), boost::token_compress_on);
        std::vector<Token> tokens;
        std::transform(D_CIT(parts), back_inserter(tokens), CommandParser::tokenizeSingle);
        return tokens;
    }

    std::string CommandParser::format(const std::string& string) {
        if (isCommentOrEmpty(string)) {
            return boost::trim_copy(string);
        }
        const auto  tokens = tokenize(string);
        std::string result;
        for (const auto& token : tokens) {
            result += toString(token);
            result += " ";
        }

        return tokens.empty() ? "" : result.substr(0, result.length() - 1);
    }

    std::string CommandParser::toString(const Command& command) {
        return std::visit(overloaded{D_FUNC(, std::string("Error")),
                                     [](const Command_Label& e) { return "LBL " + e.label; },
                                     [](const Command_Jump& e) { return "JMP " + e.label; },
                                     [](const Command_Simple& e) { return toString(e.type); },
                                     [](const Command_Modified& e) { return toString(e.modifier) + " " + toString(e.type); },
                                     [](const Command_RepeatWrapper& e) { return toString(toCommand(e)) + " " + std::to_string(e.repeatCount); }},
                          command);
    }

    bool CommandParser::isCommentOrEmpty(const std::string& string) {
        auto it = string.find_first_not_of(' ');
        return it == std::string::npos || string.at(it) == '#';
    }

    bool CommandParser::isFormatted(const std::string& string) {
        return string == format(string);
    }

    CommandParser::STRING_TYPE CommandParser::stringType(const std::string& string) {
        const auto index = string.find_first_not_of(' ');
        if (index == std::string::npos) {
            return STRING_TYPE::EMPTY;
        }
        if (string.at(index) == '#') {
            return STRING_TYPE::COMMENT;
        }
        return std::holds_alternative<Command_Error>(parseString(string)) ? STRING_TYPE::ERROR : STRING_TYPE::ACTION;
    }

} // namespace model