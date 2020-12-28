//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_COMMAND_H
#define BLOCKS_COMMAND_H

#include <map>
#include <string>

namespace model {

    struct Command {

        /****** PUBLIC ENUMS / TYPEDEFS  ******/
        enum class TYPE { SKIP, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };
        enum class MODIFIER { NONE, IGNORE, INCREMENT };

        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Command() = default;
        Command(TYPE type, MODIFIER modifier);

        /****** PUBLIC STATIC FUNCTIONS  ******/
        static bool        canParse(const std::string& string);
        static TYPE        typeFromString(const std::string& string);
        static MODIFIER    modifierFromString(const std::string& string);
        static Command     fromString(const std::string& string);
        static std::string stringFromModifier(MODIFIER modifier);
        static std::string stringFromType(TYPE modifier);
        static std::string formatCommandString(std::string& string);

        /****** DATA MEMBERS  ******/
        TYPE     m_type     = TYPE::MOVE_UP;
        MODIFIER m_modifier = MODIFIER::NONE;

        /****** PUBLIC STATIC DATA MEMBERS  ******/
        static const std::map<std::string, TYPE>     s_typeMap;
        static const std::map<std::string, MODIFIER> s_modifierMap;
    };
} // namespace model

#endif // BLOCKS_COMMAND_H
