//
// Created by pc on 30-12-20.
//

#ifndef BLOCKS_COMMANDPARSER_H
#define BLOCKS_COMMANDPARSER_H

#include "Command_enums.h"

#include <string>
#include <variant>
#include <vector>

namespace model {

    struct CMD_Error {};

    struct CMD {
        COMMAND_TYPE m_commandType;
    };

    struct CMD_M {
        COMMAND_TYPE     m_commandType;
        COMMAND_MODIFIER m_commandModifier;
    };

    struct CMD_M_NUM {
        COMMAND_TYPE     m_commandType;
        COMMAND_MODIFIER m_commandModifier;
        int              m_repeats;
    };

    class CommandParser {
        /****** PRIVATE ENUMS / TYPEDEFS  ******/
        enum class ERROR_TOKEN { ERROR };
        typedef std::variant<int, COMMAND_MODIFIER, COMMAND_TYPE, COMMAND_LOOP, COMMAND_IF, COMMAND_COMPARE, ERROR_TOKEN> Token;

      public:
        /****** PUBLIC ENUMS / TYPEDEFS  ******/
        typedef std::variant<CMD, CMD_M, CMD_M_NUM, CMD_Error> m_Command;

        /****** CONSTRUCTORS / DESTRUCTORS  ******/

        /****** PUBLIC STATIC FUNCTIONS  ******/
        static m_Command parseString(const std::string& t);

        /****** PUBLIC VIRTUAL FUNCTIONS  ******/

        /****** CONST GETTERS  ******/

        /****** CONST FUNCTIONS  ******/

        /****** NON CONST FUNCTIONS  ******/

        /****** OPERATORS  ******/

        /****** FRIENDS  ******/

      private:
        /****** PRIVATE STATIC FUNCTIONS  ******/
        static Token tokenize(const std::string& str);
        /****** PRIVATE CONST FUNCTIONS  ******/

        /****** PRIVATE NON CONST FUNCTIONS  ******/

        /****** DATA MEMBERS  ******/

      public:
        /****** PUBLIC STATIC DATA MEMBERS  ******/
    };

} // namespace model

#endif // BLOCKS_COMMANDPARSER_H
