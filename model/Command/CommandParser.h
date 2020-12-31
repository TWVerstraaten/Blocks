//
// Created by pc on 30-12-20.
//

#ifndef BLOCKS_COMMANDPARSER_H
#define BLOCKS_COMMANDPARSER_H

#include "Command.h"
#include "Command_enums.h"

#include <map>
#include <string>
#include <variant>
#include <vector>

namespace model {

    class CommandParser {

      public:
        enum class ERROR_TOKEN { ERROR };
        typedef std::variant<int, COMMAND_MODIFIER, COMMAND_TYPE, COMMAND_LOOP, COMMAND_IF, COMMAND_COMPARE, ERROR_TOKEN> Token;

        /****** PUBLIC STATIC FUNCTIONS  ******/
        static Command     parseString(const std::string& string);
        static bool        canParse(const std::string& string);
        static std::string toString(const Token& token);
        static std::string toString(const Command& command);
        static std::string format(const std::string& string);
        static bool        isCommentOrEmpty(const std::string& string);

      private:
        /****** PRIVATE STATIC FUNCTIONS  ******/
        static Token              tokenizeSingle(const std::string& string);
        static std::vector<Token> tokenize(const std::string& string);

        /****** DATA MEMBERS  ******/
        inline static const std::map<const std::string, Token> s_allTokens{{"+", COMMAND_MODIFIER::IGNORE},
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
    };

} // namespace model

#endif // BLOCKS_COMMANDPARSER_H
