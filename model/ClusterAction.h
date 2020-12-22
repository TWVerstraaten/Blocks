//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_CLUSTERACTION_H
#define BLOCKS_CLUSTERACTION_H

#include <map>
#include <string>

namespace model {

    struct ClusterAction {

        enum class ACTION { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };
        enum class MODIFIER { NONE, IGNORE, SKIP };

        static ACTION   actionFromString(const std::string& string);
        static MODIFIER modifierFromString(const std::string& string);

        static std::string stringFromModifier(MODIFIER modifier);
        static std::string stringFromAction(ACTION modifier);

        static ClusterAction fromString(const std::string& string);
        static bool          canParse(const std::string& string);

        static const std::map<std::string, ACTION>   m_actionMap;
        static const std::map<std::string, MODIFIER> m_modifierMap;

        ACTION   m_action;
        MODIFIER m_modifier;
    };
} // namespace model

#endif // BLOCKS_CLUSTERACTION_H
