//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_COMMAND_H
#define BLOCKS_COMMAND_H

#include <map>
#include <string>

namespace model {

    struct Command {
        Command() = default;

        enum class VALUE { SKIP, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };
        enum class MODIFIER { NONE, IGNORE, INCREMENT };

        Command(VALUE value, MODIFIER modifier);

        static VALUE    valueFromString(const std::string& string);
        static MODIFIER modifierFromString(const std::string& string);

        static std::string stringFromModifier(MODIFIER modifier);
        static std::string stringFromAction(VALUE modifier);

        static Command fromString(const std::string& string);
        static bool    canParse(const std::string& string);

        static const std::map<std::string, VALUE>    m_valueMap;
        static const std::map<std::string, MODIFIER> m_modifierMap;

        VALUE              m_value    = VALUE::MOVE_UP;
        MODIFIER           m_modifier = MODIFIER::NONE;
        static std::string formatActionString(std::string& string);
    };
} // namespace model

#endif // BLOCKS_COMMAND_H
