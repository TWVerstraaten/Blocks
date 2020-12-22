//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_ACTION_H
#define BLOCKS_ACTION_H

#include <map>
#include <string>

namespace model {

    struct Action {

        enum class VALUE { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };
        enum class MODIFIER { NONE, IGNORE, SKIP };

        static VALUE    actionFromString(const std::string& string);
        static MODIFIER modifierFromString(const std::string& string);

        static std::string stringFromModifier(MODIFIER modifier);
        static std::string stringFromAction(VALUE modifier);

        static Action fromString(const std::string& string);
        static bool   canParse(const std::string& string);

        static const std::map<std::string, VALUE>    m_actionMap;
        static const std::map<std::string, MODIFIER> m_modifierMap;

        VALUE    m_action;
        MODIFIER m_modifier;
    };
} // namespace model

#endif // BLOCKS_ACTION_H
