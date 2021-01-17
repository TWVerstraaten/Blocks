//
// Created by pc on 30-12-20.
//

#ifndef BLOCKS_COMMANDPARSER_H
#define BLOCKS_COMMANDPARSER_H

#include "Command.h"
#include "Command_enums.h"

#include <limits>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace model {

    class CommandParser {

      public:
        /****** PUBLIC ENUMS / TYPEDEFS  ******/
        enum class STRING_TYPE { EMPTY, COMMENT, ACTION, ERROR };
        enum class ERROR_TOKEN { ERROR };
        typedef std::variant<int,
                             std::string,
                             COMMAND_MODIFIER,
                             COMMAND_LABEL,
                             COMMAND_JUMP,
                             COMMAND_TYPE,
                             COMMAND_LOOP,
                             COMMAND_IF,
                             COMMAND_COMPARE,
                             ERROR_TOKEN>
            Token;

        /****** PUBLIC STATIC FUNCTIONS  ******/
        static bool        canParse(const std::string& string);
        static bool        isCommentOrEmpty(const std::string& string);
        static bool        isFormatted(const std::string& string);
        static STRING_TYPE stringType(const std::string& string);
        static Command     parseString(const std::string& string);
        static std::string toString(const Token& token);
        static std::string toString(const Command& command);
        static std::string format(const std::string& string);

      private:
        /****** PRIVATE STATIC FUNCTIONS  ******/
        static Token              tokenizeSingle(const std::string& string);
        static std::vector<Token> tokenize(const std::string& string);

        /****** DATA MEMBERS  ******/
        inline static const std::map<const std::string, Token> s_allTokens{{"+", COMMAND_MODIFIER::IGNORE}, {"-", COMMAND_MODIFIER::INCREMENT},
                                                                           {"RHT", COMMAND_TYPE::RHT},      {"LFT", COMMAND_TYPE::LFT},
                                                                           {"FWD", COMMAND_TYPE::FWD},      {"BCK", COMMAND_TYPE::BCK},
                                                                           {"SKP", COMMAND_TYPE::SKP},      {"STP", COMMAND_TYPE::STP},
                                                                           {"GRB", COMMAND_TYPE::GRB},      {"SPL", COMMAND_TYPE::SPL},
                                                                           {">", COMMAND_COMPARE::GREATER}, {">=", COMMAND_COMPARE::GREATER_EQUAL},
                                                                           {"<", COMMAND_COMPARE::LESS},    {"<=", COMMAND_COMPARE::LESS_EQUAL},
                                                                           {"=", COMMAND_COMPARE::EQUAL},   {"FORI", COMMAND_LOOP::FORI},
                                                                           {"IFSIZE", COMMAND_IF::IFSIZE},  {"LBL", COMMAND_LABEL::LABEL},
                                                                           {"JMP", COMMAND_JUMP::JUMP},     {"INF", std::numeric_limits<int>::max()}};
    };

} // namespace m_model

#endif // BLOCKS_COMMANDPARSER_H
