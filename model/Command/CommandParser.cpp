//
// Created by pc on 30-12-20.
//

#include "CommandParser.h"

#include "../../global/fns.h"
#include "../../global/overloaded.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>

namespace model {
    template <typename... A>
    std::variant<CMD, CMD_M, CMD_M_NUM, CMD_Error> f(A... a) {
        std::cout << "Error\n";
        return CMD_Error{};
    }

    template <>
    std::variant<CMD, CMD_M, CMD_M_NUM, CMD_Error> f<>(COMMAND_TYPE t) {
        return CMD{t};
    }

    template <>
    std::variant<CMD, CMD_M, CMD_M_NUM, CMD_Error> f<>(COMMAND_MODIFIER t, COMMAND_TYPE s) {
        return CMD_M{s, t};
    }

    template <>
    std::variant<CMD, CMD_M, CMD_M_NUM, CMD_Error> f<>(COMMAND_MODIFIER t, COMMAND_TYPE s, int u) {
        return CMD_M_NUM{s, t, u};
    }

    CommandParser::m_Command CommandParser::parseString(const std::string& string) {
        const auto trimmedString = fns::trimWhiteSpace(string);

        std::istringstream buffer(trimmedString);
        std::vector<Token> tokens;
        std::transform(
            std::istream_iterator<std::string>(buffer), std::istream_iterator<std::string>(), back_inserter(tokens), CommandParser::tokenize);
        if (tokens.empty() || tokens.size() > 3) {
            return CMD_Error{};
        }
        m_Command result;
        switch (tokens.size()) {
            case 1:
                result = std::visit([](auto t) { return f(t); }, tokens.front());
                break;
            case 2:
                result = std::visit([](auto t, auto s) { return f(t, s); }, tokens[0], tokens[1]);
                break;
            case 3:
                result = std::visit([](auto t, auto s, auto u) { return f(t, s, u); }, tokens[0], tokens[1], tokens[2]);
                break;
        }
        return result;
    }

    bool isInt(const std::string& line) {
        char* p;
        strtol(line.c_str(), &p, 10);
        return *p == 0;
    }

    CommandParser::Token CommandParser::tokenize(const std::string& str) {
        static const std::map<const std::string, Token> s_allToken{{"+", COMMAND_MODIFIER::IGNORE},
                                                                   {"-", COMMAND_MODIFIER::INCREMENT},
                                                                   {"RHT", COMMAND_TYPE::RHT},
                                                                   {"LFT", COMMAND_TYPE::LFT},
                                                                   {"FWD", COMMAND_TYPE::FWD},
                                                                   {"BCK", COMMAND_TYPE::BCK},
                                                                   {"SKP", COMMAND_TYPE::SKP},
                                                                   {"STP", COMMAND_TYPE::STP},
                                                                   {"GRB", COMMAND_TYPE::GRB},
                                                                   {"SPL", COMMAND_TYPE::SPL},
                                                                   {">", COMMAND_COMPARE::GREATER},
                                                                   {">=", COMMAND_COMPARE::GREATER_EQUAL},
                                                                   {"<", COMMAND_COMPARE::LESS},
                                                                   {"<=", COMMAND_COMPARE::LESS_EQUAL},
                                                                   {"=", COMMAND_COMPARE::EQUAL},
                                                                   {"FORI", COMMAND_LOOP::FORI},
                                                                   {"IFSIZE", COMMAND_IF::IFSIZE}};
        if (s_allToken.find(str) != s_allToken.end()) {
            return s_allToken.at(str);
        }

        try {
            if (isInt(str)) {
                int n = std::stoi(str);
                return n;
            } else {
                return model::CommandParser::ERROR_TOKEN::ERROR;
            }
        } catch (...) { return model::CommandParser::ERROR_TOKEN::ERROR; }
    }

} // namespace model